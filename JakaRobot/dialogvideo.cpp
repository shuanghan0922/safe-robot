#include "dialogvideo.h"
#include "ui_dialogvideo.h"
#include "videodevice.h"
#include <unistd.h>
#include "opencv2/imgproc/types_c.h"
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <iostream>

using namespace cv;
//luvcview -d /dev/video0 -f jpg -s 320x240
extern "C"
{
#include <stdio.h>
#include <stdlib.h>
}

DialogVideo::DialogVideo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogVideo)
{
    ui->setupUi(this);
    //frame = new QImage(p,640,480,QImage::Format_RGB888);

    //frame = new QImage(p,320,240,QImage::Format_RGB888);
    //qDebug() << frame->format();
    //RecvIp = new QLabel(tr("RecvIP:"));
    //label = new QLabel(this);
    //save = new QPushButton(tr("take photo"),this);
    //vd = new VideoDevice(tr("/dev/video0"));
    ca=new Camera(640, 480, 320, 240, 30);
    //std::cout << "camera init failed\n" << std::endl;
    //ca=new Camera(320, 240, 320, 240, 30);
    //connect(ca, SIGNAL(framesReady()), this,SLOT(update()));
    ca->start();
    //ComboBox = new QComboBox();
    //ComboBox->clear();
    //ComboBox->adjustSize();
    ipnum = 0;
    takeflag = 0;
    RTPSessionParams sessionparams;
    RTPUDPv4TransmissionParams transparams;
    sessionparams.SetOwnTimestampUnit(1.0/90000.0);
    sessionparams.SetAcceptOwnPackets(false);
    sessionparams.SetUsePollThread(true);
    transparams.SetPortbase(9000);
    int status = session.Create(sessionparams,&transparams);
    if(status < 0){
        std::cerr << RTPGetErrorString(status) << std::endl;
         QMessageBox::warning(this,tr("error"),tr("session.Create failed"),QMessageBox::Yes);
    }

    session.SetDefaultPayloadType(25);
    session.SetDefaultMark(false);
    session.SetDefaultTimestampIncrement(3000);
    /****************
    connect(vd, SIGNAL(display_error(QString)), this,SLOT(display_error(QString)));
    rs = vd->open_device();
    if(-1==rs)
    {
        QMessageBox::warning(this,tr("error"),tr("open /dev/dsp error"),QMessageBox::Yes);
        vd->close_device();
    }
    rs = vd->init_device();
    if(-1==rs)
    {
        QMessageBox::warning(this,tr("error"),tr("init failed"),QMessageBox::Yes);
        vd->close_device();
    }

    rs = vd->start_capturing();
    if(-1==rs)
    {
        QMessageBox::warning(this,tr("error"),tr("start capture failed"),QMessageBox::Yes);
        vd->close_device();
    }

    if(-1==rs)
    {
        QMessageBox::warning(this,tr("error"),tr("get frame failed"),QMessageBox::Yes);
        vd->stop_capturing();
    }
     *****************/
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    timer->start(30);
    //connect(ca, &Camera::framesReady(), this,SLOT(update()));
    //connect(ca, SIGNAL(Camera::framesReady()), this,SLOT(update()));
    //connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(take_a_photo()));
    connect(&session,SIGNAL(AddDestIp(unsigned int,unsigned short)),\
            this,SLOT(AddDestIPAction(unsigned int,unsigned short)));
    connect(&session,SIGNAL(DeleteDestIp(unsigned int,unsigned short)),\
            this,SLOT(DeleteDestIpAction(unsigned int,unsigned short)));
    /*************
    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addWidget(RecvIp);
    hLayout->addWidget(ComboBox);

    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addWidget(label);
    vLayout->addStretch();
    vLayout->addLayout(hLayout);
    vLayout->addWidget(save);

    setLayout(vLayout);
    setWindowTitle(tr("Capture"));
    ****************/
    udpBroadCastInit();
    udpTimer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(SendBroadCast()));
    udpTimer->start(500);
}

DialogVideo::~DialogVideo()
{
    RTPTime delay(10.0);
    //rs = vd->stop_capturing();
    //rs = vd->uninit_device();
    //rs = vd->close_device();

    session.BYEDestroy(delay,"Time's up",9);
    std::cout << "close" << std::endl;
    //
    udpTimer->stop();
    timer->stop();
    ca->stop();
    ca->quit();
    ca->wait();
    close();
    delete ui;
}

void DialogVideo::save_mjpeg()
{
     QDateTime current_date_time = QDateTime::currentDateTime();
     QString name("/home/hfdxp/CameraRead/");
     QString current_date = current_date_time.toString("yyyy-MM-dd_hh:mm:ss");
     current_date.append(".jpg");
     name.append(current_date);
     char*  ch;
     QByteArray ba = name.toLatin1();
     ch = ba.data();
       int  fd = ::open(ch,O_CREAT|O_RDWR|O_TRUNC,0666);
     if(fd < 0){
         std::cout << "open failed\n" << std::endl;
         std::cout << ch << std::endl;
     }else{
         std::cout << "write\n" << ch << std::endl;
         std::cout << "buffer->destlength:" << ca->len << std::endl;
         write(fd,ca->p,ca->len);
         ::close(fd);
     }
}
void DialogVideo::take_a_photo()
{
    takeflag = 1;
}
void DialogVideo::AddDestIPAction(unsigned int ip,unsigned short port)
{
    struct in_addr inaddr;
    inaddr.s_addr = htonl(ip);
    QString IP(inet_ntoa(inaddr));
    QString str = IP + ":" + QString::number(port);
    ipnum++;
    //ComboBox->addItem(str);
}
void DialogVideo::udpBroadCastInit(){
#define BROADCAST_PORT 13145
     char send[] = "port:9000";
     udpFd = socket(AF_INET,SOCK_DGRAM,0);
     bzero(&servaddr,sizeof(servaddr));
     servaddr.sin_family = AF_INET;
     inet_aton("255.255.255.255",&servaddr.sin_addr);
     servaddr.sin_port   = htons(BROADCAST_PORT);
     const int on = 1;
     setsockopt(udpFd,SOL_SOCKET,SO_BROADCAST,&on,sizeof(on));
     sendto(udpFd,send,strlen(send),0,(sockaddr *)&servaddr,sizeof(servaddr));
}

void DialogVideo::DeleteDestIpAction(unsigned int ip,unsigned short port)
{
    struct in_addr inaddr;
    int index;
    inaddr.s_addr = htonl(ip);
    QString IP(inet_ntoa(inaddr));
    QString str = IP + ":" + QString::number(port);
    //index = ComboBox->findText(str);
    //ComboBox->removeItem(index);
    ipnum--;
}

void DialogVideo::paintEvent(QPaintEvent *)
{
    int n,last,i;
    int status;
    char *picturebuffer;

    //rs = vd->get_frame((void **)&p,&len);
    //frame->loadFromData(ca->p,ca->len * sizeof(char));
    /*****
    if(recognitionFlag == 1){
        m = QImage2cvMat(*frame);
        detectAndDisplay(m);
    }
    **********/
    //label->setPixmap(QPixmap::fromImage(*frame,Qt::AutoColor));
    ui->label->setPixmap(QPixmap::fromImage(ca->qimg));
    if(ipnum > 0){
        picturebuffer = (char *)malloc(ca->len + sizeof(struct Packet));
        struct Packet *sendPacket = (struct Packet *)picturebuffer;
        sendPacket->magic1 = 0x12345678;
        sendPacket->magic2 = 0x87654321;
        sendPacket->sum =  sendPacket->magic1 +  sendPacket->magic2;
        sendPacket->lenght = ca->len;
        // std::cout << "len = " << len + sizeof(struct Packet) << std::endl;
        memcpy(sendPacket->body,ca->p,ca->len);
        n = (sendPacket->lenght + sizeof(struct Packet)) / 1000;
        last = (sendPacket->lenght + sizeof(struct Packet)) % 1000;
        for(int i = 0 ; i < n ; i++){
            if(i == n - 1){
                if(last == 0){
                     status = session.SendPacket(picturebuffer + i*1000,1000,25,true,0);
                }else{
                     status = session.SendPacket(picturebuffer + i*1000,1000,25,true,0);
                     status = session.SendPacket(picturebuffer + n*1000,last,25,true,0);
                }
            }else{
                 if(i == 0)
                    status = session.SendPacket(picturebuffer + i*1000,1000,25,true,3000);
                 else
                    status = session.SendPacket(picturebuffer + i*1000,1000,25,true,0);
            }
            if(status < 0){
                std::cerr << RTPGetErrorString(status) << std::endl;
            }
            if(i % 10 == 0){
                usleep(100);
            }
        }
        free(picturebuffer);
    }
    if(takeflag == 1){
        takeflag = 0;
        save_mjpeg();
    }
    //rs = vd->unget_frame();


}


void DialogVideo::display_error(QString err)
{
    QMessageBox::warning(this,tr("error"), err,QMessageBox::Yes);
}

/*yuv格式转换为rgb格式*/
int DialogVideo::convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height)
{
 unsigned int in, out = 0;
 unsigned int pixel_16;
 unsigned char pixel_24[3];
 unsigned int pixel32;
 int y0, u, y1, v;
 for(in = 0; in < width * height * 2; in += 4) {
      pixel_16 =
       yuv[in + 3] << 24 |
       yuv[in + 2] << 16 |
       yuv[in + 1] <<  8 |
       yuv[in + 0];
      y0 = (pixel_16 & 0x000000ff);
      u  = (pixel_16 & 0x0000ff00) >>  8;
      y1 = (pixel_16 & 0x00ff0000) >> 16;
      v  = (pixel_16 & 0xff000000) >> 24;
      pixel32 = convert_yuv_to_rgb_pixel(y0, u, v);
      pixel_24[0] = (pixel32 & 0x000000ff);
      pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
      pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
      rgb[out++] = pixel_24[0];
      rgb[out++] = pixel_24[1];
      rgb[out++] = pixel_24[2];
      pixel32 = convert_yuv_to_rgb_pixel(y1, u, v);
      pixel_24[0] = (pixel32 & 0x000000ff);
      pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
      pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
      rgb[out++] = pixel_24[0];
      rgb[out++] = pixel_24[1];
      rgb[out++] = pixel_24[2];
 }
 return 0;
}

int DialogVideo::convert_yuv_to_rgb_pixel(int y, int u, int v)
{
     unsigned int pixel32 = 0;
     unsigned char *pixel = (unsigned char *)&pixel32;
     int r, g, b;
     r = y + (1.370705 * (v-128));
     g = y - (0.698001 * (v-128)) - (0.337633 * (u-128));
     b = y + (1.732446 * (u-128));
     if(r > 255) r = 255;
     if(g > 255) g = 255;
     if(b > 255) b = 255;
     if(r < 0) r = 0;
     if(g < 0) g = 0;
     if(b < 0) b = 0;
     pixel[0] = r * 220 / 256;
     pixel[1] = g * 220 / 256;
     pixel[2] = b * 220 / 256;
     return pixel32;
}
/*yuv格式转换为rgb格式*/

QImage DialogVideo::cvMat2QImage(const cv::Mat& mat)
{


    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4)
    {
        //qDebug() << "CV_8UC4";
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}
cv::Mat DialogVideo::QImage2cvMat(QImage image)
{
    cv::Mat mat;
    switch(image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        //mat.data = image.constBits()
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, CV_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    default:
        qDebug() << image.format();
    }
    return mat;
}


QImage DialogVideo::detectAndDisplay(cv::Mat frame)
{
      std::vector<Rect> faces;
      cv::Mat frame_gray;

      cv::cvtColor( frame, frame_gray, cv::COLOR_BGR2GRAY );
      cv::equalizeHist( frame_gray, frame_gray );

      //-- Detect faces
      face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|cv::CASCADE_SCALE_IMAGE, Size(30, 30) );

      for ( size_t i = 0; i < faces.size(); i++ )
      {
          cv::Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
          cv::ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2 ), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );

          cv::Mat faceROI = frame_gray( faces[i] );
          std::vector<Rect> eyes;

          //-- In each face, detect eyes
          eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CASCADE_SCALE_IMAGE, Size(30, 30) );

          for ( size_t j = 0; j < eyes.size(); j++ )
          {
              Point eye_center( faces[i].x + eyes[j].x + eyes[j].width/2, faces[i].y + eyes[j].y + eyes[j].height/2 );
              int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
              circle( frame, eye_center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
          }
      }
      return cvMat2QImage(frame);
}


void DialogVideo::on_pushButton_clicked()
{
    QMessageBox::information(this,"提示","进入按下BUTTON");
}
