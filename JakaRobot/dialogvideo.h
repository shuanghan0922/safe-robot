#ifndef DIALOGVIDEO_H
#define DIALOGVIDEO_H

#include <QDialog>
#include <QtGui>
#include <iostream>
//#include "videodevice.h"
#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include "rtpsession.h"
#include "rtpsessionparams.h"
#include "rtpudpv4transmitter.h"
#include "rtpipv4address.h"
#include "rtptimeutilities.h"
#include "rtppacket.h"
#include "rtpsourcedata.h"
#include "rtcpsrpacket.h"
#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"
#include "camera.h"

using namespace jrtplib;

struct Packet {
    uint32_t magic1;
    uint32_t magic2;
    uint32_t sum;
    uint32_t lenght;
    uchar body[];
};
class MyRTPSession :public QObject,public RTPSession{
    Q_OBJECT
protected:
    void OnNewSource(RTPSourceData *dat){
        if(dat->IsOwnSSRC()){
            return;
        }
        u_int32_t ip;
        u_int16_t port;
        if(dat->GetRTPDataAddress() != 0){
            const RTPIPv4Address *addr = \
                    (const RTPIPv4Address *)(dat->GetRTPDataAddress());
            ip = addr->GetIP();
            port = addr->GetPort();
        }else if(dat->GetRTCPDataAddress() != 0){
            const RTPIPv4Address *addr = \
                        (const RTPIPv4Address *)(dat->GetRTCPDataAddress());
            ip = addr->GetIP();
            port = addr->GetPort() - 1;
        }else
            return ;
        RTPIPv4Address dest(ip,port);
        AddDestination(dest);
        std::cout << "add ip" << std::endl;
        emit AddDestIp(ip,port);
    }
    void OnBYEPacket(RTPSourceData *dat){
        if(dat->IsOwnSSRC()){
            return;
        }
        u_int32_t ip;
        u_int16_t port;
        if(dat->GetRTPDataAddress() != 0){
            const RTPIPv4Address *addr = \
                    (const RTPIPv4Address *)(dat->GetRTPDataAddress());
            ip = addr->GetIP();
            port = addr->GetPort();
        }else if(dat->GetRTCPDataAddress() != 0){
            const RTPIPv4Address *addr = \
                        (const RTPIPv4Address *)(dat->GetRTCPDataAddress());
            ip = addr->GetIP();
            port = addr->GetPort() - 1;
        }else
            return ;

        RTPIPv4Address dest(ip,port);
        DeleteDestination(dest);
        emit DeleteDestIp(ip,port);
    }
    void OnRemoveSource(RTPSourceData *dat){
        if(dat->IsOwnSSRC())
            return ;
        if(dat->ReceivedBYE())
            return ;
        u_int32_t ip;
        u_int16_t port;

        if(dat->GetRTPDataAddress() != 0){
            const RTPIPv4Address *addr = \
                    (const RTPIPv4Address *)(dat->GetRTPDataAddress());
            ip = addr->GetIP();
            port = addr->GetPort();
        }else if(dat->GetRTCPDataAddress() != 0){
            const RTPIPv4Address *addr = \
                        (const RTPIPv4Address *)(dat->GetRTCPDataAddress());
            ip = addr->GetIP();
            port = addr->GetPort() - 1;
        }else
            return ;

        RTPIPv4Address dest(ip,port);
        DeleteDestination(dest);
        emit DeleteDestIp(ip,port);
    }
      void OnRTCPCompoundPacket(RTCPCompoundPacket *pack,\
                                const RTPTime &receivetime,\
                                const RTPAddress *senderaddress){
          RTCPPacket *rtcppack;
          pack->GotoFirstPacket();
          while ((rtcppack = pack->GetNextPacket()) != 0){
              if (rtcppack->IsKnownFormat()){
                  switch (rtcppack->GetPacketType()){
                      case RTCPPacket::SR:{
                          RTCPSRPacket *p = (RTCPSRPacket *)rtcppack;
                          uint32_t senderssrc = p->GetSenderSSRC();
                          std::cout << "SR Info:" << std::endl;
                          std::cout << "        NTP timestamp:    " << p->GetNTPTimestamp().GetMSW() << ":" << p->GetNTPTimestamp().GetLSW() << std::endl;
                          std::cout << "        RTP timestamp:    " << p->GetRTPTimestamp() << std::endl;
                          std::cout << "        Packet count:     " <<  p->GetSenderPacketCount() << std::endl;
                          std::cout << "        Octet count:      " << p->GetSenderOctetCount() << std::endl;
                          std::cout << "        SSRC       :      " << senderssrc               << std::endl;
                          break;
                      }
                      case RTCPPacket::RR:{
                          std::cout << " a RR" << std::endl;
                          break;
                      }
                      case RTCPPacket::SDES:{
                           std::cout << " a SDES" << std::endl;
                          break;
                      }
                      case RTCPPacket::BYE:{
                          std::cout << " a sender Bye" << std::endl;
                          break;
                      }
                      case RTCPPacket::APP:{
                          std::cout << " a APP" << std::endl;
                          break;
                      }
                      default:
                          std::cout << " unKnown RTCP Packet" << std::endl;
                  }
              }
          }
      }

signals:
    void AddDestIp(unsigned int ip,unsigned short port);
    void DeleteDestIp(unsigned int ip,unsigned short port);
};

namespace Ui {
class DialogVideo;
}

class DialogVideo : public QDialog
{
    Q_OBJECT

public:
    explicit DialogVideo(QWidget *parent = nullptr);
    ~DialogVideo();

private:
    Ui::DialogVideo *ui;
    cv::Mat    m;
    cv::String face_cascade_name;
    cv::String eyes_cascade_name;
    cv::CascadeClassifier face_cascade;
    cv::CascadeClassifier eyes_cascade;
    //QLabel *label;
    QImage *frame;
    //QPushButton *save;
    QTimer *timer;
    QTimer *udpTimer;
    //QComboBox *ComboBox;
    //QLabel *RecvIp;
    MyRTPSession session;
    int rs;
    uchar ipnum;

    uchar recognitionFlag;

    unsigned char takeflag;
    int udpFd;
    struct sockaddr_in servaddr;
    int convert_yuv_to_rgb_pixel(int y, int u, int v);
    int convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height);
    //VideoDevice *vd;
    Camera *ca;
    void save_mjpeg();
    void udpBroadCastInit();
    QImage detectAndDisplay(cv::Mat frame);
    QImage cvMat2QImage(const cv::Mat& mat);
    cv::Mat QImage2cvMat(QImage image);
private slots:
    void paintEvent(QPaintEvent *);
    void display_error(QString err);
    void take_a_photo();
    void AddDestIPAction(unsigned int ip,unsigned short port);
    void DeleteDestIpAction(unsigned int ip,unsigned short port);
    void SendBroadCast(){
        char send[] = "port:9000";
        sendto(udpFd,send,strlen(send),0,(sockaddr *)&servaddr,sizeof(servaddr));
    }
    void on_pushButton_clicked();
};

#endif // DIALOGVIDEO_H
