#include "dialogmanual.h"
#include "ui_dialogmanual.h"
//#include "mygauge2.h"
#include <QMessageBox>
#include "ComData.h"
#include <QSqlQuery>
#include "jkrobot.h"
#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include "example.hpp"
#include <fstream>              // File IO
#include <iostream>             // Terminal IO
#include <sstream>              // Stringstreams
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include "videodevice.h"
#include <unistd.h>
#include "opencv2/imgproc/types_c.h"

using namespace cv;
//luvcview -d /dev/video0 -f jpg -s 320x240
extern "C"
{
#include <stdio.h>
#include <stdlib.h>
}


DialogManual::DialogManual(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogManual)
{
    ui->setupUi(this);
   //room1--------------------------------------
    mainLayout1=new QVBoxLayout();
    mainLayout1->setSpacing(0);
    mainLayout1->setContentsMargins(0,2,0,40);

    sels=new myGauge2(ui->groupBoxsels);//     frmsels);//test);
    sels->setAnimating(false);//true

    mainLayout1->addWidget(sels);

    ui->groupBoxsels->setLayout(mainLayout1);

    sels->initVariables("手动","就地","远方");
    int value=50;
    sels->setValue(value);
    //-----------------------------------------------
    mainLayout2=new QVBoxLayout();
    mainLayout2->setSpacing(0);
    mainLayout2->setContentsMargins(0,0,0,40);
    bres=new myGauge2(ui->groupBoxbres);//     frmbres);//test);
    bres->setAnimating(false);//true

    mainLayout2->addWidget(bres);

   // ui->frmbres->setLayout(mainLayout1);
    ui->groupBoxbres->setLayout(mainLayout2);
    //bres->setVisible(true);
    bres->initVariables("分闸","O","合闸");
    value=70;
    bres->setValue(value);
   //------------------------------------------------------
    mainLayout3=new QVBoxLayout();
    mainLayout3->setSpacing(0);
    mainLayout3->setContentsMargins(0,0,0,40);
    diss=new myGauge2(ui->groupBoxdiss);//     frmdiss);//test);
    diss->setAnimating(false);//true

    mainLayout3->addWidget(diss);

   // ui->frmdiss->setLayout(mainLayout1);
    ui->groupBoxdiss->setLayout(mainLayout3);
    //diss->setVisible(true);
    diss->initVariables("分闸","O","合闸");
    value=30;
    diss->setValue(value);
   //-------------------------------------------------------

    mainLayout4=new QVBoxLayout();
    mainLayout4->setSpacing(0);
    mainLayout4->setContentsMargins(0,0,0,40);
    ears=new myGauge2(ui->groupBoxears);//     frmears);//test);
    ears->setAnimating(false);//true

    mainLayout4->addWidget(ears);

   // ui->frmears->setLayout(mainLayout1);
    ui->groupBoxears->setLayout(mainLayout4);
    //ears->setVisible(true);
    ears->initVariables("分闸","O","合闸");
    value=50;
    ears->setValue(value);

   //room2------------------------------------------
    mainLayout21=new QVBoxLayout();
    mainLayout21->setSpacing(0);
    mainLayout21->setContentsMargins(0,2,0,40);
    ui->groupBoxsel21->setTitle("远方217就地");

    sel21=new myGauge2(ui->groupBoxsel21);//     frmsels);//test);
    sel21->setAnimating(false);//true

    mainLayout21->addWidget(sel21);

   // ui->frmsels->setLayout(mainLayout1);
    ui->groupBoxsel21->setLayout(mainLayout21);
    //sels->setVisible(true);
    sel21->initVariables("就地","","远方");
    value=30;
    sel21->setValue(value);

    mainLayout22=new QVBoxLayout();
    mainLayout22->setSpacing(0);
    mainLayout22->setContentsMargins(0,2,0,40);
    ui->groupBoxsel21->setTitle("远方218就地");
    sel22=new myGauge2(ui->groupBoxsel22);//     frmsels);//test);
    sel22->setAnimating(false);//true

    mainLayout22->addWidget(sel22);

   // ui->frmsels->setLayout(mainLayout1);
    ui->groupBoxsel22->setLayout(mainLayout22);
    //sels->setVisible(true);
    sel22->initVariables("就地","","远方");
    value=30;
    sel22->setValue(value);

    mainLayout23=new QVBoxLayout();
    mainLayout23->setSpacing(0);
    mainLayout23->setContentsMargins(0,2,0,40);
    ui->groupBoxsel23->setTitle("远方219就地");
    sel23=new myGauge2(ui->groupBoxsel23);//     frmsels);//test);
    sel23->setAnimating(false);//true

    mainLayout23->addWidget(sel23);

   // ui->frmsels->setLayout(mainLayout1);
    ui->groupBoxsel23->setLayout(mainLayout23);
    //sels->setVisible(true);
    sel23->initVariables("就地","","远方");
    value=30;
    sel23->setValue(value);

    mainLayout24=new QVBoxLayout();
    mainLayout24->setSpacing(0);
    mainLayout24->setContentsMargins(0,2,0,40);
    ui->groupBoxsel21->setTitle("远方220就地");
    sel24=new myGauge2(ui->groupBoxsel24);//     frmsels);//test);
    sel24->setAnimating(false);//true

    mainLayout24->addWidget(sel24);

   // ui->frmsels->setLayout(mainLayout1);
    ui->groupBoxsel24->setLayout(mainLayout24);
    //sels->setVisible(true);
    sel24->initVariables("就地","","远方");
    value=30;
    sel24->setValue(value);

    mainLayout25=new QVBoxLayout();
    mainLayout25->setSpacing(0);
    mainLayout25->setContentsMargins(0,2,0,40);
    ui->groupBoxsel25->setTitle("217");

    sel25=new myGauge2(ui->groupBoxsel25);//     frmsels);//test);
    sel25->setAnimating(false);//true

    mainLayout25->addWidget(sel25);

   // ui->frmsels->setLayout(mainLayout1);
    ui->groupBoxsel25->setLayout(mainLayout25);
    //sels->setVisible(true);
    sel25->initVariables("分","0","合");
    value=30;
    sel25->setValue(value);

    mainLayout26=new QVBoxLayout();
    mainLayout26->setSpacing(0);
    mainLayout26->setContentsMargins(0,2,0,40);
    ui->groupBoxsel26->setTitle("218");
    sel26=new myGauge2(ui->groupBoxsel26);//     frmsels);//test);
    sel26->setAnimating(false);//true

    mainLayout26->addWidget(sel26);

   // ui->frmsels->setLayout(mainLayout1);
    ui->groupBoxsel26->setLayout(mainLayout26);
    //sels->setVisible(true);
    sel26->initVariables("分","0","合");
    value=30;
    sel26->setValue(value);

    mainLayout27=new QVBoxLayout();
    mainLayout27->setSpacing(0);
    mainLayout27->setContentsMargins(0,2,0,40);
    ui->groupBoxsel27->setTitle("219");
    sel27=new myGauge2(ui->groupBoxsel27);//     frmsels);//test);
    sel27->setAnimating(false);//true

    mainLayout27->addWidget(sel27);

   // ui->frmsels->setLayout(mainLayout1);
    ui->groupBoxsel27->setLayout(mainLayout27);
    //sels->setVisible(true);
    sel27->initVariables("分","0","合");
    value=30;
    sel27->setValue(value);

    mainLayout28=new QVBoxLayout();
    mainLayout28->setSpacing(0);
    mainLayout28->setContentsMargins(0,2,0,40);
    ui->groupBoxsel28->setTitle("220");
    sel28=new myGauge2(ui->groupBoxsel28);//     frmsels);//test);
    sel28->setAnimating(false);//true

    mainLayout28->addWidget(sel28);

   // ui->frmsels->setLayout(mainLayout1);
    ui->groupBoxsel28->setLayout(mainLayout28);
    //sels->setVisible(true);
    sel28->initVariables("分","0","合");
    value=30;
    sel28->setValue(value);
    //-----------------------------------------------------
    //测试
    mainLayout21t=new QVBoxLayout();
    mainLayout21t->setSpacing(0);
    mainLayout21t->setContentsMargins(0,2,0,40);
    //ui->groupBox21->setTitle("");

    xn21=new myGauge2(ui->groupBox21);//     frmsels);//test);
    xn21->setAnimating(false);//true

    mainLayout21t->addWidget(xn21);

   // ui->frmsels->setLayout(mainLayout1);
    ui->groupBox21->setLayout(mainLayout21t);
    //sels->setVisible(true);
    xn21->initVariables("分闸","0","合闸");
    value=30;
    xn21->setValue(value);

    mainLayout22t=new QVBoxLayout();
    mainLayout22t->setSpacing(0);
    mainLayout22t->setContentsMargins(0,2,0,40);
    //ui->groupBox21->setTitle("远方218就地");
    xn22=new myGauge2(ui->groupBox22);//     frmsels);//test);
    xn22->setAnimating(false);//true

    mainLayout22t->addWidget(xn22);

   // ui->frmsels->setLayout(mainLayout1);
    ui->groupBox22->setLayout(mainLayout22t);
    //sels->setVisible(true);
    xn22->initVariables("左一","中间","右一");
    value=30;
    xn22->setValue(value);

    mainLayout23t=new QVBoxLayout();
    mainLayout23t->setSpacing(0);
    mainLayout23t->setContentsMargins(0,2,0,40);
    //ui->groupBox23->setTitle("远方219就地");
    xn23=new myGauge2(ui->groupBox23);//     frmsels);//test);
    xn23->setAnimating(false);//true

    mainLayout23t->addWidget(xn23);

   // ui->frmsels->setLayout(mainLayout1);
    ui->groupBox23->setLayout(mainLayout23t);
    //sels->setVisible(true);
    xn23->initVariables("分闸","0","合闸");
    value=30;
    xn23->setValue(value);

    mainLayout24t=new QVBoxLayout();
    mainLayout24t->setSpacing(0);
    mainLayout24t->setContentsMargins(0,2,0,40);
    //ui->groupBoxsel21->setTitle("远方220就地");
    xn24=new myGauge2(ui->groupBox24);//     frmsels);//test);
    xn24->setAnimating(false);//true

    mainLayout24t->addWidget(xn24);

   // ui->frmsels->setLayout(mainLayout1);
    ui->groupBox24->setLayout(mainLayout24t);
    //sels->setVisible(true);
    xn24->initVariables("分闸","0","合闸");
    value=30;
    xn24->setValue(value);

    mainLayout25t=new QVBoxLayout();
    mainLayout25t->setSpacing(0);
    mainLayout25t->setContentsMargins(0,2,0,40);
    //ui->groupBox25->setTitle("217");

    xn25=new myGauge2(ui->groupBox25);//     frmsels);//test);
    xn25->setAnimating(false);//true

    mainLayout25t->addWidget(xn25);

   // ui->frmsels->setLayout(mainLayout1);
    ui->groupBox25->setLayout(mainLayout25t);
    //sels->setVisible(true);
    xn25->initVariables("分闸","0","合闸");
    value=30;
    xn25->setValue(value);

    mainLayout26t=new QVBoxLayout();
    mainLayout26t->setSpacing(0);
    mainLayout26t->setContentsMargins(0,2,0,40);
    //ui->groupBox26->setTitle("218");
    xn26=new myGauge2(ui->groupBox31);//     frmsels);//test);
    xn26->setAnimating(false);//true

    mainLayout26t->addWidget(xn26);

   // ui->frmsels->setLayout(mainLayout1);
    ui->groupBox31->setLayout(mainLayout26t);
    //sels->setVisible(true);
    xn26->initVariables("分闸","0","合闸");
    value=30;
    xn26->setValue(value);

    mainLayout27t=new QVBoxLayout();
    mainLayout27t->setSpacing(0);
    mainLayout27t->setContentsMargins(0,2,0,40);
    //ui->groupBox32->setTitle("219");
    xn27=new myGauge2(ui->groupBox32);//     frmsels);//test);
    xn27->setAnimating(false);//true

    mainLayout27t->addWidget(xn27);

   // ui->frmsels->setLayout(mainLayout1);
    ui->groupBox32->setLayout(mainLayout27t);
    //sels->setVisible(true);
    xn27->initVariables("左一","中间","右一");
    value=30;
    xn27->setValue(value);

    mainLayout28t=new QVBoxLayout();
    mainLayout28t->setSpacing(0);
    mainLayout28t->setContentsMargins(0,2,0,40);
    //ui->groupBox33->setTitle("220");
    xn28=new myGauge2(ui->groupBox33);//     frmsels);//test);
    xn28->setAnimating(false);//true

    mainLayout28t->addWidget(xn28);

   // ui->frmsels->setLayout(mainLayout1);
    ui->groupBox33->setLayout(mainLayout28t);
    //sels->setVisible(true);
    xn28->initVariables("分闸","0","合闸");
    value=30;
    xn28->setValue(value);
    //---------------------------------------------------------



    //drawlamp(int x,int y,int radius,float scale,int col)
    //drawlamp(100,400,100,0.2,1);
    ui->pushButton_18->hide();
    ui->pushButton_19->hide();

    ui->tabWidget->setCurrentIndex(0);

    timerstu = new QTimer(this);
    connect(timerstu,SIGNAL(timeout()),this,SLOT(dispstu()));
    timerstu->start(1000);

    //video
    //ca=new Camera(640, 480, 320, 240, 30);
    ca=new Camera(320,240,320, 240, 30);//320, 240,
    //std::cout << "camera init failed\n" << std::endl;
    //ca=new Camera(320, 240, 320, 240, 30);
    //connect(ca, SIGNAL(framesReady()), this,SLOT(update()));
    ca->start();

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

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    timer->start(30);

    //connect(ca, SIGNAL(Camera::framesReady()), this,SLOT(update()));
    //connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(take_a_photo()));
    connect(&session,SIGNAL(AddDestIp(unsigned int,unsigned short)),\
            this,SLOT(AddDestIPAction(unsigned int,unsigned short)));
    connect(&session,SIGNAL(DeleteDestIp(unsigned int,unsigned short)),\
            this,SLOT(DeleteDestIpAction(unsigned int,unsigned short)));

    udpBroadCastInit();
    udpTimer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(SendBroadCast()));
    udpTimer->start(500);
    //video end

    update();
}



void DialogManual::drawlamp(int x,int y,int radius,float scale,int col)
{
  QPainter painter(ui->frame_2); //
//painter.setRenderHint(QPainter::Antialiasing); 	// 使用反锯齿(如果可用)
// painter.translate(60+ui->widget->width() / 2,70+ui->widget->height() / 2);	// 坐标变换为窗体中心
painter.translate(x,y); // 坐标变换为窗体中心
//int side = 50;   // qMin(ui->widget->width(), ui->widget->height());
painter.scale(scale, scale);		// 比例缩放
painter.setPen(Qt::NoPen);

//int radius =100;
//外边框
QLinearGradient lg1(0, -radius, 0, radius);
lg1.setColorAt(0, QColor(255, 255, 255));
lg1.setColorAt(1, QColor(166, 166, 166));
painter.setBrush(lg1);
painter.drawEllipse(-radius, -radius, radius << 1, radius << 1);

// 内边框
radius -= 13;
QLinearGradient lg2(0, -radius, 0, radius);
lg2.setColorAt(0, QColor(166, 166, 166));
lg2.setColorAt(1, QColor(255, 255, 255));
painter.setBrush(lg2);
painter.drawEllipse(-radius, -radius, radius << 1, radius << 1);

// 内部指示颜色
radius -= 4;
QRadialGradient rg(0, 0, radius);
if (col==1)//red
{
    rg.setColorAt(0, QColor(245, 0, 0));
    rg.setColorAt(0.6, QColor(210, 0, 0));
    rg.setColorAt(1, QColor(166, 0, 0));
}
else if(col==2)//green
{
    rg.setColorAt(0, QColor(0, 245, 0));
    rg.setColorAt(0.6, QColor(0, 210, 0));
    rg.setColorAt(1, QColor(0, 166, 0));
}
else if(col==3)//yellow
{
    rg.setColorAt(0, QColor(245, 245, 0));
    rg.setColorAt(0.6, QColor(210, 210, 0));
    rg.setColorAt(1, QColor(166, 166, 0));
}
/*****************
else if(col==4)//blue
{
    rg.setColorAt(0, QColor(0,245, 0));
    rg.setColorAt(0.6, QColor(0, 210, 0));
    rg.setColorAt(1, QColor(0, 0,66));
}
************************/

if(col!=0)painter.setBrush(rg);
painter.drawEllipse(-radius, -radius, radius << 1, radius << 1);
}

void DialogManual::dispstu()//disp
{
  //测试
  if(1) //gchg3)//pgno==3&&
  {
      QString equidx="00";
      //if(runmodel!=0)return;
      for(int i=0;i<nkk;i++)
       if(kkparaset[i].id==equidx)
       {
           switch(kkparaset[i].pos)
           {
           case 3:switch(kkparaset[i].value)
                  {
                    case -1:xn21->setValue(30);break;
                    case 1:xn21->setValue(70);break;
                  }
                  break;
           case 4:switch(kkparaset[i].value)
                  {
                   case -1:xn22->setValue(30);break;
                   case 0:xn22->setValue(50);break;
                   case 1:xn22->setValue(70);break;
                  }
                  break;
           case 5:switch(kkparaset[i].value)
                  {
                   case -1:xn23->setValue(30);break;
                   case 1:xn23->setValue(70);break;
                  }
                   break;
           case 6:switch(kkparaset[i].value)
                  {
                    case -1:xn24->setValue(30);break;
                    case 1:xn24->setValue(70);break;
                   }
                   break;
           case 7:switch(kkparaset[i].value)
                  {
                    case -1:xn25->setValue(30);break;
                    case 1:xn25->setValue(70);break;
                  }
                  break;
           case 12:switch(kkparaset[i].value)
                  {
                   case -1:xn26->setValue(30);break;
                   case 1:xn26->setValue(70);break;
                  }
                  break;
           case 13:switch(kkparaset[i].value)
                  {
                   case -1:xn27->setValue(30);break;
                   case 0:xn27->setValue(50);break;
                   case 1:xn27->setValue(70);break;
                  }
                   break;
           case 14:switch(kkparaset[i].value)
                  {
                    case -1:xn28->setValue(30);break;
                    case 1:xn28->setValue(70);break;
                   }
                   break;

           }
         }
         gchg3=false;
         gpicchg3=true;
         update();
  }

  //室1
  if(gchg1)//&&pgno==1)//柜或开关发生变化时
  {
      int idx=equid2index(gequid);
      ui->labelno->setText(equparaset[idx].no);
      ui->labelname->setText(equparaset[idx].title);
      if(equparaset[idx].roomno==1)
      {
          for(int i=0;i<nkk;i++)
           if(kkparaset[i].id==gequid)
           {
               switch(kkparaset[i].pos)
               {
                 case 1:switch(kkparaset[i].value)
                        {
                          case -1:sels->setValue(30);break;
                          case 1:sels->setValue(70);break;
                          case 0:sels->setValue(50);break;
                        }
                        break;
                 case 2:switch(kkparaset[i].value)
                        {
                         case -1:bres->setValue(30);break;
                         case 1:bres->setValue(70);break;
                        }
                        break;
                 case 3:switch(kkparaset[i].value)
                        {
                         case -1:diss->setValue(30);break;
                         case 1:diss->setValue(70);break;
                        }
                         break;
                 case 4:switch(kkparaset[i].value)
                        {
                          case -1:ears->setValue(30);break;
                          case 1:ears->setValue(70);break;
                         }
                         break;
                }

            }

            gchg1=false;
            gpicchg1=true;
            update();
         }
      }//if gchg1



      //update();
}

DialogManual::~DialogManual()
{
    timerstu->stop();
    RTPTime delay(10.0);
    //rs = vd->stop_capturing();
    //rs = vd->uninit_device();
    //rs = vd->close_device();

    session.BYEDestroy(delay,"Time's up",9);
    std::cout << "close" << std::endl;
    //
    udpTimer->stop();
    timer->stop();
    camera_running=false;//视频采集;
    ca->quit();
    ca->wait();
    close();

    delete ui;
}

void DialogManual::save_mjpeg()
{
     QDateTime current_date_time = QDateTime::currentDateTime();
     QString name("/home/hfut/CameraRead/");
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
void DialogManual::take_a_photo()
{
    takeflag = 1;
}
void DialogManual::AddDestIPAction(unsigned int ip,unsigned short port)
{
    struct in_addr inaddr;
    inaddr.s_addr = htonl(ip);
    QString IP(inet_ntoa(inaddr));
    QString str = IP + ":" + QString::number(port);
    ipnum++;
    //ComboBox->addItem(str);
}
void DialogManual::udpBroadCastInit(){
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

void DialogManual::DeleteDestIpAction(unsigned int ip,unsigned short port)
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

void DialogManual::paintEvent(QPaintEvent *)
{
    QImage image;
    QPixmap pix;
    QString pic1pon,pic2pon,pic3pon,pic1poff,pic2poff,pic3poff;
    QString picgbton,picgbtoff,picrbton,picrbtoff;
    QString strdir;
    pic1pon="1pon.png";
    pic2pon="2pon.png";
    pic3pon="3pon.png";
    pic1poff="1poff.png";
    pic2poff="2poff.png";
    pic3poff="3poff.png";
    picgbton="gon.png";
    picgbtoff="goff.png";
    picrbton="ron.jpg";
    picrbtoff="roff.png";

    strdir.sprintf("%s/",dirpf);
    pic1pon=strdir+pic1pon;
    pic2pon=strdir+pic2pon;
    pic3pon=strdir+pic3pon;
    pic1poff=strdir+pic1poff;
    pic2poff=strdir+pic2poff;
    pic3poff=strdir+pic3poff;
    picgbton=strdir+picgbton;
    picgbtoff=strdir+picgbtoff;
    picrbton=strdir+picrbton;
    picrbtoff=strdir+picrbtoff;

    if(gpicchg1)//柜或开关发生变化时
    {

        int idx=equid2index(gequid);
        if(equparaset[idx].roomno==1)
        {           
            //qDebug()<<"room1 kongkai！";
            if(equparaset[idx].type==2)
            {
                ui->labelzk2->hide();
                ui->labelkk2name->hide();
            }
            else
            {
                ui->labelzk2->show();
                ui->labelkk2name->show();
            }

            for(int i=0;i<nkk;i++)
            {
             if(kkparaset[i].id==gequid)
             {
                 switch(kkparaset[i].pos)
                 {
                   case 5:switch(kkparaset[i].value)
                          {
                            case 0:if(equparaset[idx].type==3)image.load(pic1poff);
                                   else image.load(pic2poff);
                                   ui->labelkk1name->setText(kkparaset[i].name);
                                   break;
                            case 1:if(equparaset[idx].type==3)image.load(pic1pon);
                                   else image.load(pic2pon);
                                   ui->labelkk1name->setText(kkparaset[i].name);
                                   break;
                          }
                          break;
                   case 6:switch(kkparaset[i].value)
                          {
                           case 0:image.load(pic1poff);
                                  ui->labelkk2name->setText(kkparaset[i].name);
                                  break;
                           case 1:image.load(pic1pon);
                                  ui->labelkk2name->setText(kkparaset[i].name);
                                  break;
                          }
                          break;
                   case 7:switch(kkparaset[i].value)
                          {
                           case 0:image.load(pic1poff);
                                  ui->labelkk3name->setText(kkparaset[i].name);
                                  break;
                           case 1:image.load(pic1pon);
                                  ui->labelkk3name->setText(kkparaset[i].name);
                                  break;
                          }
                          break;
                  }

                 //************
                 //QPainter painter(this);
                   pix = QPixmap::fromImage(image);//.scaled(size(), Qt::KeepAspectRatio) );
               //  ui->label->resize(651,621);//**********************************************************8
                 if(kkparaset[i].pos==5)
                 {
                  ui->labelzk1->setPixmap(pix );
                  ui->labelzk1->setScaledContents(true);
                 }
                 if(kkparaset[i].pos==6)
                 {
                  ui->labelzk2->setPixmap(pix );
                  ui->labelzk2->setScaledContents(true);
                 }
                 if(kkparaset[i].pos==7)
                 {
                  ui->labelzk3->setPixmap(pix );
                  ui->labelzk3->setScaledContents(true);
                 }
              }

            }//endfor
        }//room==1
        gpicchg1=false;
      } //

    if(1) //gpicchg3)//测试柜或开关发生变化时
    {
        QString qid="00";
        int idx=equid2index(qid);
        if(equparaset[idx].roomno==3)
        {
           // qDebug()<<"room3 kongkai！";

            for(int i=0;i<nkk;i++)
            {
             if(kkparaset[i].id==qid)
             {

                 switch(kkparaset[i].pos)
                 {
                   case 1:switch(kkparaset[i].value)
                          {
                            case 0:image.load(pic2poff);
                                   break;
                            case 1:image.load(pic2pon);
                                   break;
                          }
                          break;
                   case 2:switch(kkparaset[i].value)
                          {
                           case 0:image.load(pic3poff);
                                  break;
                           case 1:image.load(pic3pon);
                                  break;
                          }
                          break;
                   case 8:switch(kkparaset[i].value)
                          {
                           case 0:image.load(picgbtoff);
                                  break;
                           case 1:image.load(picgbton);
                                  break;
                          }
                          break;
                    case 9:switch(kkparaset[i].value)
                        {
                         case 0:image.load(picgbtoff);
                                break;
                         case 1:image.load(picgbton);
                                break;
                        }
                        break;
                    case 10:switch(kkparaset[i].value)
                        {
                         case 0:image.load(picrbtoff);
                                break;
                         case 1:image.load(picrbton);
                                break;
                        }
                        break;
                    case 11:switch(kkparaset[i].value)
                        {
                         case 0:image.load(picrbtoff);
                                break;
                         case 1:image.load(picrbton);
                                break;
                        }
                        break;
                    case 15:switch(kkparaset[i].value)
                        {
                          case 0:image.load(pic2poff);
                                 break;
                          case 1:image.load(pic2pon);
                                 break;
                        }
                        break;
                  }

                 //************
                 //QPainter painter(this);
                   pix = QPixmap::fromImage(image);//.scaled(size(), Qt::KeepAspectRatio) );
               //  ui->label->resize(651,621);//**********************************************************8
                 if(kkparaset[i].pos==1)
                 {
                  ui->labelzk2p->setPixmap(pix );
                  ui->labelzk2p->setScaledContents(true);
                 }
                 if(kkparaset[i].pos==2)
                 {
                  ui->labelzk3p->setPixmap(pix );
                  ui->labelzk3p->setScaledContents(true);
                 }
                 if(kkparaset[i].pos==8)
                 {
                  ui->labelbtn1->setPixmap(pix );
                  ui->labelbtn1->setScaledContents(true);
                 }
                 if(kkparaset[i].pos==9)
                 {
                  ui->labelbtn2->setPixmap(pix );
                  ui->labelbtn2->setScaledContents(true);
                 }
                 if(kkparaset[i].pos==10)
                 {
                  ui->labelbtn3->setPixmap(pix );
                  ui->labelbtn3->setScaledContents(true);
                 }
                 if(kkparaset[i].pos==11)
                 {
                  ui->labelbtn4->setPixmap(pix );
                  ui->labelbtn4->setScaledContents(true);
                 }
                 if(kkparaset[i].pos==15)
                 {
                  ui->labelzk2p_1->setPixmap(pix );
                  ui->labelzk2p_1->setScaledContents(true);
                 }

              }

            }//endfor
        }//room==3
        gpicchg3=false;
      } //

    //video
    if(camera_running)//
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
        ui->labelvideo->setPixmap(QPixmap::fromImage(ca->qimg));
        ui->labelvideo->setScaledContents(true);
        //disp video*******************
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
    }


}

void DialogManual::display_error(QString err)
{
    QMessageBox::warning(this,tr("error"), err,QMessageBox::Yes);
}

/*yuv格式转换为rgb格式*/
int DialogManual::convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height)
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

int DialogManual::convert_yuv_to_rgb_pixel(int y, int u, int v)
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

QImage DialogManual::cvMat2QImage(const cv::Mat& mat)
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
cv::Mat DialogManual::QImage2cvMat(QImage image)
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


QImage DialogManual::detectAndDisplay(cv::Mat frame)
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


void DialogManual::on_pushButton_clicked()
{
  gequid="01";
  gchg1=true;
}

void DialogManual::on_pushButtonman_clicked()
{
  int idx=kkposandequid2index(gequid,1);
  kkopt(kkparaset[idx].no,-1);

  gchg1=true;
}

void DialogManual::on_pushButton_18_clicked()
{
    /****************
    QSqlQuery query;
    bool value;
    int idx;
    QString qx,qy,qz,qrx,qry,qrz,qsl,q0;
    q0="00";
    Jkstatus.cartesiantran_position[0]=11.11;
    qx.sprintf("%f",Jkstatus.cartesiantran_position[0]);
    qy.sprintf("%f",Jkstatus.cartesiantran_position[1]);
    qz.sprintf("%f",Jkstatus.cartesiantran_position[2]);
    qrx.sprintf("%f",Jkstatus.cartesiantran_position[3]);
    qry.sprintf("%f",Jkstatus.cartesiantran_position[4]);
    qrz.sprintf("%f",Jkstatus.cartesiantran_position[5]);

    idx=equid2index(q0);
    equparaset[idx].x=Jkstatus.cartesiantran_position[0];
    equparaset[idx].y=Jkstatus.cartesiantran_position[1];
    equparaset[idx].z=Jkstatus.cartesiantran_position[2];
    equparaset[idx].rx=Jkstatus.cartesiantran_position[3];
    equparaset[idx].ry=Jkstatus.cartesiantran_position[4];
    equparaset[idx].rz=Jkstatus.cartesiantran_position[5];

    //qsl.sprintf("%f",Jkstatus.cartesiantran_position[0]);
    //QMessageBox::information(this,"提示",qsl);
    //qsl="update equpara set x="+qx+",y="+qy+",z="+qz+",rx="+qrx+",ry="+qry+",rz="+qrz+" where id="+q0;
    //QMessageBox::information(this,"提示",qsl);
    qsl="update equpara set x='" + qx + "',y='" + qy + "',z='" + qz + "',rx='" + qrx+ "',ry='" + qry+ "',rz='" + qrz+ "' where id='" + q0 + "'";
   value=query.exec(qsl);//"update equpara set x="+qx+",y="+qy+",z="+qz+",rx="+qrx+",ry="+qry+",rz="+qrz+" where id="+q0);
   if(value)
   {
       QMessageBox::information(this,"提示","JAKA初始坐标修改成功！");

      // this->close();
   }
   else
   {
       QMessageBox::information(this,"提示","JAKA初始坐标修改失败！");
      // this->close();
   }
   ***********************/
}

void DialogManual::on_pushButton_19_clicked()
{

/********************
    QString q0;
    int idx;
    CartesianPose cart;
    q0="00";
    //jk2initpos(q0);

    if(Jkstatus.is_socket_connect!=1)
    {
        QMessageBox::information(this,"提示","JAKA控制器连接失败，请检查！");
        return;
    }
    idx=equid2index(q0);
    cart.tran.x =equparaset[idx].x;
    cart.tran.y =equparaset[idx].y;
    cart.tran.z =equparaset[idx].z;
    cart.rpy.rx =equparaset[idx].rx;
    cart.rpy.ry =equparaset[idx].ry;
    cart.rpy.rz =equparaset[idx].rz;
    //笛卡尔空间运动,其中 ABS 代表绝对运动,FALSE 代表指令是非阻塞的,10 代表速度为 10mm/s
    //printf("rx=%f , ry=%f, rz=%f\n", cart.rpy.rx, cart.rpy.ry, cart.rpy.rz);
    JKrobot.linear_move(&cart, ABS, TRUE, 10);
    //打印笛卡尔空间位置

   // printf("errcode=%d \nx=%f, y=%f, z=%f\n", status.errcode,status.cartesiantran_position
   // [0], status.cartesiantran_position[1], status.cartesiantran_position[2]);
  
   // printf("rx=%f, ry=%f, rz=%f",status.cartesiantran_position[3], status.cartesiantran_po
  //  sition[4], status.cartesiantran_position[5]);

  *********************/
}

void DialogManual::on_pushButton2pon_clicked()
{
    QString qno="001";
    int qst=1;
    kkopt(qno,qst);
    gchg3=true;
}

void DialogManual::on_pushButton2poff_clicked()
{
    QString qno="001";
    int qst=0;
    kkopt(qno,qst);
    gchg3=true;
}

void DialogManual::on_pushButton3pon_clicked()
{
    QString qno="002";
    int qst=1;
    kkopt(qno,qst);
    gchg3=true;
}

void DialogManual::kkopt(QString no,int st)//开关编号，到达状态
{
    int kkidx;
    QSqlQuery query;
    bool value;
    double offsetx,offsety,offsetz,offsetrx,offsetry,offsetrz;
    QString qsl,qst;
    gno=no;
    gstate=st;
    kkidx=kkno2index(no);
    if(kkparaset[kkidx].value==st)
    {
        QMessageBox::information(this,"提示",kkparaset[kkidx].name+"已达到状态，请检查！");
        return;
    }

    emit Sequid(kkparaset[kkidx].id);//send to mainwindow


    //修改状态及记录,以后删除，该在Process线程中开关操作完成时修改
    /****************
    kkparaset[kkidx].value=st;
    qst=QString::number(st);
    //QMessageBox::information(this,"提示",QString::number(kkparaset[kkidx].value));
    qsl="update kkpara set value='" + qst + "' where no='" + no + "'";
    value=query.exec(qsl);//"update equpara set x="+qx+",y="+qy+",z="+qz+",rx="+qrx+",ry="+qry+",rz="+qrz+" where id="+q0);
    if(value) QMessageBox::information(this,"提示","状态修改成功！以后需要改到Process线程中...");
    else QMessageBox::information(this,"提示","状态修改失败！");
    ***************/
}


void DialogManual::on_pushButton3poff_clicked()
{
    QString qno="002";
    int qst=0;
    kkopt(qno,qst);
    gchg3=true;
}

void DialogManual::on_pushButtonon21_clicked()
{
    QString qno="003";
    int qst=-1;
    kkopt(qno,qst);
    gchg3=true;
}

void DialogManual::on_pushButtonoff21_clicked()
{
    QString qno="003";
    int qst=1;
    kkopt(qno,qst);
    gchg3=true;
}

void DialogManual::on_pushButtonon22_clicked()
{
    QString qno="004";
    int qst=1;
    kkopt(qno,qst);
    gchg3=true;
}

void DialogManual::on_pushButtonoff22_clicked()
{
    QString qno="004";
    int qst=-1;
    kkopt(qno,qst);
    gchg3=true;
}

void DialogManual::on_pushButton22min_clicked()
{
    QString qno="004";
    int qst=0;
    kkopt(qno,qst);
    gchg3=true;
}

void DialogManual::on_pushButtonon23_clicked()
{
    QString qno="005";
    int qst=-1;
    kkopt(qno,qst);
    gchg3=true;
}

void DialogManual::on_pushButtonoff23_clicked()
{
    QString qno="005";
    int qst=1;
    kkopt(qno,qst);
    gchg3=true;
}

void DialogManual::on_pushButtonon24_clicked()
{
    QString qno="006";
    int qst=-1;
    kkopt(qno,qst);
    gchg3=true;
}

void DialogManual::on_pushButtonoff24_clicked()
{
    QString qno="006";
    int qst=1;
    kkopt(qno,qst);
    gchg3=true;
}

void DialogManual::on_pushButtonon25_clicked()
{
    QString qno="007";
    int qst=-1;
    kkopt(qno,qst);
    gchg3=true;
}

void DialogManual::on_pushButtonoff25_clicked()
{
    QString qno="007";
    int qst=1;
    kkopt(qno,qst);
    gchg3=true;
}

void DialogManual::on_pushButton1on_clicked()
{
    QString qno="0011";
    int qst=1;
    kkopt(qno,qst);
    gchg3=true;
}

void DialogManual::on_pushButton1off_clicked()
{
    QString qno="0011";
    int qst=0;
    kkopt(qno,qst);
    gchg3=true;
}

void DialogManual::on_pushButton2on_clicked()
{
    QString qno="0012";
    int qst=1;
    kkopt(qno,qst);
    gchg3=true;
    //int jk=kkno2index(qno);
    //QMessageBox::information(this,"提示","进入012！kkidx"+QString::number(jk));
}

void DialogManual::on_pushButton2off_clicked()
{
    QString qno="0012";
    int qst=0;
    kkopt(qno,qst);
    gchg3=true;
}

void DialogManual::on_pushButton3on_clicked()
{
    QString qno="0013";
    int qst=1;
    kkopt(qno,qst);
   // int jk=kkno2index(qno);
    gchg3=true;
    //QMessageBox::information(this,"提示","进入013！kkidx"+QString::number(jk));


}

void DialogManual::on_pushButton3off_clicked()
{
    QString qno="0013";
    int qst=0;
    kkopt(qno,qst);
    gchg3=true;
}

void DialogManual::on_pushButton4on_clicked()
{
    QString qno="0014";
    int qst=1;
    kkopt(qno,qst);
    gchg3=true;
}

void DialogManual::on_pushButton4off_clicked()
{
    QString qno="0014";
    int qst=0;
    kkopt(qno,qst);
    gchg3=true;
}

void DialogManual::on_pushButton31on_clicked()
{
    QString qno="008";
    int qst=-1;
    kkopt(qno,qst);
    gchg3=true;
}

void DialogManual::on_pushButton31off_clicked()
{
    QString qno="008";
    int qst=1;
    kkopt(qno,qst);
    gchg3=true;
}

void DialogManual::on_pushButton32on_clicked()
{
    QString qno="009";
    int qst=-1;
    kkopt(qno,qst);
    gchg3=true;
}

void DialogManual::on_pushButton32off_clicked()
{
    QString qno="009";
    int qst=1;
    kkopt(qno,qst);
    gchg3=true;
}

void DialogManual::on_pushButton32min_clicked()
{
    QString qno="009";
    int qst=0;
    kkopt(qno,qst);
    gchg3=true;
}

void DialogManual::on_pushButton33on_clicked()
{
    QString qno="0010";
    int qst=-1;
    kkopt(qno,qst);
    gchg3=true;
}

void DialogManual::on_pushButton33off_clicked()
{
    QString qno="0010";
    int qst=1;
    kkopt(qno,qst);
    gchg3=true;
}

void DialogManual::on_pushButton_2_clicked()
{
    gequid="02";
    gchg1=true;
}

void DialogManual::on_pushButton_3_clicked()
{
    gequid="03";
    gchg1=true;
}

void DialogManual::on_pushButton_4_clicked()
{
    gequid="04";
    gchg1=true;
}

void DialogManual::on_pushButton_5_clicked()
{
    gequid="05";
    gchg1=true;
}

void DialogManual::on_pushButton_6_clicked()
{
    gequid="06";
    gchg1=true;
}

void DialogManual::on_pushButton_7_clicked()
{
    gequid="07";
    gchg1=true;
}

void DialogManual::on_pushButton_8_clicked()
{
    gequid="08";
    gchg1=true;
}

void DialogManual::on_pushButton_9_clicked()
{
    gequid="09";
    gchg1=true;
}

void DialogManual::on_pushButtonloc_clicked()
{
    int idx=kkposandequid2index(gequid,1);
    kkopt(kkparaset[idx].no,0);

    gchg1=true;
}

void DialogManual::on_pushButtonfar_clicked()
{
    int idx=kkposandequid2index(gequid,1);
    kkopt(kkparaset[idx].no,1);

    gchg1=true;
}

void DialogManual::on_pushButtonzk1off_clicked()
{
    int idx=kkposandequid2index(gequid,5);
    kkopt(kkparaset[idx].no,0);

    gchg1=true;
}

void DialogManual::on_pushButtonzk1on_clicked()
{
    int idx=kkposandequid2index(gequid,5);
    kkopt(kkparaset[idx].no,1);

    gchg1=true;
}

void DialogManual::on_pushButtonbreoff_clicked()
{
    int idx=kkposandequid2index(gequid,2);
    kkopt(kkparaset[idx].no,-1);

    gchg1=true;
}

void DialogManual::on_pushButtonbreon_clicked()
{
    int idx=kkposandequid2index(gequid,2);
    kkopt(kkparaset[idx].no,1);

    gchg1=true;
}

void DialogManual::on_pushButtondissoff_clicked()
{
    int idx=kkposandequid2index(gequid,3);
    kkopt(kkparaset[idx].no,-1);

    gchg1=true;
}

void DialogManual::on_pushButtondisson_clicked()
{
    int idx=kkposandequid2index(gequid,3);
    kkopt(kkparaset[idx].no,1);

    gchg1=true;
}

void DialogManual::on_pushButtonearoff_clicked()
{
    int idx=kkposandequid2index(gequid,4);
    kkopt(kkparaset[idx].no,-1);

    gchg1=true;
}

void DialogManual::on_pushButtonearon_clicked()
{
    int idx=kkposandequid2index(gequid,4);
    kkopt(kkparaset[idx].no,1);

    gchg1=true;
}

void DialogManual::on_pushButtonzk2off_clicked()
{
    int idx=kkposandequid2index(gequid,6);
    kkopt(kkparaset[idx].no,0);

    gchg1=true;
}

void DialogManual::on_pushButtonzk2on_clicked()
{
    int idx=kkposandequid2index(gequid,6);
    kkopt(kkparaset[idx].no,1);

    gchg1=true;
}

void DialogManual::on_pushButtonzk3off_clicked()
{
    int idx=kkposandequid2index(gequid,7);
    kkopt(kkparaset[idx].no,0);

    gchg1=true;
}

void DialogManual::on_pushButtonzk3on_clicked()
{
    int idx=kkposandequid2index(gequid,7);
    kkopt(kkparaset[idx].no,1);

    gchg1=true;
}

void DialogManual::on_pushButton_37_clicked()
{
   // QString q0;
   // q0="00";
   // emit Sequid(q0);
    QJsonObject root_Obj;
    int sec,usec;
    struct timeval us;
    QString topic;
    int idx;
    //q0="00";
    double quat[4],eu[3],coord[3];
    coord[0]=-0.34058359265327453613;//equparaset[idx].shx;
    coord[1]=-0.11496724188327789307;//equparaset[idx].shy;
    coord[2]=-0.49346005916595458984;//equparaset[idx].shz;
    quat[0] =0.02654558356676450098;//equparaset[idx].shx1;
    quat[1] =-0.03758920266706877039;//equparaset[idx].shy1;
    quat[2] =0.03435658208791222273;//equparaset[idx].shz1;//
    quat[3] =0.99834964271200621333;//equparaset[idx].shw1;

    quat2eular(quat,eu);
    eu[0]=0.0;
    eu[1]=0.0;
    eular2quat(eu,quat);


    gettimeofday(&us,NULL);
    sec=us.tv_sec;
    usec=us.tv_usec;
    double timestamp=sec;
    double ff=usec/1000000.0;
    timestamp=sec+ff;
    //qDebug()<<"gettimeofday: "<<ff<<sec<<QString::number(timestamp, 'f', 6) ;


    QJsonObject header;
    header.insert("seq",1);
    header.insert("stamp",timestamp);
    header.insert("frame_id","map");

    QJsonObject position;
    position.insert("x", coord[0]);
    position.insert("y", coord[1]);
    position.insert("z", coord[2]);

    QJsonObject orientation;
    orientation.insert("x", quat[0]);
    orientation.insert("y", quat[1]);
    orientation.insert("z", quat[2]);
    orientation.insert("w", quat[3]);

    QJsonObject pose;
    pose.insert("position", position);
    pose.insert("orientation", orientation);

   // QJsonObject pose1;
   // pose1.insert("pose",pose);


    QJsonObject msg;
    msg.insert("header",header);
    msg.insert("pose",pose);

    topic="/move_base_simple/goal";
    webcmd->pubcmd(topic,msg);



}

void DialogManual::on_tabWidget_currentChanged(int index)
{
    pgno=index;
}


void DialogManual::on_pushButton_38_clicked()
{
    //RobotInitialPosition();
    btnopt=1;
}

void DialogManual::on_pushButton2poff_2_clicked()
{
    QString qno="0015";
    int qst=0;
    kkopt(qno,qst);
    gchg3=true;
}

void DialogManual::on_pushButton2pon_2_clicked()
{
    QString qno="0015";
    int qst=1;
    kkopt(qno,qst);
    gchg3=true;
}

void DialogManual::on_pushButtontest_clicked()
{
    QString qno="001";
    int qst=1;
    gtestauto=true;
    kkopt(qno,qst);
    gchg3=true;
}

void DialogManual::on_pushButtontest_2_clicked()
{
    /************
    int kkidx,equidx;
    QString qsl,qst,equid;
    //gno=""
    //kkidx=kkno2index(gno);
    //equid=kkparaset[kkidx].id;
    equid="00";
    equidx=equid2index(equid);
    gbd=true;//展开标定
    ************/
    if(Jkstatus.is_socket_connect==1)
    {
      //int res=move2calibrationpos(equidx);
        btnopt=2;
    }
}

void DialogManual::on_pushButtontest_3_clicked()
{
    gbd=false;
}

void DialogManual::on_pushButtontest_4_clicked()
{
    //getvideo();
    //process=new ProcessImage();//this
    //process.resize(320,240);
    //process->resize(640,480);
    //process->setWindowFlags(Qt::WindowStaysOnTopHint);
    //process->show();
    //dlgvideo=new DialogVideo();
    //dlgvideo->setWindowFlags(Qt::WindowStaysOnTopHint);
    //dlgvideo->show();

}

int DialogManual::getvideo()
{
    /****************
    rs2::log_to_console(RS2_LOG_SEVERITY_ERROR);
        // Create a simple OpenGL window for rendering:
        videowindow app(1280, 720, "RealSense Capture Example");

        // Declare depth colorizer for pretty visualization of depth data
        rs2::colorizer color_map;
        // Declare rates printer for showing streaming rates of the enabled streams.
        rs2::rates_printer printer;

        // Declare RealSense pipeline, encapsulating the actual device and sensors
        //rs2::pipeline pipe;

        // Start streaming with default recommended configuration
        // The default video configuration contains Depth and Color streams
        // If a device is capable to stream IMU data, both Gyro and Accelerometer are enabled by default
        pipe.start();

        while (app) // Application still alive?
        {
            rs2::frameset data = pipe.wait_for_frames().    // Wait for next set of frames from the camera
                                 apply_filter(printer);//.     // Print each enabled stream frame rate
                                // apply_filter(color_map);   // Find and colorize the depth data

            // The show method, when applied on frameset, break it to frames and upload each frame into a gl textures
            // Each texture is displayed on different viewport according to it's stream unique id
            app.show(data);
        }
        *****************/

}

void DialogManual::on_pushButton_39_clicked()
{
    test();
}

void DialogManual::on_pushButtonvon_clicked()
{
    if(camera_running)
    {
        camera_running=false;
        ui->pushButtonvon->setText("开启");
    }
    else
    {
        camera_running=true;
        ui->pushButtonvon->setText("关闭");
    }

}

void DialogManual::on_btnConfrim_clicked()
{
    QMessageBox* confirmMessage = new QMessageBox(this);  //init
    QMessageBox::StandardButton reply;
    reply = confirmMessage->information(this, "确认或取消", "是否确认", QMessageBox::Ok, QMessageBox::Cancel);
    if (reply == QMessageBox::Ok) {
        std::cout << "你点击了确定" << std::endl;
        confirm();
    }
    else if (reply == QMessageBox::Cancel) {
        std::cout << "你点击了取消" << std::endl;
    }
}

void DialogManual::on_pushButtonok_clicked()
{

}
