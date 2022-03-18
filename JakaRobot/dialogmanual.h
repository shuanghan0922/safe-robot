#ifndef DIALOGMANUAL_H
#define DIALOGMANUAL_H
#include "mygauge2.h"
//#include "dialogvideo.h"
#include <QDialog>
#include <QVBoxLayout>
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
#include <iostream>
//video
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
//video end


namespace Ui {
class DialogManual;
}

class DialogManual : public QDialog
{
    Q_OBJECT

public:
    explicit DialogManual(QWidget *parent = nullptr);
    ~DialogManual();
    void paintEvent(QPaintEvent *);
    QTimer *timerstu;
    void kkopt(QString no,int st);//开关编号，到达状态
    void drawlamp(int x,int y,int radius,float scale,int col);
    int getvideo();
signals:
    void Sequid(const QString id);//send to mainwindow

private slots:
    void dispstu();
    void on_pushButton_clicked();

    void on_pushButtonman_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton2pon_clicked();

    void on_pushButton2poff_clicked();

    void on_pushButton3pon_clicked();

    void on_pushButton3poff_clicked();

    void on_pushButtonon21_clicked();

    void on_pushButtonoff21_clicked();

    void on_pushButtonon22_clicked();

    void on_pushButtonoff22_clicked();

    void on_pushButton22min_clicked();

    void on_pushButtonon23_clicked();

    void on_pushButtonoff23_clicked();

    void on_pushButtonon24_clicked();

    void on_pushButtonoff24_clicked();

    void on_pushButtonon25_clicked();

    void on_pushButtonoff25_clicked();

    void on_pushButton1on_clicked();

    void on_pushButton1off_clicked();

    void on_pushButton2on_clicked();

    void on_pushButton2off_clicked();

    void on_pushButton3on_clicked();

    void on_pushButton3off_clicked();

    void on_pushButton4on_clicked();

    void on_pushButton4off_clicked();

    void on_pushButton31on_clicked();

    void on_pushButton31off_clicked();

    void on_pushButton32on_clicked();

    void on_pushButton32off_clicked();

    void on_pushButton32min_clicked();

    void on_pushButton33on_clicked();

    void on_pushButton33off_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButtonloc_clicked();

    void on_pushButtonfar_clicked();

    void on_pushButtonzk1off_clicked();

    void on_pushButtonzk1on_clicked();

    void on_pushButtonbreoff_clicked();

    void on_pushButtonbreon_clicked();

    void on_pushButtondissoff_clicked();

    void on_pushButtondisson_clicked();

    void on_pushButtonearoff_clicked();

    void on_pushButtonearon_clicked();

    void on_pushButtonzk2off_clicked();

    void on_pushButtonzk2on_clicked();

    void on_pushButtonzk3off_clicked();

    void on_pushButtonzk3on_clicked();

    void on_pushButton_37_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_pushButton_38_clicked();

    void on_pushButton2poff_2_clicked();

    void on_pushButton2pon_2_clicked();

    void on_pushButtontest_clicked();

    void on_pushButtontest_2_clicked();

    void on_pushButtontest_3_clicked();

    void on_pushButtontest_4_clicked();

    void on_pushButton_39_clicked();
    //video


private:
    Ui::DialogManual *ui;
    //room1
    myGauge2 *sels;
    myGauge2 *ears;
    myGauge2 *diss;
    myGauge2 *bres;
    QVBoxLayout *mainLayout1;
    QVBoxLayout *mainLayout2;
    QVBoxLayout *mainLayout3;
    QVBoxLayout *mainLayout4;
    //room2
    myGauge2 *sel21;
    QVBoxLayout *mainLayout21;
    myGauge2 *sel22;
    QVBoxLayout *mainLayout22;
    myGauge2 *sel23;
    QVBoxLayout *mainLayout23;
    myGauge2 *sel24;
    QVBoxLayout *mainLayout24;
    myGauge2 *sel25;
    QVBoxLayout *mainLayout25;
    myGauge2 *sel26;
    QVBoxLayout *mainLayout26;
    myGauge2 *sel27;
    QVBoxLayout *mainLayout27;
    myGauge2 *sel28;
    QVBoxLayout *mainLayout28;
    //测试
    myGauge2 *xn21;
    QVBoxLayout *mainLayout21t;
    myGauge2 *xn22;
    QVBoxLayout *mainLayout22t;
    myGauge2 *xn23;
    QVBoxLayout *mainLayout23t;
    myGauge2 *xn24;
    QVBoxLayout *mainLayout24t;
    myGauge2 *xn25;
    QVBoxLayout *mainLayout25t;
    myGauge2 *xn26;
    QVBoxLayout *mainLayout26t;
    myGauge2 *xn27;
    QVBoxLayout *mainLayout27t;
    myGauge2 *xn28;
    QVBoxLayout *mainLayout28t;

    //Video
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
    //void paintEvent(QPaintEvent *);
    void display_error(QString err);
    void take_a_photo();
    void AddDestIPAction(unsigned int ip,unsigned short port);
    void DeleteDestIpAction(unsigned int ip,unsigned short port);
    void SendBroadCast(){
        char send[] = "port:9000";
        sendto(udpFd,send,strlen(send),0,(sockaddr *)&servaddr,sizeof(servaddr));
    }

    void on_pushButtonvon_clicked();
};

#endif // DIALOGMANUAL_H
