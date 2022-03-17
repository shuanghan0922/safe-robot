#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "ComData.h"
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QInputDialog>
#include "dialogpose.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include "dialogequset.h"

#define PI 3.1415926


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);



    setWindowFlags(windowFlags()&~(Qt::WindowMaximizeButtonHint|Qt::WindowCloseButtonHint));//隐藏窗口最大最小取消按钮

    wsclient = new WSClient;
    client_thread = new QThread;
    connect(client_thread,&QThread::finished,wsclient,&QObject::deleteLater);
    wsclient->moveToThread(client_thread);
    connect(wsclient,&WSClient::connection_state,this,&MainWindow::C_connection);
    connect(wsclient,&WSClient::occur_error,this,&MainWindow::C_error);
    connect(wsclient,&WSClient::NewTextMessage,this,&MainWindow::C_recive);
    connect(wsclient,&WSClient::NewBinaryMessage,this,&MainWindow::C_reciveb);//

    connect(this, &MainWindow::C_connect, wsclient, &WSClient::ConnectTo);
    connect(this, &MainWindow::C_disconnect, wsclient, &WSClient::Disconnect);
    connect(this, &MainWindow::C_send, wsclient, &WSClient::sendTextMessage);
    connect(this, &MainWindow::C_sendb, wsclient, &WSClient::sendBinaryMessage);

    client_thread->start();

    connect(webcmd,&Webcmd::websub,this,&MainWindow::web_sub);
    connect(webcmd,&Webcmd::webunsub,this,&MainWindow::web_unsub);
    connect(webcmd,&Webcmd::webadv,this,&MainWindow::web_adv);
    connect(webcmd,&Webcmd::webunadv,this,&MainWindow::web_unadv);
    connect(webcmd,&Webcmd::webpub,this,&MainWindow::web_pub);
    connect(webcmd,&Webcmd::webser,this,&MainWindow::web_ser);

    QSqlQuery query,query1;
    int res;

    bool value=query.exec("select * from syspara");
    if(!value) QMessageBox::information(this,"提示","参数库连接接失败！");

   // query.exec("select * from syspara");
    while(query.next())//指向第一条记录
    {
      // ui->radioButtonauto->setChecked(query.value(0).toBool());
     //  ui->radioButtonmanul->setChecked(!query.value(0).toBool());
       sysparaset.bsl=query.value(1).toBool(); //toInt());
       sysparaset.bpp=query.value(2).toBool(); //   toInt());
       sysparaset.bcy=query.value(3).toBool();//.toInt());
       sysparaset.bur=query.value(4).toBool();//.toInt());
       sysparaset.bsf=query.value(5).toBool();//.toInt());
       sysparaset.bsj=query.value(6).toBool();//.toInt());
       sysparaset.pfno=query.value(7).toString();
       sysparaset.pfname=query.value(8).toString();
       //ui->comboBoxcp->setCurrentText(query.value(9).toString());
      // ui->labelcpname->setText(query.value(10).toString());
       sysparaset.dayok=query.value(11).toInt();
       sysparaset.daybj=query.value(12).toInt();
       sysparaset.dayop=query.value(13).toInt();
       sysparaset.addplc=query.value(14).toString();//ROS IP
      // QMessageBox::information(0,"提示",sysparaset.addplc);
       sysparaset.portplc=query.value(15).toInt();//ROS port
       sysparaset.addsecs=query.value(16).toString();
       sysparaset.portsecs=query.value(17).toInt();
       sysparaset.addpcplc=query.value(18).toString();
       sysparaset.addpcsecs=query.value(19).toString();
       sysparaset.addpcphoto=query.value(20).toString();
       sysparaset.stepdj=query.value(21).toFloat();
       sysparaset.stepupdj=query.value(22).toFloat();
      // sysparaset.zfreshold=query.value(23).toFloat();
      // sysparaset.ffreshold=query.value(24).toFloat();
       sysparaset.dytm=query.value(25).toInt();
       sysparaset.szunit=query.value(26).toInt();
       sysparaset.kjunit=query.value(27).toInt();
       sysparaset.sjposnum=query.value(28).toInt();
       sysparaset.szmin=query.value(29).toInt();
       sysparaset.kjmin=query.value(30).toInt();
       /***************************
       sysparaset.ppdelay=query.value(31).toInt();
       sysparaset.urruntm=query.value(32).toInt();
       sysparaset.ppruntm=query.value(33).toInt();
       sysparaset.yjhmruntm=query.value(34).toInt();
       sysparaset.yjqsruntm=query.value(35).toInt();
       sysparaset.szruntm=query.value(36).toInt();
       sysparaset.cqruntm=query.value(37).toInt();
       sysparaset.cqqcruntm=query.value(38).toInt();
       sysparaset.xlruntm=query.value(39).toInt();
        **********************************/
       sysparaset.szkg=query.value(40).toInt();
     //  sysparaset.ppnum=query.value(41).toInt();
      // sysparaset.cullfreshold=query.value(42).toFloat();
   //    sysparaset.boxfreshold=query.value(43).toFloat();

       sysparaset.freshold1=query.value(44).toFloat();
       sysparaset.freshold2=query.value(45).toFloat();
       sysparaset.freshold3=query.value(46).toFloat();
       sysparaset.freshold41=query.value(47).toFloat();
       sysparaset.freshold42=query.value(48).toInt();
       sysparaset.freshold43=query.value(49).toFloat();
       sysparaset.freshold44=query.value(50).toInt();
       sysparaset.modelsel=query.value(51).toInt();

       sysparaset.addplc1=query.value(52).toString();//JAKA ip
       sysparaset.portplc1=query.value(53).toInt();
       sysparaset.addplc2=query.value(54).toString();//
       sysparaset.portplc2=query.value(55).toInt();

       sysparaset.dx=query.value(56).toDouble();
       sysparaset.dy=query.value(57).toDouble();
       sysparaset.dz=query.value(58).toDouble();

       In_parameter(0,0)=query.value(59).toDouble();
       In_parameter(0,1)=query.value(60).toDouble();
       In_parameter(0,2)=query.value(61).toDouble();
       In_parameter(1,0)=query.value(62).toDouble();
       In_parameter(1,1)=query.value(63).toDouble();
       In_parameter(1,2)=query.value(64).toDouble();
       In_parameter(2,0)=query.value(65).toDouble();
       In_parameter(2,1)=query.value(66).toDouble();
       In_parameter(2,2)=query.value(67).toDouble();

       sysparaset.linearspeed=35;
       sysparaset.angularspeed=0.3;
       sysparaset.errthreshold=1.0;

      // sysparaset.bpppinchk=query.value(51).toBool();
     //  sysparaset.bcullconnectchk=query.value(52).toBool();
     //  sysparaset.bcullchipchk=query.value(53).toBool();

     //  qDebug()<<"******************djstep:"<<sysparaset.stepdj;
      // QMessageBox::information(0,"提示",);
    }

    QString qserver;
    qserver="ws://"+sysparaset.addplc+":"+QString::number(sysparaset.portplc);
    ui->ServerUrl->setText(qserver);
    emit C_connect(qserver);//link to ros
    sleep(1);
    //if(rosconnect==0)QMessageBox::information(this,"提示","ROS机器人连接失败!");
    //else
    {
        QJsonObject msg;
        msg.insert("msg", 1);
        QString service="/robuster/robuster_base/control";
        callservice(service, msg);//启动底盘驱动
        //QJsonObject msg;
        //msg.insert("msg", 1);
        usleep(100000);
        service="/robuster/navigation/control";
        callservice(service, msg);//启动导航
        /**********
        QString topic="/move_base/status";
        QString type="";
        subscribe(topic,type);//获取导航状态
        **********/

    }




    //link to JAKA
    int flag,flag1;
    char *rosip;
    QByteArray strip=sysparaset.addplc1.toLatin1();
    rosip=strip.data();
    flag=JKrobot.login_in(rosip);//"10.5.5.100"
    if(flag!=0) QMessageBox::information(this,"提示","Jaka机器人连接失败！错误代码:"+QString::number(flag));
    else   //(flag==0)
    {
       // QMessageBox::information(this,"提示","Jaka机器人连接成功！");
        Jkstatus.is_socket_connect=1;
        flag1=JKrobot.power_on();
        if(flag1!=0) QMessageBox::information(this,"提示","Jaka机器人上电失败！错误代码:"+QString::number(flag));
        sleep(1);
        flag1=JKrobot.enable_robot();
        if(flag1!=0) QMessageBox::information(this,"提示","Jaka机器人使能失败！错误代码:"+QString::number(flag));
        ui->pushButton_7->setEnabled(true);
    }

    value=query.exec("select * from equpara order by id");
    if(!value) QMessageBox::information(this,"提示","电气柜参数表连接接失败！");
    int i=0;
    while(query.next())//指向第一条记录，修改须与设置中修改一致
    {
      equparaset[i].id=query.value(0).toString().simplified();
      equparaset[i].no=query.value(1).toString().simplified();
      equparaset[i].title=query.value(2).toString().simplified();
      equparaset[i].roomno=query.value(3).toInt();
      equparaset[i].type=query.value(4).toInt();
      equparaset[i].x=query.value(5).toDouble();
      equparaset[i].y=query.value(6).toDouble();
      equparaset[i].z=query.value(7).toDouble();
      equparaset[i].rx=query.value(8).toDouble();
      equparaset[i].ry=query.value(9).toDouble();
      equparaset[i].rz=query.value(10).toDouble();
      equparaset[i].shx=query.value(11).toDouble();
      equparaset[i].shy=query.value(12).toDouble();
      equparaset[i].shz=query.value(13).toDouble();
      equparaset[i].shx1=query.value(14).toDouble();
      equparaset[i].shy1=query.value(15).toDouble();
      equparaset[i].shz1=query.value(16).toDouble();
      equparaset[i].shw1=query.value(17).toDouble();
      equparaset[i].calibration_dz=query.value(18).toDouble();
      i++;
    }
    nequ=i;
   // QString qx;
   // qx.sprintf("%f",equparaset[0].x);
   // QMessageBox::information(this,"提示",equparaset[0].id);//qx);
    value=query.exec("select * from kkpara order by no");
    if(!value) QMessageBox::information(this,"提示","开关参数表连接接失败！");
    i=0;
    while(query.next())//指向第一条记录，修改须与设置中修改一致
    {
      kkparaset[i].id=query.value(0).toString().simplified();
      kkparaset[i].no=query.value(1).toString().simplified();
      kkparaset[i].name=query.value(2).toString().simplified();
      kkparaset[i].type=query.value(3).toInt();
      kkparaset[i].value=query.value(4).toInt();
      kkparaset[i].dx=query.value(5).toDouble();//相对于标定点的偏移量dy,dz,dx, 单位mm
      kkparaset[i].dy=query.value(6).toDouble();
      kkparaset[i].dz=query.value(7).toDouble();
      kkparaset[i].locate_dx=query.value(8).toDouble();//开关标定点修正值dx，dy，dz，单位mm
      kkparaset[i].locate_dy=query.value(9).toDouble();
      kkparaset[i].locate_dz=query.value(10).toDouble();
      kkparaset[i].pos=query.value(11).toInt();
      kkparaset[i].pix_w=query.value(12).toInt();  //宽度像素值 拍照位置
      kkparaset[i].pix_h=query.value(13).toInt();  //高度像素值
      kkparaset[i].pix_x=query.value(14).toInt();  //宽度像素值 操作位置
      kkparaset[i].pix_y=query.value(15).toInt();  //高度像素值
      kkparaset[i].color=query.value(16).toInt();  //颜色值，0-无色，1-黑色，2-红色，3-绿色，4-蓝色5-带灯红色，6-带灯绿色
      kkparaset[i].opt_x=query.value(17).toDouble(); //开关操作坐标修正值
      kkparaset[i].opt_y=query.value(18).toDouble();
      kkparaset[i].opt_z=query.value(19).toDouble();
      kkparaset[i].locate_angle6=query.value(20).toDouble();
      kkparaset[i].opt_angle6=query.value(21).toDouble();
      kkparaset[i].color_value1=query.value(22).toInt();
      kkparaset[i].color_value2=query.value(23).toInt();
      kkparaset[i].color_value3=query.value(24).toInt();
      kkparaset[i].color_value4=query.value(25).toInt();
      kkparaset[i].color_value5=query.value(26).toInt();
      kkparaset[i].color_value6=query.value(27).toInt();
      kkparaset[i].color_value7=query.value(28).toInt();
      kkparaset[i].color_value8=query.value(29).toInt();
      kkparaset[i].color_value9=query.value(30).toInt();
      kkparaset[i].color_value10=query.value(31).toInt();
      kkparaset[i].color_value11=query.value(32).toInt();
      kkparaset[i].color_value12=query.value(33).toInt();

      i++;
    }
    nkk=i;
    //QMessageBox::information(this,"提示",QString::number(nkk));

    dlggl=new Dialoggl(this);
    dlgpose=new Dialogpose(this);

    dlgauto=new Dialogauto(this);
    connect(dlgauto,&Dialogauto::Sendquid,this,&MainWindow::Man_Rosmove);

    dlggl=new Dialoggl(this);
    dlgpose=new Dialogpose(this);


    dlgman=new DialogManual(this);
    connect(dlgman,&DialogManual::Sequid,this,&MainWindow::Man_Rosmove);

    //dlgequ=new Dialogequset(this);
    connect(this, &MainWindow::Initpos, dlgequ, &Dialogequset::disprospos);

    ui->stackedWidget->addWidget(dlgauto);
    ui->stackedWidget->addWidget(dlggl);
    ui->stackedWidget->addWidget(dlgman);

    init_statusBar();

    button_style1="QPushButton{background-color:white;color: black;border-radius: 10px;  border: 2px groove gray;border-style: outset;}";

    button_style2="QPushButton{background-color:black;color:white;border-radius: 10px;  border: 2px groove gray;border-style: inset; }";

    ui->pushButton_8->setStyleSheet(button_style1);
    ui->pushButton_3->setStyleSheet(button_style1);
    ui->pushButton_46->setStyleSheet(button_style1);
    ui->pushButton_47->setStyleSheet(button_style1);
    ui->pushButton_48->setStyleSheet(button_style1);

    ui->stackedWidget->setCurrentWidget(dlgman);
    ui->pushButton_8->setStyleSheet(button_style2);
    runmodel=0;

    //ui->pushButton_2->setEnabled(false);
    //ui->pushButton_3->setEnabled(false);
    //ui->pushButton_4->setEnabled(false);
    //ui->pushButton_5->setEnabled(false);
    //ui->pushButton_6->setEnabled(false);
    //ui->pushButton_7->setEnabled(false);

    ui->widget->hide();//JAKA state
    ui->widget_4->hide();
    ui->frame->hide(); //move
    ui->frame_2->hide(); //navigation
    //ROS
    ui->groupBox->hide();
    ui->widget_2->hide();
    ui->widget_3->hide();   

    //test
    //ui->stackedWidget->hide();
    ui->stackedWidget->move(10,80);//
    ui->stackedWidget->resize(1130,910);
    ui->line->hide(); //test边框
    ui->line_2->hide();
    ui->line_3->hide();
    ui->line_4->hide();

    ui->pushButtonjosn->hide();

    /*************
    ui->label_px_2->hide();
    ui->label_py_2->hide();
    ui->label_pz_2->hide();
    ui->label_x_2->hide();
    ui->label_y_2->hide();
    ui->label_z_2->hide();
***************/

    res=pthread_create(&pthrcv,NULL,ThreadProcrcv,NULL);//启动通信线程
    if(res!=0)
    {
      QMessageBox::information(NULL, "提示", "通信线程启动失败，请退出重新进人！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
     // printf("ThreadProcrcv thread create fail");
     return;
    }
    //*********
        res=pthread_create(&pthjaka,NULL,Threadjaka,NULL);//启动Jaka机器人线程
        if(res!=0)
        {
          QMessageBox::information(NULL, "提示", "Jaka机器人线程启动失败，请退出重新进人！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
          //printf("ThreadProcductdata thread create fail");
         return;
        }

    //**************

        res=pthread_create(&pthros,NULL,Threadros,NULL);//启动ROS线程
        if(res!=0)
        {
          QMessageBox::information(NULL, "提示", "ROS线程启动失败，请退出重新进人！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

         // printf("Threadpy thread create fail");
         return;
        }

        res=pthread_create(&pthphoto,NULL,Threadphoto,NULL);//启动相机线程
        if(res!=0)
        {
          QMessageBox::information(NULL, "提示", "相机线程线程启动失败，请退出重新进人！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
         // printf("Threadsecs thread create fail");
         return;
        }

        res=pthread_create(&pthprocess,NULL,ThreadProcess,NULL);//启动处理线程
        if(res!=0)
        {
          QMessageBox::information(NULL, "提示", "处理线程线程启动失败，请退出重新进人！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
         // printf("Threadsecs thread create fail");
         return;
        }
        
        timerstu = new QTimer(this);     
        connect(timerstu,SIGNAL(timeout()),this,SLOT(dispstu()));
        timerstu->start(100);
        
       // timerroscmd = new QTimer(this);
       // connect(timerroscmd,SIGNAL(timeout()),this,SLOT(sendroscmd()));
       // timerroscmd->start(1000);
        
       // QString appdir=QCoreApplication::applicationDirPath();//Debug

       // QStringList datadir=QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);
       // QMessageBox::information(this,"提示:datadir",datadir[1]);
          
}

void MainWindow::init_statusBar()//状态栏显示
{
    char fn[50];

    QStatusBar* bar = ui->statusbar;     //获取状态栏
    statusLabel1t = new QLabel(this); //新建标签
    statusLabel1t->setMinimumSize(100,40); //设置标签最小尺寸
    statusLabel1t->setFrameShape(QFrame::WinPanel); //设置标签形状
    statusLabel1t->setFrameShadow(QFrame::Sunken); //设置标签阴影

    //******************
    statusLabel1p = new QLabel(this);
    statusLabel1p->setFixedSize(40,40); //     setMinimumSize(20,20);
    statusLabel1p->setFrameShape(QFrame::WinPanel);
    statusLabel1p->setFrameShadow(QFrame::Sunken);
    //**********************
//************
    statusLabel2p = new QLabel(this);
    statusLabel2p->setFixedSize(40,40); //     setMinimumSize(20,20);
    statusLabel2p->setFrameShape(QFrame::WinPanel);
    statusLabel2p->setFrameShadow(QFrame::Sunken);

    statusLabel2t = new QLabel(this);
    statusLabel2t->setFixedSize(100,40); //     setMinimumSize(20,20);
    statusLabel2t->setFrameShape(QFrame::WinPanel);
    statusLabel2t->setFrameShadow(QFrame::Sunken);
//************
    statusLabel3p = new QLabel(this);
    statusLabel3p->setFixedSize(40,40); //     setMinimumSize(20,20);
    statusLabel3p->setFrameShape(QFrame::WinPanel);
    statusLabel3p->setFrameShadow(QFrame::Sunken);

    statusLabel3t = new QLabel(this);
    statusLabel3t->setFixedSize(100,40); //     setMinimumSize(20,20);
    statusLabel3t->setFrameShape(QFrame::WinPanel);
    statusLabel3t->setFrameShadow(QFrame::Sunken);

    statusLabel4p = new QLabel(this);
    statusLabel4p->setFixedSize(40,40); //     setMinimumSize(20,20);
    statusLabel4p->setFrameShape(QFrame::WinPanel);
    statusLabel4p->setFrameShadow(QFrame::Sunken);

    statusLabel4t = new QLabel(this);
    statusLabel4t->setFixedSize(300,40); //     setMinimumSize(20,20);
    statusLabel4t->setFrameShape(QFrame::WinPanel);
    statusLabel4t->setFrameShadow(QFrame::Sunken);

    statusLabel5p = new QLabel(this);
    statusLabel5p->setFixedSize(40,40); //     setMinimumSize(20,20);
    statusLabel5p->setFrameShape(QFrame::WinPanel);
    statusLabel5p->setFrameShadow(QFrame::Sunken);

    statusLabel5t = new QLabel(this);
    statusLabel5t->setFixedSize(150,40); //     setMinimumSize(20,20);
    statusLabel5t->setFrameShape(QFrame::WinPanel);
    statusLabel5t->setFrameShadow(QFrame::Sunken);

    statusLabel6p = new QLabel(this);
    statusLabel6p->setFixedSize(40,40); //     setMinimumSize(20,20);
    statusLabel6p->setFrameShape(QFrame::WinPanel);

    statusLabel6t = new QLabel(this);
    statusLabel6t->setFixedSize(150,40); //     setMinimumSize(20,20);
    statusLabel6p->setFrameShadow(QFrame::Sunken);
    statusLabel6t->setFrameShape(QFrame::WinPanel);
    statusLabel6t->setFrameShadow(QFrame::Sunken);

    bar->addWidget(statusLabel1p);
    bar->addWidget(statusLabel1t);
    bar->addWidget(statusLabel2p);
    bar->addWidget(statusLabel2t);
    bar->addWidget(statusLabel3p);
    bar->addWidget(statusLabel3t);
    bar->addWidget(statusLabel4p);
    bar->addWidget(statusLabel4t);
    bar->addWidget(statusLabel5p);
    bar->addWidget(statusLabel5t);
    bar->addWidget(statusLabel6p);

    bar->addWidget(statusLabel6t);

    statusLabel1t->setText(""); //初始化内容
    statusLabel2t->setText("");
    statusLabel3t->setText(""); //初始化内容
    statusLabel4t->setText("");
    statusLabel5t->setText(""); //初始化内容
    statusLabel6t->setText(""); //初始化内容

    update();





    //statusLabel2t->setText("编辑器fdfd5");

}

void MainWindow::paintEvent(QPaintEvent *)//画状态栏图形
{
    QImage image;
    char fn[50];
    sprintf(fn,"%s/%s",dirpf,"user.jpg");
    image.load(fn); //"/home/xzh/testfinspushbutton/user.jpg");
    QPainter painter(this);
    QPixmap pix = QPixmap::fromImage( image.scaled(size(), Qt::KeepAspectRatio) );
    //second_statusLabel->resize(20,20);//**********************************************************8
    statusLabel1p->setPixmap(pix );
    statusLabel1p->setScaledContents(true);
//************
    sprintf(fn,"%s/%s",dirpf,"jaka.png");
    image.load(fn);
    //QPainter painter(this);
    pix = QPixmap::fromImage( image.scaled(size(), Qt::KeepAspectRatio) );
    //second_statusLabel->resize(20,20);//**********************************************************8
    statusLabel2p->setPixmap(pix );
    statusLabel2p->setScaledContents(true);
//**************
    sprintf(fn,"%s/%s",dirpf,"mr2000.jpg");
    image.load(fn);  //"/home/xzh/testfinspushbutton/PLC.png");
    //QPainter painter(this);
    pix = QPixmap::fromImage( image.scaled(size(), Qt::KeepAspectRatio) );
    //second_statusLabel->resize(20,20);//**********************************************************8
    statusLabel3p->setPixmap(pix );
    statusLabel3p->setScaledContents(true);

    sprintf(fn,"%s/%s",dirpf,"robot.png");
    image.load(fn); //"/home/xzh/testfinspushbutton/photo.jpg");
    //QPainter painter(this);
    pix = QPixmap::fromImage( image.scaled(size(), Qt::KeepAspectRatio) );
    //second_statusLabel->resize(20,20);//**********************************************************8
    statusLabel4p->setPixmap(pix );
    statusLabel4p->setScaledContents(true);

    sprintf(fn,"%s/%s",dirpf,"time.jpg");
    image.load(fn); //"/home/xzh/testfinspushbutton/time.jpg");
    //QPainter painter(this);
    pix = QPixmap::fromImage( image.scaled(size(), Qt::KeepAspectRatio) );
    //second_statusLabel->resize(20,20);//**********************************************************8
    statusLabel5p->setPixmap(pix );
    statusLabel5p->setScaledContents(true);

    sprintf(fn,"%s/%s",dirpf,"model.gif");
    image.load(fn); //"/home/xzh/testfinspushbutton/time.jpg");
    //QPainter painter(this);
    pix = QPixmap::fromImage( image.scaled(size(), Qt::KeepAspectRatio) );
    //second_statusLabel->resize(20,20);//**********************************************************8
    statusLabel6p->setPixmap(pix );
    statusLabel6p->setScaledContents(true);
}


void MainWindow::dispstu()//JAKA State
{
    QString qf;
    if(runmodel==3)//调试
    {
        qf.sprintf("%f",Jkstatus.cartesiantran_position[0]);  //机器人末端所在的笛卡尔空间位置
        ui->label_x->setText(qf);
        qf.sprintf("%f",Jkstatus.cartesiantran_position[1]);
        ui->label_y->setText(qf);
        qf.sprintf("%f",Jkstatus.cartesiantran_position[2]);
        ui->label_z->setText(qf);
        qf.sprintf("%f",Jkstatus.cartesiantran_position[3]);
        ui->label_px->setText(qf);
        qf.sprintf("%f",Jkstatus.cartesiantran_position[4]);
        ui->label_py->setText(qf);
        qf.sprintf("%f",Jkstatus.cartesiantran_position[5]);
        ui->label_pz->setText(qf);
        //机器人关节空间位置
        qf.sprintf("%f",Jkstatus.joint_position[0]);  //机器人末端所在的笛卡尔空间位置
        ui->labelj0->setText(qf);
        qf.sprintf("%f",Jkstatus.joint_position[1]);
        ui->labelj1->setText(qf);
        qf.sprintf("%f",Jkstatus.joint_position[2]);
        ui->labelj2->setText(qf);
        qf.sprintf("%f",Jkstatus.joint_position[3]);
        ui->labelj3->setText(qf);
        qf.sprintf("%f",Jkstatus.joint_position[4]);
        ui->labelj4->setText(qf);
        qf.sprintf("%f",Jkstatus.joint_position[5]);
        ui->labelj5->setText(qf);

        ui->labelpos->setText(QString::number(Jkstatus.inpos));
        ui->labeltoolid->setText(QString::number(Jkstatus.current_tool_id));
        ui->labeluserid->setText(QString::number(Jkstatus.current_user_id));
        ui->labelerrcode->setText(QString::number(Jkstatus.errcode));
        ui->labelconnect->setText(QString::number(Jkstatus.is_socket_connect));
        ui->labelpoweron->setText(QString::number(Jkstatus.powered_on));
        ui->labelenabled->setText(QString::number(Jkstatus.enabled));
        ui->labelprotect->setText(QString::number(Jkstatus.protective_stop));
        ui->labelemstop->setText(QString::number(Jkstatus.emergency_stop));
    }
    //状态栏更新
    statusLabel1t->setText(logname); //user
    //int res;


    switch(rosconnect)//
    {
       case 1:statusLabel3t->setText("ROS已连接");statusLabel3t->setStyleSheet("color:black;");break;
       case 0:statusLabel3t->setText("ROS未连接!");statusLabel3t->setStyleSheet("color:red;");break;
    }

    switch(Jkstatus.is_socket_connect)//Jaka
    {
      case 1:statusLabel2t->setText("JAKA已连接");statusLabel2t->setStyleSheet("color:black;");break;
      case 0:statusLabel2t->setText("JAKA未连接!");statusLabel2t->setStyleSheet("color:red;");break;

    }

    if(step==1)
    {
        switch(rosGoalStatus)//ROS
        {
          case 1:statusLabel4t->setText("ROS导航中...");statusLabel4t->setStyleSheet("color:black;");break;
          case 0:statusLabel4t->setText("ROS等待目标点...");statusLabel4t->setStyleSheet("color:black;");break;
          case 3:statusLabel4t->setText("ROS到达目标点");statusLabel4t->setStyleSheet("color:black;");break;
          case 4:statusLabel4t->setText("ROS异常终止");statusLabel4t->setStyleSheet("color:red;");break;

        }
    }

    if(step==2)
    {
       if(jakaopt==20)
       {
           statusLabel4t->setText("JAKA 舒展中...");
           statusLabel4t->setStyleSheet("color:black;");
       }
       if(jakaopt==21)
       {
           statusLabel4t->setText("JAKA 舒展到位");
           statusLabel4t->setStyleSheet("color:black;");
       }

    }

    if(step==3)
    {
       if(jakaopt==30)
       {
           statusLabel4t->setText("JAKA 初始位运行中...");
           statusLabel4t->setStyleSheet("color:black;");
       }
       if(jakaopt==31)
       {
           statusLabel4t->setText("JAKA 初始位到位,标定计算中...");
           statusLabel4t->setStyleSheet("color:black;");
       }

    }

    if(step==4)
    {
        statusLabel4t->setText("标定计算完成");
        statusLabel4t->setStyleSheet("color:black;");
    }

    if(step==5)
    {
       if(jakaopt==50)
       {
           statusLabel4t->setText("JAKA 开关操作位运行中...");
           statusLabel4t->setStyleSheet("color:black;");
       }
       if(jakaopt==51)
       {
           statusLabel4t->setText("JAKA 开关操作位到位");
           statusLabel4t->setStyleSheet("color:black;");
       }

    }

    if(step==6)
    {
       if(jakaopt==60)
       {
           statusLabel4t->setText("JAKA 开关操作中...");
           statusLabel4t->setStyleSheet("color:black;");
       }
       if(jakaopt==61)
       {
           statusLabel4t->setText("JAKA 开关操作到位");
           statusLabel4t->setStyleSheet("color:black;");
       }

    }



   // statusLabel4t->setText(sysparaset.pfname);//product

    statusLabel5t->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")); //time

    switch(runmodel)//0:手动，1：自动，2：设置，3：调试
    {
       case 0:statusLabel6t->setText("手动模式");break;
       case 1:statusLabel6t->setText("自动模式");break;
       case 2:statusLabel6t->setText("设置模式");break;
       case 3:statusLabel6t->setText("调试模式");break;

    }

}

void MainWindow::sendroscmd()//ROS CMD
{
   QString topic;
   QString type;
   switch(roscmd) 
   {
      case 271:topic="/robot_pose"; //获取当前位姿
               type="geometry_msgs/Pose";
               subscribe(topic,type); 
               break;
      case 270: topic="/robot_pose"; //停止获取当前位姿
               // QString type="geometry_msgs/Pose";
               unsubscribe(topic);
               break;
                      
                       
                      
   }
}


/* 客户端操作 begin */
void MainWindow::C_reciveb(const QByteArray data)//QString message)
{
    QMessageBox::information(this,"提示","进入QByteArray接收数据！");
    QString datetime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    ui->client_rec->append(datetime);
    //ui->client_rec->append(data);//message);


        QJsonParseError err_rpt;
        QJsonDocument  root_Doc = QJsonDocument::fromJson(data, &err_rpt);//字符串格式化为JSON

        if(err_rpt.error != QJsonParseError::NoError)
        {
            //qDebug() << "JSON格式错误";
            QMessageBox::information(this,"提示","JSON格式错误！");
           // return -1;
        }
        //else QMessageBox::information(this,"提示","JSON格式正确！");

        QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);
        QString qindent=QString(root_str);
        ui->client_rec->append(qindent);


}
void MainWindow::C_recive(const QString message)
{
   // QMessageBox::information(this,"提示","进入QString接收数据！");
    QString datetime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    ui->client_rec->append(datetime);
   // ui->client_rec->append(message);
    //*******************
    QJsonParseError err_rpt;
    QJsonDocument  doucment = QJsonDocument::fromJson(message.toLatin1(), &err_rpt);//字符串格式化为JSON

    if(err_rpt.error != QJsonParseError::NoError)
    {
        //qDebug() << "JSON格式错误";
        QMessageBox::information(this,"提示","JSON格式错误！");
        return;
    }
    else
    {
        //QMessageBox::information(this,"提示","JSON格式正确！");
        //解析JSON
   // QJsonParseError jsonError;
   // QJsonDocument doucment = QJsonDocument::fromJson(byteArray, &jsonError);
    if (!doucment.isNull())//&& (jsonError.error == QJsonParseError::NoError))
    {
        if (doucment.isObject())
        {
            QJsonObject object = doucment.object();
            if (object.contains("op")&&object.contains("topic"))
            {
                QString opvalue = object.value("op").toString();
                QString topicvalue = object.value("topic").toString();

                if (opvalue=="publish"&&topicvalue=="/move_base/status")
                {
                   QJsonObject msgobj = object.value("msg").toObject();
                   /* data键 */
                   QJsonValue stVaule = msgobj.value("status_list");
                   if(stVaule.isArray())
                   {
                      // for(int idx = 0; idx <= 3; idx++)
                      // {
                           QJsonObject data_Obj = stVaule.toArray().at(0).toObject();
                           QString add = data_Obj.value("goal_id").toString();//"id：" +

                           rosGoalStatus =data_Obj.value("status").toInt();
                           QString qsaqi=QString::number(rosGoalStatus);//" status：" +
                           QString lv = data_Obj.value("text").toString();//" text：" +

                           ui->labelstatus->setText(qsaqi);
                           ui->labeltext->setText(lv);
                           ui->labelid->setText(add);

                          // qDebug() << add + qsaqi + lv ;
                    }
                }//navagation state

                if (opvalue=="publish"&&topicvalue=="/robot_pose")//位姿
                {
                   QJsonObject msgobj = object.value("msg").toObject();
                   QJsonObject posobj=msgobj.value("position").toObject();
                   QJsonObject oriobj=msgobj.value("orientation").toObject();
                   /* data键 */
                   rospos.x=posobj.value("x").toDouble();
                   rospos.y=posobj.value("y").toDouble();
                   rospos.z=posobj.value("z").toDouble();
                   rospos.x1=oriobj.value("x").toDouble();
                   rospos.y1=oriobj.value("y").toDouble();
                   rospos.z1=oriobj.value("z").toDouble();
                   rospos.w1=oriobj.value("w").toDouble();
                   emit Initpos();

                }//pos state



            }//if (object.contains("op")&&object.contains("topic"))




       }//if (doucment.isObject())



    }//if (!doucment.isNull())

    QByteArray root_str = doucment.toJson(QJsonDocument::Indented);
    QString qindent=QString(root_str);
    ui->client_rec->append(qindent);
  }//else


}

void MainWindow::C_error(const QString msg)
{
    QString datetime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    ui->client_rec->append(datetime);
    ui->client_rec->append(msg);
}
void MainWindow::C_connection(const QString msg, const bool state)
{
    QString datetime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    ui->client_rec->append(datetime);
    ui->client_rec->append(msg);
    if(state)
    {
        rosconnect=true;
        ui->client_connect->setEnabled(false);
        ui->client_disconnect->setEnabled(true);
    }
    else
    {
        rosconnect=false;
        ui->client_connect->setEnabled(true);
        ui->client_disconnect->setEnabled(false);
    }
}


void MainWindow::Man_Rosmove(QString id)
{
  //move ros to equid
   //QMessageBox::information(this,"提示","柜ID："+id);
    //创建JSON对象    
    QJsonObject root_Obj;
    int sec,usec;
    struct timeval us;
    QString q0;
    int idx;
    //q0="00";
    double quat[4],eu[3],coord[3];
    static QString qid="";

    if(id!=qid)isnewequ=true;
    else isnewequ=false;
    if(gbd)
    {
        step=2;        
        return;
    }
    if(isnewequ)
    {
        //*********************
        QString topic="/move_base/status";
        QString type="";
        subscribe(topic,type);//获取导航状态
        usleep(1000);
        idx=equid2index(id);
        coord[0]=equparaset[idx].shx;
        coord[1]=equparaset[idx].shy;
        coord[2]=equparaset[idx].shz;
        quat[0] =equparaset[idx].shx1;
        quat[1] =equparaset[idx].shy1;
        quat[2] =equparaset[idx].shz1;//
        quat[3] =equparaset[idx].shw1;

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
        publish(topic,msg);

        qid=id;
        gnum=0;
        step=1;

    }
    else step=2;//step=2;

}

void MainWindow::web_sub(QString topic,QString type)
{
    subscribe(topic,type);
}

void MainWindow::web_unsub(QString topic)
{
    unsubscribe(topic);
}

void MainWindow::web_adv(QString topic,QString type)
{
    adverise(topic,type);
}

void MainWindow::web_unadv(QString topic)
{
    unadverise(topic);
}

void MainWindow::web_pub(QString topic,QJsonObject msg)
{
    publish(topic,msg);
}

void MainWindow::web_ser(QString service,QJsonObject msg)
{
    callservice(service,msg);
}

MainWindow::~MainWindow()
{


    int ret,res;
    QSqlQuery query;



    //QMessageBox::information(NULL, "提示", "程序停止！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    /************
    switch(QMessageBox::question(this, "Question提示", "是否停止程序运行？", QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok))
    {
      case QMessageBox::Ok:
            break;
      case QMessageBox::Cancel:
            return;

    }
*************************/

    res=pthread_cancel(pthrcv);
    if(res!=0) QMessageBox::information(NULL, "提示", "线程RECV停止失败！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

    res=pthread_cancel(pthros);
    if(res!=0) QMessageBox::information(NULL, "提示", "調用线程ROS停止失败！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
//*********************************
    res=pthread_cancel(pthjaka);
    if(res!=0) QMessageBox::information(NULL, "提示", "調用线程JAKA停止失败！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);


    res=pthread_cancel(pthphoto);   
    if(res!=0) QMessageBox::information(NULL, "提示", "調用线程PHOTO停止失败！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

    res=pthread_cancel(pthprocess);
    if(res!=0) QMessageBox::information(NULL, "提示", "調用线程PRECESS停止失败！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

    timerstu->stop();
    //timerroscmd->stop();
    
    if(Jkstatus.is_socket_connect!=1)return;
    JKrobot.disable_robot();
    JKrobot.power_off();
    JKrobot.login_out();
    
//********************************
   //  timerstu->stop();
   //  timerdel->stop();
   //  dlgauto->timeralarm->stop();

     //退出操作记录
    // bool value;
     /**********************************
     QString tm=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
     QString item="退出";
     QString user=logid;
     QString demo="系统退出";
     bool value=query.exec("INSERT INTO syslog VALUES('"+tm+"','"+logid+"','"+item+"','"+demo+"')");
     if(value)
     {
      //   QMessageBox::information(this,"提示","退出操作记录成功！");
        // this->close();
     }
     else
     {
         QMessageBox::information(this,"提示","退出操作记录失败！");
        // this->close(); QTimer *timeralarm;
     }
********************************************/

     dlggl->close();
     delete dlggl;

     dlgman->close();
     delete dlgman;

     dlgtest->close();
     delete dlgtest;

     dlgauto->close();
     delete dlgauto;

     delete dlgpose;

     if(client_thread->isRunning())
     {
         client_thread->quit();  //退出线程运行
         if(!client_thread->wait(3000))  //等待3000ms，如果超时还未完全退出
         {
             QMessageBox::warning(this,"警告","client_thread->terminate");
             client_thread->terminate(); //强制中断线程运行
         }
     }


    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    int flag;
    char *rosip;
    QByteArray strip=sysparaset.addplc1.toLatin1();
    rosip=strip.data();
    flag=JKrobot.login_in(rosip);//"10.5.5.100"
    if(flag!=0) QMessageBox::information(this,"提示","Jaka机器人连接失败！错误代码:"+QString::number(flag));
    else
    {
        QMessageBox::information(this,"提示","Jaka机器人连接成功！");
        Jkstatus.is_socket_connect=1;
    }
    if(Jkstatus.is_socket_connect)
    {
     JKrobot.power_on();
     JKrobot.enable_robot();
     ui->pushButton_7->setEnabled(true);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    if(Jkstatus.is_socket_connect!=1)
    {
        QMessageBox::information(this,"提示","JAKA控制器连接失败，请检查！");
        return;
    }
     dlgmove=new DialogJakamovetest(this);
     dlgmove->show();

}

void MainWindow::on_pushButton_3_clicked()
{
    ui->stackedWidget->setCurrentWidget(dlggl);
    ui->pushButton_8->setStyleSheet(button_style1);//gl
    ui->pushButton_3->setStyleSheet(button_style2);
    ui->pushButton_46->setStyleSheet(button_style1);
    ui->pushButton_47->setStyleSheet(button_style1);
    ui->pushButton_48->setStyleSheet(button_style1);
    ui->stackedWidget->show();
    runmodel=2;
    ui->line->hide(); //test边框
    ui->line_2->hide();
    ui->line_3->hide();
    ui->line_4->hide();
    ui->pushButtonjosn->hide();
    ui->widget_4->hide();
}

void MainWindow::on_pushButton_4_clicked()
{
    static bool disp=true;
    if(disp)ui->widget->show();
    else ui->widget->hide();
    disp=!disp;


}

void MainWindow::on_pushButton_5_clicked()
{

    MultStrStorType fn;
    if(Jkstatus.is_socket_connect!=1)return;
    JKrobot.get_exist_traj_file_name(&fn);
    if(fn.len<1)return;
    else
    {
       //ui->label_13->setText(fn.name[0]);
       //if(fn.len>1)ui->label_14->setText(fn.name[1]);
    }
}

void MainWindow::on_pushButton_6_clicked()
{
    if(Jkstatus.is_socket_connect!=1)return;
    int res=JKrobot.program_load("test2");
    if(res==0)QMessageBox::information(this,"提示","加载成功！");
    JKrobot.program_run();
}

void MainWindow::on_pushButton_7_clicked()
{
    dlgtest=new Dialogtest(this);
    dlgtest->show();
}

void MainWindow::on_pushButton_8_clicked()
{

    ui->stackedWidget->setCurrentWidget(dlgman);
    ui->pushButton_8->setStyleSheet(button_style2);//man
    ui->pushButton_3->setStyleSheet(button_style1);
    ui->pushButton_46->setStyleSheet(button_style1);
    ui->pushButton_47->setStyleSheet(button_style1);
    ui->pushButton_48->setStyleSheet(button_style1);
    ui->stackedWidget->show();
    runmodel=0;
    ui->line->hide(); //test边框
    ui->line_2->hide();
    ui->line_3->hide();
    ui->line_4->hide();
    ui->pushButtonjosn->hide();
    ui->widget_4->hide();
}

void MainWindow::on_client_connect_clicked()
{
    emit C_connect(ui->ServerUrl->text());
}

void MainWindow::on_client_disconnect_clicked()
{
    emit C_disconnect();
}

void MainWindow::on_pushButton_14_clicked()
{
    /*********************
    //创建JSON对象
       QJsonObject root_Obj;

       QJsonObject msg;
       msg.insert("msg", 1); //0/开启，1/关闭

       root_Obj.insert("op", "call_service");//开启小车底盘驱动
       root_Obj.insert("service", "/robuster/robuster_base/control");
       root_Obj.insert("args", msg);


       //创建Json文档
       QJsonDocument root_Doc;
       root_Doc.setObject(root_Obj);
       QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
       //QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
       //QString strJson(root_str);
       //ui->client_msg->setText(strJson);
       ui->client_msg->setText(root_str);

      emit C_sendb(root_str);//send Byte
     ****************/
    QJsonObject msg;
    msg.insert("msg", 1);
    QString service="/robuster/robuster_base/control";
    callservice(service, msg);

}

void MainWindow::on_pushButton_15_clicked()
{
    //创建JSON对象
       QJsonObject root_Obj;
       //添加键值对，值的类型自动识别，顺序不可自定义
       root_Obj.insert("op", "call_service");//查询设备编号
       root_Obj.insert("service", "/robuster/get_devInfo");


       //创建Json文档
       QJsonDocument root_Doc;
       root_Doc.setObject(root_Obj);
       QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
       //QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
       //QString strJson(root_str);
       //ui->client_msg->setText(strJson);
       ui->client_msg->setText(root_str);

      emit C_sendb(root_str);//send Byte
}

void MainWindow::on_pushButton_11_clicked()
{
    /*******************88
    //创建JSON对象
       QJsonObject root_Obj;
       //添加键值对，值的类型自动识别，顺序不可自定义
       root_Obj.insert("op", "subscribe");//查询设备状态
       root_Obj.insert("topic", "/device_state");


       //创建Json文档
       QJsonDocument root_Doc;
       root_Doc.setObject(root_Obj);
       QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
       //QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
       //QString strJson(root_str);
       //ui->client_msg->setText(strJson);
       ui->client_msg->setText(root_str);
       emit C_sendb(root_str);//send Byte
    *********************/
       QString topic="/device_state";
       QString type="";
       subscribe(topic,type);
}

void MainWindow::on_pushButton_10_clicked()
{
    /**************
    //创建JSON对象
       QJsonObject root_Obj;
       //添加键值对，值的类型自动识别，顺序不可自定义
       root_Obj.insert("op", "unsubscribe");//查询设备状态
       root_Obj.insert("topic", "/device_state");

       //创建Json文档
       QJsonDocument root_Doc;
       root_Doc.setObject(root_Obj);
       QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
       //QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
       //QString strJson(root_str);
       //ui->client_msg->setText(strJson);
       ui->client_msg->setText(root_str);
       emit C_sendb(root_str);//send Byte
    *************/
       QString topic="/device_state";
       //QString type="";
       unsubscribe(topic);


}

void MainWindow::on_pushButton_16_clicked()
{
    //创建JSON对象
       QJsonObject root_Obj;
       //添加键值对，值的类型自动识别，顺序不可自定义

       QJsonObject msg;
       msg.insert("permission", 0); //0/获取，1/放弃

       root_Obj.insert("op", "call_service");//获得控制权限
       root_Obj.insert("service", "/set_permission");
       root_Obj.insert("args", msg);

       //创建Json文档
       QJsonDocument root_Doc;
       root_Doc.setObject(root_Obj);
       QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
       //QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
       //QString strJson(root_str);
       //ui->client_msg->setText(strJson);
       ui->client_msg->setText(root_str);

      emit C_sendb(root_str);//send Byte
}

void MainWindow::on_pushButton_12_clicked()
{
    /*************
    //创建JSON对象
       QJsonObject root_Obj;
       //添加键值对，值的类型自动识别，顺序不可自定义
       QJsonObject msg;
       msg.insert("front", 0); //0-100
       msg.insert("rear", 0); //0-100

       root_Obj.insert("op", "call_service");//获得控制权限
       root_Obj.insert("service", "/robuster/light_control");
       root_Obj.insert("args", msg);


       //创建Json文档
       QJsonDocument root_Doc;
       root_Doc.setObject(root_Obj);
       QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
       //QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
       //QString strJson(root_str);
       //ui->client_msg->setText(strJson);
       ui->client_msg->setText(root_str);

      emit C_sendb(root_str);//send Byte
     ***************/
    QJsonObject msg;
    msg.insert("front", 0); //0-100
    msg.insert("rear", 0); //0-100
    QString service="/robuster/light_control";
    callservice(service, msg);

}

void MainWindow::on_pushButton_17_clicked()
{
    /***************
    //创建JSON对象
       QJsonObject root_Obj;
       //添加键值对，值的类型自动识别，顺序不可自定义
       QJsonObject msg;
       msg.insert("front", 100); //0-100
       msg.insert("rear", 100); //0-100

       root_Obj.insert("op", "call_service");//获得控制权限
       root_Obj.insert("service", "/robuster/light_control");
       root_Obj.insert("args", msg);


       //创建Json文档
       QJsonDocument root_Doc;
       root_Doc.setObject(root_Obj);
       QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
       //QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
       //QString strJson(root_str);
       //ui->client_msg->setText(strJson);
       ui->client_msg->setText(root_str);

      emit C_sendb(root_str);//send Byte
     ****************/
       QJsonObject msg;
       msg.insert("front", 100); //0-100
       msg.insert("rear", 100); //0-100
       QString service="/robuster/light_control";
       callservice(service, msg);
}

void MainWindow::on_pushButton_20_clicked()
{
    //创建JSON对象
       QJsonObject root_Obj;
       //添加键值对，值的类型自动识别，顺序不可自定义

       QJsonObject msg;
       unsigned char bj;
       bj=0xf0;
       msg.insert("permission", 0); //bit4：电量/5：电机/6：碰撞/7：温度

       root_Obj.insert("op", "call_service");//解除防撞报警
       root_Obj.insert("service", "/reset_alarm");
       root_Obj.insert("args", msg);
       //创建Json文档
       QJsonDocument root_Doc;
       root_Doc.setObject(root_Obj);
       QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
       //QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
       //QString strJson(root_str);
       //ui->client_msg->setText(strJson);
       ui->client_msg->setText(root_str);

      emit C_sendb(root_str);//send Byte
}

void MainWindow::on_pushButton_19_clicked()
{
    //创建JSON对象
       QJsonObject root_Obj;
       //添加键值对，值的类型自动识别，顺序不可自定义
     /****************
       QJsonObject msg;
       msg.insert("value", 1); //0/off，1/on

       root_Obj.insert("op", "call_service");//开/关异常处理
       root_Obj.insert("service", "/robuster/disable_error");
       root_Obj.insert("args", msg);

       //创建Json文档
       QJsonDocument root_Doc;
       root_Doc.setObject(root_Obj);
       QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
       //QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
       //QString strJson(root_str);
       //ui->client_msg->setText(strJson);
       ui->client_msg->setText(root_str);

      emit C_sendb(root_str);//send Byte
      ************/
}

void MainWindow::on_pushButton_13_clicked()
{
    //创建JSON对象
       QJsonObject root_Obj;
       //添加键值对，值的类型自动识别，顺序不可自定义
       root_Obj.insert("op", "call_service");//IMU复位
       root_Obj.insert("service", "/robuster/reset_imu");


       //创建Json文档
       QJsonDocument root_Doc;
       root_Doc.setObject(root_Obj);
       QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
       //QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
       //QString strJson(root_str);
       //ui->client_msg->setText(strJson);
       ui->client_msg->setText(root_str);

      emit C_sendb(root_str);//send Byte
}

void MainWindow::on_pushButton_18_clicked()
{
    /*******************
    //创建JSON对象
       QJsonObject root_Obj;
       //添加键值对，值的类型自动识别，顺序不可自定义
       root_Obj.insert("op", "subscribe");//获取
       root_Obj.insert("topic", "/motor_state");


       //创建Json文档
       QJsonDocument root_Doc;
       root_Doc.setObject(root_Obj);
       QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
       //QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
       //QString strJson(root_str);
       //ui->client_msg->setText(strJson);
       ui->client_msg->setText(root_str);
       emit C_sendb(root_str);//send Byte
  *****************/
       QString topic="/motor_state";
       QString type="";
       subscribe(topic,type);
}

void MainWindow::on_pushButton_21_clicked()
{
    /***************
    //创建JSON对象
       QJsonObject root_Obj;
       //添加键值对，值的类型自动识别，顺序不可自定义
       root_Obj.insert("op", "subscribe");//获取odom数据
       root_Obj.insert("topic", "/odom");


       //创建Json文档
       QJsonDocument root_Doc;
       root_Doc.setObject(root_Obj);
       QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
       //QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
       //QString strJson(root_str);
       //ui->client_msg->setText(strJson);
       ui->client_msg->setText(root_str);
       emit C_sendb(root_str);//send Byte
   ***************/
       QString topic="/odom";
       QString type="";
       subscribe(topic,type);
}

void MainWindow::on_pushButton_23_clicked()
{
    /****************
    //创建JSON对象
       QJsonObject root_Obj;
       //添加键值对，值的类型自动识别，顺序不可自定义
       root_Obj.insert("op", "subscribe");//获取GPS数据
       root_Obj.insert("topic", "/Fix");


       //创建Json文档
       QJsonDocument root_Doc;
       root_Doc.setObject(root_Obj);
       QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
       //QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
       //QString strJson(root_str);
       //ui->client_msg->setText(strJson);
       ui->client_msg->setText(root_str);
       emit C_sendb(root_str);//send Byte
   **************/
       QString topic="/Fix";
       QString type="";
       subscribe(topic,type);
}

void MainWindow::on_pushButton_24_clicked()
{ sleep(1);
    // if(rosconnect==0)QMessageBox::information(this,"提示","ROS机器人连接失败!");
   //  else
     {
         QJsonObject msg;
         msg.insert("msg", 1);
         QString service="/robuster/robuster_base/control";
         callservice(service, msg);//启动底盘驱动
         //QJsonObject msg;
         //msg.insert("msg", 1);
         usleep(100000);
         service="/robuster/navigation/control";
         callservice(service, msg);//启动导航
         /**********
         QString topic="/move_base/status";
         QString type="";
         subscribe(topic,type);//获取导航状态
         **********/

     }
    /******************
    //创建JSON对象
       QJsonObject root_Obj;
       //添加键值对，值的类型自动识别，顺序不可自定义
       root_Obj.insert("op", "subscribe");//获取电池数据
       root_Obj.insert("topic", "/battery_state");

       //创建Json文档
       QJsonDocument root_Doc;
       root_Doc.setObject(root_Obj);
       QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
       //QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
       //QString strJson(root_str);
       //ui->client_msg->setText(strJson);
       ui->client_msg->setText(root_str);
       emit C_sendb(root_str);//send Byte
   *****************/
       QString topic="/battery_state";
       QString type="";
       subscribe(topic,type);


}

void MainWindow::on_pushButton_25_clicked()//运动控制
{
    /***********************
    //创建JSON对象
       QJsonObject root_Obj;
       //添加键值对，值的类型自动识别，顺序不可自定义
       root_Obj.insert("op", "adverise");//
       root_Obj.insert("topic", "/cmd_vel");
       root_Obj.insert("type", "geometry_msgs/Twist");


       //创建Json文档
       QJsonDocument root_Doc;
       root_Doc.setObject(root_Obj);
       QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
       //QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
       //QString strJson(root_str);
       //ui->client_msg->setText(strJson);
       ui->client_msg->setText(root_str);
       emit C_sendb(root_str);//send Byte
 *******************/
       QString topic="/cmd_vel";
       QString type="geometry_msgs/Twist";
       adverise(topic,type);

       ui->frame->show();
}
void MainWindow::KeyPressEvent(QKeyEvent *ev)
{
    if(ev->key()==Qt::Key_Up)
    {
       //QMessageBox::information(this,"提示","key_up！");
        qDebug()<<"Key_UP";
    }
}

void MainWindow::move(double vl,double va)
{
    //创建JSON对象
    QJsonObject root_Obj;
    QJsonObject linear;
    linear.insert("x", vl);
    linear.insert("y", 0);
    linear.insert("z", 0);

    QJsonObject angular;
    angular.insert("x", 0);
    angular.insert("y", 0);
    angular.insert("z", va);

    QJsonObject msg;
    msg.insert("linear", linear);
    msg.insert("angular", angular);

/**************
    //QJsonObject root_Obj;
    //添加键值对，值的类型自动识别，顺序不可自定义
    root_Obj.insert("op", "publish");
    root_Obj.insert("topic", "/cmd_vel");
    root_Obj.insert("msg", msg);

    QJsonDocument root_Doc;
    root_Doc.setObject(root_Obj);
    QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
    //QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
    //QString strJson(root_str);
    //ui->client_msg->setText(strJson);
    ui->client_msg->setText(root_str);
    emit C_sendb(root_str);//send Byte
  *************/

    QString topic="/cmd_vel";
    publish(topic,msg);
}

void MainWindow::on_pushButton_34_clicked()
{
    move(0.2,0);
}

void MainWindow::on_pushButton_35_clicked()
{
    move(-0.2,0);
}

void MainWindow::on_pushButton_36_clicked()
{
    move(0.0,-0.2);
}

void MainWindow::on_pushButton_37_clicked()
{
    move(0.0,0.2);
}

void MainWindow::on_pushButton_38_clicked()
{
    move(0,0);
}

void MainWindow::on_pushButton_26_clicked()//建图
{
    //创建JSON对象
    /*************************
       QJsonObject root_Obj;
       //添加键值对，值的类型自动识别，顺序不可自定义

       QJsonObject msg;
       msg.insert("msg", 1); //0:启动，/1，关闭

       root_Obj.insert("op", "call_service");//IMU复位
       root_Obj.insert("service", "/robuster/mapping/control");
       root_Obj.insert("args", msg);


       //创建Json文档
       QJsonDocument root_Doc;
       root_Doc.setObject(root_Obj);
       QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
       //QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
       //QString strJson(root_str);
       //ui->client_msg->setText(strJson);
       ui->client_msg->setText(root_str);
       emit C_sendb(root_str);//send Byte
     **********************/
       QJsonObject msg;
       msg.insert("msg", 1);
       QString service="/robuster/mapping/control";
       callservice(service, msg);


}

void MainWindow::on_pushButton_27_clicked()
{
    /*********************
    //创建JSON对象
       QJsonObject root_Obj;
       //添加键值对，值的类型自动识别，顺序不可自定义
       root_Obj.insert("op", "subscribe");//获取地图数据
       root_Obj.insert("topic", "/map");
       root_Obj.insert("type", "/nav_msgs/OccupancyGrid");


       //创建Json文档
       QJsonDocument root_Doc;
       root_Doc.setObject(root_Obj);
       QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
       //QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
       //QString strJson(root_str);
       //ui->client_msg->setText(strJson);
       ui->client_msg->setText(root_str);
      emit C_sendb(root_str);//send Byte
  *******************/
       QString topic="/map";
       QString type="/nav_msgs/OccupancyGrid";
       subscribe(topic,type);
}

void MainWindow::on_pushButton_39_clicked()
{
    /*************
    //创建JSON对象
       QJsonObject root_Obj;
       //添加键值对，值的类型自动识别，顺序不可自定义
       root_Obj.insert("op", "unsubscribe");//停止获取地图数据
       root_Obj.insert("topic", "/map");
       root_Obj.insert("type", "nav_mags/OccupancyGrid");


       //创建Json文档
       QJsonDocument root_Doc;
       root_Doc.setObject(root_Obj);
       QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
       //QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
       //QString strJson(root_str);
       //ui->client_msg->setText(strJson);
       ui->client_msg->setText(root_str);
      emit C_sendb(root_str);//send Byte
     ****************/
       QString topic="/map";
      // QString type="/nav_msgs/OccupancyGrid";
       unsubscribe(topic);
}

void MainWindow::on_pushButton_22_clicked()
{
    //创建JSON对象
       QJsonObject root_Obj;
       //添加键值对，值的类型自动识别，顺序不可自定义
       bool ok;
       QString text = QInputDialog::getText(this, tr("保存文件名设置"),
                                            tr("文件名:"), QLineEdit::Normal,
                                            QDir::home().dirName(), &ok);
       //if (ok && !text.isEmpty())


       QJsonObject msg;
       msg.insert("filename", text); //文件名
/*******************
       root_Obj.insert("op", "call_service");//保存地图
       root_Obj.insert("service", "/mapping/save");
       root_Obj.insert("args", msg);


       //创建Json文档
       QJsonDocument root_Doc;
       root_Doc.setObject(root_Obj);
       QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
       //QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
       //QString strJson(root_str);
       //ui->client_msg->setText(strJson);
       ui->client_msg->setText(root_str);
      emit C_sendb(root_str);//send Byte
 ****************/
       QString service="/mapping/save";
       callservice(service, msg);

}

void MainWindow::on_pushButton_28_clicked()
{
    /*********************
    //创建JSON对象
       QJsonObject root_Obj;
       //添加键值对，值的类型自动识别，顺序不可自定义

       QJsonObject msg;
       msg.insert("msg", 0); //

       root_Obj.insert("op", "call_service");//加载地图列表
       root_Obj.insert("service", "/robuster/mapping/list");
       root_Obj.insert("args", msg);


       //创建Json文档
       QJsonDocument root_Doc;
       root_Doc.setObject(root_Obj);
       QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
       //QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
       //QString strJson(root_str);
       //ui->client_msg->setText(strJson);
       ui->client_msg->setText(root_str);
       emit C_sendb(root_str);//send Byte
   ***************/
       QJsonObject msg;
       msg.insert("msg", 0);
       QString service="/robuster/mapping/list";
       callservice(service, msg);
}

void MainWindow::on_pushButton_30_clicked()
{
    //创建JSON对象
       QJsonObject root_Obj;
       //添加键值对，值的类型自动识别，顺序不可自定义
       bool ok;
       QString text = QInputDialog::getText(this, tr("保存文件名设置"),
                                            tr("文件名:"), QLineEdit::Normal,
                                            QDir::home().dirName(), &ok);
       //if (ok && !text.isEmpty())

       QJsonObject msg;
       msg.insert("filename", text); //文件名
  /***************88
       root_Obj.insert("op", "call_service");//选择地图
       root_Obj.insert("service", "/robuster/mapping/select");
       root_Obj.insert("args", msg);


       //创建Json文档
       QJsonDocument root_Doc;
       root_Doc.setObject(root_Obj);
       QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
       //QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
       //QString strJson(root_str);
       //ui->client_msg->setText(strJson);
       ui->client_msg->setText(root_str);
       emit C_sendb(root_str);//send Byte

       *****************/
       QString service="/robuster/mapping/list";
       callservice(service, msg);
}

void MainWindow::on_pushButton_29_clicked()
{
    /****************
    //创建JSON对象
       QJsonObject root_Obj;
       //添加键值对，值的类型自动识别，顺序不可自定义

       QJsonObject msg;
       msg.insert("msg", 1); //0:启动，1，关闭

       root_Obj.insert("op", "call_service");//启动导航
       root_Obj.insert("service", "/robuster/navigation/control");
       root_Obj.insert("args", msg);


       //创建Json文档
       QJsonDocument root_Doc;
       root_Doc.setObject(root_Obj);
       QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
       //QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
       //QString strJson(root_str);
       //ui->client_msg->setText(strJson);
       ui->client_msg->setText(root_str);
       emit C_sendb(root_str);//send Byte
    *****************/
       QJsonObject msg;
       msg.insert("msg", 1);
       QString service="/robuster/navigation/control";
       callservice(service, msg);
}

void MainWindow::on_pushButton_31_clicked()
{
    /*******************
    //创建JSON对象
       QJsonObject root_Obj;
       //添加键值对，值的类型自动识别，顺序不可自定义
       root_Obj.insert("op", "subscribe");//获取导航轨迹
       root_Obj.insert("topic", "/robuster/move_base/TebLocalPlannerROS/local_plan");
       root_Obj.insert("type", "nav_msgs/Path");


       //创建Json文档
       QJsonDocument root_Doc;
       root_Doc.setObject(root_Obj);
       QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
       //QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
       //QString strJson(root_str);
       //ui->client_msg->setText(strJson);
       ui->client_msg->setText(root_str);
       emit C_sendb(root_str);//send Byte
   ****************/
       QString topic="/robuster/move_base/TebLocalPlannerROS/local_plan";
       QString type="nav_msgs/Path";
       subscribe(topic,type);
}

void MainWindow::on_pushButton_40_clicked()
{
    //创建JSON对象
       QJsonObject root_Obj;
       QJsonObject msg;
       msg.insert("msg","");// "actionlib_msgs/GoalID");
/******************
       //添加键值对，值的类型自动识别，顺序不可自定义
       root_Obj.insert("op", "publish");//
       root_Obj.insert("topic", "/move_base/cancel");
       root_Obj.insert("msg", msg);


       //创建Json文档
       QJsonDocument root_Doc;
       root_Doc.setObject(root_Obj);
       QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
       //QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
       //QString strJson(root_str);
       //ui->client_msg->setText(strJson);
       ui->client_msg->setText(root_str);
       emit C_sendb(root_str);//send Byte
 ****************/
       QString topic="/move_base/cancel";
       publish(topic,msg);
}

void MainWindow::on_pushButton_41_clicked()
{
    /******************
    //创建JSON对象
       QJsonObject root_Obj;
       //添加键值对，值的类型自动识别，顺序不可自定义
       root_Obj.insert("op", "unsubscribe");//stop获取
       root_Obj.insert("topic", "/motor_state");


       //创建Json文档
       QJsonDocument root_Doc;
       root_Doc.setObject(root_Obj);
       QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
       //QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
       //QString strJson(root_str);
       //ui->client_msg->setText(strJson);
       ui->client_msg->setText(root_str);

      emit C_sendb(root_str);//send Byte
    *******************/
       QString topic="/motor_state";
      // QString type="";
       unsubscribe(topic);
}

void MainWindow::on_pushButton_42_clicked()
{
    /**************
    //创建JSON对象
       QJsonObject root_Obj;
       //添加键值对，值的类型自动识别，顺序不可自定义
       root_Obj.insert("op", "unsubscribe");//停止获取odom数据
       root_Obj.insert("topic", "/odom");


       //创建Json文档
       QJsonDocument root_Doc;
       root_Doc.setObject(root_Obj);
       QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
       //QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
       //QString strJson(root_str);
       //ui->client_msg->setText(strJson);
       ui->client_msg->setText(root_str);
       emit C_sendb(root_str);//send Byte
    ***************/
       QString topic="/odom";
      // QString type="";
       unsubscribe(topic);
}

void MainWindow::on_pushButton_43_clicked()
{
    /*****************
    //创建JSON对象
       QJsonObject root_Obj;
       //添加键值对，值的类型自动识别，顺序不可自定义
       root_Obj.insert("op", "unsubscribe");//停止获取GPS数据
       root_Obj.insert("topic", "/Fix");


       //创建Json文档
       QJsonDocument root_Doc;
       root_Doc.setObject(root_Obj);
       QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
       //QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
       //QString strJson(root_str);
       //ui->client_msg->setText(strJson);
       ui->client_msg->setText(root_str);
       emit C_sendb(root_str);//send Byte
   ****************/
       QString topic="/Fix";
      // QString type="";
       unsubscribe(topic);
}

void MainWindow::on_pushButton_44_clicked()
{
    //创建JSON对象
       QJsonObject root_Obj;
       //添加键值对，值的类型自动识别，顺序不可自定义
       root_Obj.insert("op", "unsubscribe");//停止获取电池数据
       root_Obj.insert("topic", "/battery_state");


       //创建Json文档
       QJsonDocument root_Doc;
       root_Doc.setObject(root_Obj);

       QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
       //QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
       //QString strJson(root_str);
       //ui->client_msg->setText(strJson);
       ui->client_msg->setText(root_str);
       emit C_sendb(root_str);//send Byte

       QString topic="/battery_state";
      // QString type="";
       unsubscribe(topic);
}


void MainWindow::on_pushButton_32_clicked()
{
    //创建JSON对象
    QJsonObject root_Obj;

/*************
    struct timespec time1;
    clock_gettime(CLOCK_REALTIME, &time1);
    //printf( %d, %ld",);
    qDebug()<<"CLOCK_REALTIME:"<<time1.tv_sec<<","<<time1.tv_nsec;
*************/
    int sec,usec;
    struct timeval us;
    gettimeofday(&us,NULL);
    sec=us.tv_sec;
    usec=us.tv_usec;
    double timestamp=sec;
    double ff=usec/1000000.0;
    timestamp=sec+ff;
    //qDebug()<<"gettimeofday: "<<ff<<sec<<QString::number(timestamp, 'f', 6) ;

    if(!posset.bsetpos)return;
    else posset.bsetpos=false;
    /***************
    'header': {
                   'seq': 1,
                   'stamp': datetime.now().timestamp(),
                   'frame_id': 'map'
               },
    ***************/
    QJsonObject header;
    header.insert("seq",1);
    header.insert("stamp",timestamp);
    header.insert("frame_id","map");

    QJsonObject position;
    position.insert("x", posset.px);
    position.insert("y", posset.py);
    position.insert("z", posset.pz);

    QJsonObject orientation;
    orientation.insert("x", posset.ox);
    orientation.insert("y", posset.oy);
    orientation.insert("z", posset.oz);
    orientation.insert("w", posset.ow);

    QJsonObject pose;
    pose.insert("position", position);
    pose.insert("orientation", orientation);

   // QJsonObject pose1;
   // pose1.insert("pose",pose);


    QJsonObject msg;
    msg.insert("header",header);
    msg.insert("pose",pose);

    QString topic="/move_base_simple/goal";
    publish(topic,msg);

/********************
    root_Obj.insert("op","publish");
    root_Obj.insert("topic","/move_base_simple/goal");//发送目标点
    root_Obj.insert("msg",msg);

    //创建Json文档
    QJsonDocument root_Doc;
    root_Doc.setObject(root_Obj);


    QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
    QByteArray root_strdisp = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
    //QString strJson(root_str);
    //ui->client_msg->setText(strJson);
    ui->client_msg->setText(root_strdisp);

   emit C_sendb(root_str);//send Byte
**********************/
}

void MainWindow::on_pushButton_45_clicked()
{
    static bool disp=true;
    if(disp)
    {
        ui->groupBox->show();
        ui->widget_2->show();
        ui->widget_3->show();
    }
    else
    {
        ui->groupBox->hide();
        ui->widget_2->hide();
        ui->widget_3->hide();
    }
    disp=!disp;

}

void MainWindow::on_pushButton_33_clicked()
{
    //创建JSON对象
    QJsonObject root_Obj;

/*************
    struct timespec time1;
    clock_gettime(CLOCK_REALTIME, &time1);
    //printf( %d, %ld",);
    qDebug()<<"CLOCK_REALTIME:"<<time1.tv_sec<<","<<time1.tv_nsec;
*************/
    int sec,usec;
    struct timeval us;
    gettimeofday(&us,NULL);
    sec=us.tv_sec;
    usec=us.tv_usec;
    double timestamp=sec;
    double ff=usec/1000000.0;
    timestamp=sec+ff;
    //qDebug()<<"gettimeofday: "<<ff<<sec<<QString::number(timestamp, 'f', 6) ;

    if(!posset.bsetpos)return;
    else posset.bsetpos=false;
    /***************
    'header': {
                   'seq': 1,
                   'stamp': datetime.now().timestamp(),
                   'frame_id': 'map'
               },
    ***************/
    QJsonObject header;
    header.insert("seq",1);
    header.insert("stamp",timestamp);
    header.insert("frame_id","map");

    QJsonObject position;
    position.insert("x", posset.px);
    position.insert("y", posset.py);
    position.insert("z", posset.pz);

    QJsonObject orientation;
    orientation.insert("x", posset.ox);
    orientation.insert("y", posset.oy);
    orientation.insert("z", posset.oz);
    orientation.insert("w", posset.ow);

    QJsonObject pose;
    pose.insert("position", position);
    pose.insert("orientation", orientation);

    QJsonObject pose1;
    pose1.insert("pose",pose);


    QJsonObject msg;
    msg.insert("header",header);
    msg.insert("pose",pose1);

    QString topic="/initialpose";
    publish(topic,msg);

/***************
    root_Obj.insert("op","publish");
    root_Obj.insert("topic","/initialpose");//发送初始点
    root_Obj.insert("msg",msg);

    //创建Json文档
    QJsonDocument root_Doc;
    root_Doc.setObject(root_Obj);


    QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
    QByteArray root_strdisp = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
    //QString strJson(root_str);
    //ui->client_msg->setText(strJson);
    ui->client_msg->setText(root_strdisp);

   emit C_sendb(root_str);//send Byte
***********************/
}

void MainWindow::on_Cclear_clicked()
{
    ui->client_rec->clear();
}

void MainWindow::on_pushButton_46_clicked()
{
    ui->stackedWidget->setCurrentWidget(dlgauto);
    ui->pushButton_8->setStyleSheet(button_style1);//man
    ui->pushButton_3->setStyleSheet(button_style1);
    ui->pushButton_46->setStyleSheet(button_style2);//auto
    ui->pushButton_47->setStyleSheet(button_style1);
    ui->pushButton_48->setStyleSheet(button_style1);
    ui->stackedWidget->show();
    runmodel=1;
    ui->line->hide(); //test边框
    ui->line_2->hide();
    ui->line_3->hide();
    ui->line_4->hide();
    ui->pushButtonjosn->hide();
    ui->widget_4->hide();
}

void MainWindow::on_pushButton_47_clicked()
{
    //ui->stackedWidget->setCurrentWidget(dlgman);
    ui->pushButton_8->setStyleSheet(button_style1);//man
    ui->pushButton_3->setStyleSheet(button_style1);
    ui->pushButton_46->setStyleSheet(button_style1);
    ui->pushButton_47->setStyleSheet(button_style2);//tst
    ui->pushButton_48->setStyleSheet(button_style1);
    ui->stackedWidget->hide();
    runmodel=3;//0:手动，1：自动，2：设置，3：调试
    ui->line->show(); //test边框
    ui->line_2->show();
    ui->line_3->show();
    ui->line_4->show();
    ui->pushButtonjosn->show();
    ui->widget_4->show();
}

void MainWindow::on_pushButton_48_clicked()
{
   // ui->stackedWidget->setCurrentWidget(dlgman);
    ui->pushButton_8->setStyleSheet(button_style1);//man
    ui->pushButton_3->setStyleSheet(button_style1);
    ui->pushButton_46->setStyleSheet(button_style1);
    ui->pushButton_47->setStyleSheet(button_style1);
    ui->pushButton_48->setStyleSheet(button_style2);//exit
    ui->stackedWidget->show();
    ui->pushButtonjosn->hide();
    ui->widget_4->hide();

    switch(QMessageBox::question(this, "提示", "是否停止程序运行？", QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok))
    {
      case QMessageBox::Ok:
            break;
      case QMessageBox::Cancel:
            return;

    }

    close();
}

void MainWindow::on_pushButtonjosn_clicked()
{
    /********************8888
    {
        "Company": "Digia",
        "From": 1991,
        "Name": "Qt",
        "Page": {
            "Developers": "https://www.qt.io/developers/",
            "Download": "https://www.qt.io/download/",
            "Home": "https://www.qt.io/"},
        "Version": [
            4.8,
            5.2,
            5.7]
    }
    ***********************/

     /**********************
       // 构建 Json 数组 - Version
        QJsonArray versionArray;
        versionArray.append(4.8);
        versionArray.append(5.2);
        versionArray.append(5.7);

        // 构建 Json 对象 - Page
        QJsonObject pageObject;
        pageObject.insert("Home", "https://www.qt.io/");
        pageObject.insert("Download", "https://www.qt.io/download/");
        pageObject.insert("Developers", "https://www.qt.io/developers/");

        // 构建 Json 对象
        QJsonObject json;
        json.insert("Name", "Qt");
        json.insert("Company", "Digia");
        json.insert("From", 1991);
        json.insert("Version", QJsonValue(versionArray));
        json.insert("Page", QJsonValue(pageObject));

        // 构建 Json 文档
        QJsonDocument document;
        document.setObject(json);
        QByteArray byteArray = document.toJson(QJsonDocument::Compact);
        QString strJson(byteArray);
            //解析JSON
        QJsonParseError jsonError;
        QJsonDocument doucment = QJsonDocument::fromJson(byteArray, &jsonError);
        if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError))
        {
            if (doucment.isObject())
            {
                QJsonObject object = doucment.object();
                if (object.contains("Name"))
                {
                    QJsonValue value = object.value("Name");
                    if (value.isString())
                    {
                        QString strName = value.toString();
                        qDebug() << "Name : " << strName;
                    }
                }
                if (object.contains("Company"))
                {
                    QJsonValue value = object.value("Company");
                    if (value.isString())
                    {
                        QString strCompany = value.toString();
                        qDebug() << "Company : " << strCompany;
                    }
                }
                if (object.contains("From"))
                {
                    QJsonValue value = object.value("From");
                    if (value.isDouble())
                    {
                        int nFrom = value.toVariant().toInt();
                        qDebug() << "From : " << nFrom;
                    }
                }
                if (object.contains("Version"))
                {
                    QJsonValue value = object.value("Version");
                    if (value.isArray())
                    {
                        QJsonArray array = value.toArray();
                        int nSize = array.size();
                        for (int i = 0; i < nSize; ++i)
                        {
                            QJsonValue value = array.at(i);
                            if (value.isDouble())
                            {
                                double dVersion = value.toDouble();
                                qDebug() << "Version : " << dVersion;
                            }
                        }
                    }
                }
                if (object.contains("Page"))
                {
                    QJsonValue value = object.value("Page");
                    if (value.isObject())
                    {
                        QJsonObject obj = value.toObject();
                        if (obj.contains("Home"))
                        {
                            QJsonValue value = obj.value("Home");
                            if (value.isString())
                            {
                                QString strHome = value.toString();
                                qDebug() << "Home : " << strHome;
                            }
                        }
                        if (obj.contains("Download"))
                        {
                            QJsonValue value = obj.value("Download");
                            if (value.isString())
                            {
                                QString strDownload = value.toString();
                                qDebug() << "Download : " << strDownload;
                            }
                        }
                        if (obj.contains("Developers"))
                        {
                            QJsonValue value = obj.value("Developers");
                            if (value.isString())
                            {
                                QString strDevelopers = value.toString();
                                qDebug() << "Developers : " << strDevelopers;
                            }
                        }
                    }
                }
            }
        }
        ***************/

        /********************
        {
        "msg": {
        "header": {
        "frame_id": "",
        "seq": 241,
        "stamp": {
        "nsecs": 544486443,
        "secs": 1635384830
        }
        },
        "status_list": [[1，0，"wait"],[1,1,"Nav"],[1,3,"arrive"],[1,4,"nonom"]
        ]
        },
        "op": "publish",
        "topic": "/move_base/status"
        }
        ************************/
           // QJsonArray wait = {1,0,"wait"};
           // QJsonArray nav = {1,1,"nav"};
           // QJsonArray arr = {1,3,"arrive"};
           // QJsonArray nom = {1,4,"nonom"};

            QJsonObject wait;
            wait.insert("goal_id","01");
            wait.insert("status",0);
            wait.insert("text","wait");

            QJsonObject nav;
            nav.insert("goal_id","01");
            nav.insert("status",1);
            nav.insert("text","nav");

            QJsonObject arrive;
            arrive.insert("goal_id","01");
            arrive.insert("status",3);
            arrive.insert("text","arrive");

            QJsonObject nomal;
            nomal.insert("goal_id","01");
            nomal.insert("status",4);
            nomal.insert("text","nomal");


            QJsonArray status_list;    //数组内嵌套了5个数组
            status_list.insert(0, wait);
            status_list.insert(1, nav);
            status_list.insert(2, arrive);
            status_list.insert(3, nomal);

            QJsonObject stamp;
            stamp.insert("nsecs", 544486443);
            stamp.insert("secs", 1635384830);

            QJsonObject header;
            header.insert("frame_id", "");
            header.insert("seq", 241);
            header.insert("stamp",stamp);

            QJsonObject msg;
            msg.insert("header",header);
            msg.insert("status_list",status_list);

            QJsonObject json;
            json.insert("msg",msg);
            json.insert("op","publish");
            json.insert("topic","/move_base/status");

            // 构建 Json 文档
            QJsonDocument document;
            document.setObject(json);
            QByteArray byteArray = document.toJson(QJsonDocument::Compact);
            QString strJson(byteArray);
                //解析JSON
            QJsonParseError jsonError;
            QJsonDocument doucment = QJsonDocument::fromJson(byteArray, &jsonError);
            if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError))
            {
                if (doucment.isObject())
                {
                    QJsonObject object = doucment.object();
                    if (object.contains("op")&&object.contains("topic"))
                    {
                        QString opvalue = object.value("op").toString();
                        QString topicvalue = object.value("topic").toString();

                        if (opvalue=="publish"&&topicvalue=="/move_base/status")
                        {
                           QJsonObject msgobj = object.value("msg").toObject();
                           /* data键 */
                           QJsonValue stVaule = msgobj.value("status_list");
                           if(stVaule.isArray())
                           {
                               for(int idx = 0; idx <= 3; idx++)
                               {
                                   QJsonObject data_Obj = stVaule.toArray().at(idx).toObject();
                                   QString add = "id：" + data_Obj.value("goal_id").toString();

                                   int aqi =data_Obj.value("status").toInt();
                                   QString qsaqi=" status：" +QString::number(aqi);
                                   QString lv = " text：" + data_Obj.value("text").toString();

                                   qDebug() << add + qsaqi + lv ;
                               }
                           }

                           // QString strName = value.toString();
                           // qDebug() << "Name : " << strName;
                        }
                    }
                    if (object.contains("Company"))
                    {
                        QJsonValue value = object.value("Company");
                        if (value.isString())
                        {
                            QString strCompany = value.toString();
                            qDebug() << "Company : " << strCompany;
                        }
                    }
                    if (object.contains("From"))
                    {
                        QJsonValue value = object.value("From");
                        if (value.isDouble())
                        {
                            int nFrom = value.toVariant().toInt();
                            qDebug() << "From : " << nFrom;
                        }
                    }
                    if (object.contains("Version"))
                    {
                        QJsonValue value = object.value("Version");
                        if (value.isArray())
                        {
                            QJsonArray array = value.toArray();
                            int nSize = array.size();
                            for (int i = 0; i < nSize; ++i)
                            {
                                QJsonValue value = array.at(i);
                                if (value.isDouble())
                                {
                                    double dVersion = value.toDouble();
                                    qDebug() << "Version : " << dVersion;
                                }
                            }
                        }
                    }
                    if (object.contains("Page"))
                    {
                        QJsonValue value = object.value("Page");
                        if (value.isObject())
                        {
                            QJsonObject obj = value.toObject();
                            if (obj.contains("Home"))
                            {
                                QJsonValue value = obj.value("Home");
                                if (value.isString())
                                {
                                    QString strHome = value.toString();
                                    qDebug() << "Home : " << strHome;
                                }
                            }
                            if (obj.contains("Download"))
                            {
                                QJsonValue value = obj.value("Download");
                                if (value.isString())
                                {
                                    QString strDownload = value.toString();
                                    qDebug() << "Download : " << strDownload;
                                }
                            }
                            if (obj.contains("Developers"))
                            {
                                QJsonValue value = obj.value("Developers");
                                if (value.isString())
                                {
                                    QString strDevelopers = value.toString();
                                    qDebug() << "Developers : " << strDevelopers;
                                }
                            }
                        }
                    }
                }
            }









            //QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
           // QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
            //QString strJson(root_str);

           // ui->client_msg->setText(strJson);


}

void MainWindow::on_pushButtonnavst_clicked()
{
    /*********************
    //创建JSON对象
       QJsonObject root_Obj;
       //添加键值对，值的类型自动识别，顺序不可自定义
       root_Obj.insert("op", "subscribe");//获取导航状态
       root_Obj.insert("topic", "/move_base/status");


       //创建Json文档
       QJsonDocument root_Doc;
       root_Doc.setObject(root_Obj);

       QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
       //QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
       //QString strJson(root_str);
       //ui->client_msg->setText(strJson);
       ui->client_msg->setText(root_str);       
       emit C_sendb(root_str);//send Byte
   ************/
       QString topic="/move_base/status";
       QString type="";
       subscribe(topic,type);

       ui->frame_2->show();
}

void MainWindow::on_pushButton_51_clicked()
{
    /**************
    //创建JSON对象
       QJsonObject root_Obj;
       //添加键值对，值的类型自动识别，顺序不可自定义

       QJsonObject msg;
       msg.insert("msg", 0); //0:启动，/1，关闭

       root_Obj.insert("op", "call_service");//IMU复位
       root_Obj.insert("service", "/robuster/mapping/control");
       root_Obj.insert("args", msg);


       //创建Json文档
       QJsonDocument root_Doc;
       root_Doc.setObject(root_Obj);
       QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
       //QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
       //QString strJson(root_str);
       //ui->client_msg->setText(strJson);
       ui->client_msg->setText(root_str);
       emit C_sendb(root_str);//send Byte
       ****************/
    QJsonObject msg;
    msg.insert("msg", 0); //0:启动，/1，关闭
    QString service="/robuster/mapping/control";
    callservice(service, msg);
}

void MainWindow::on_pushButton_52_clicked()
{
    /*****************
    //创建JSON对象
       QJsonObject root_Obj;
       //添加键值对，值的类型自动识别，顺序不可自定义

       QJsonObject msg;
       msg.insert("msg", 0); //0:启动，1，关闭

       root_Obj.insert("op", "call_service");//关闭导航
       root_Obj.insert("service", "/robuster/navigation/control");
       root_Obj.insert("args", msg);


       //创建Json文档
       QJsonDocument root_Doc;
       root_Doc.setObject(root_Obj);
       QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
       //QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
       //QString strJson(root_str);
       //ui->client_msg->setText(strJson);
       ui->client_msg->setText(root_str);
       emit C_sendb(root_str);//send Byte
       ****************/

       QJsonObject msg;
       msg.insert("msg", 0);
       QString service="/robuster/navigation/control";
       callservice(service, msg);
}

void MainWindow::on_pushButtonnavst_2_clicked()
{
    /*****************
    //创建JSON对象
       QJsonObject root_Obj;
       //添加键值对，值的类型自动识别，顺序不可自定义
       root_Obj.insert("op", "unsubscribe");//获取导航状态
       root_Obj.insert("topic", "/move_base/status");


       //创建Json文档
       QJsonDocument root_Doc;
       root_Doc.setObject(root_Obj);

       QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
       //QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
       //QString strJson(root_str);
       //ui->client_msg->setText(strJson);
       ui->client_msg->setText(root_str);
       ui->frame_2->hide();
      emit C_sendb(root_str);//send Byte
  ********************/
       QString topic="/move_base/status";
       //QString type="";
       unsubscribe(topic);

}

void MainWindow::on_pushButtonpos_clicked()
{
   // Dialogpose *dlg=new Dialogpose(this);
    dlgpose->show();
}

void MainWindow::adverise(QString topic,QString type)//创建发布主题
{
    //创建JSON对象
       QJsonObject root_Obj;
       //添加键值对，值的类型自动识别，顺序不可自定义
       root_Obj.insert("op", "adverise");//
       root_Obj.insert("topic", topic);
       root_Obj.insert("type", type);

       //创建Json文档
       QJsonDocument root_Doc;
       root_Doc.setObject(root_Obj);
       QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
       QByteArray root_strdisp = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
       //QString strJson(root_str);
       //ui->client_msg->setText(strJson);
       ui->client_msg->setText(root_strdisp);

       emit C_sendb(root_str);//send Byte

}
void MainWindow::unadverise(QString topic)//停止发布主题
{
    //创建JSON对象
       QJsonObject root_Obj;
       //添加键值对，值的类型自动识别，顺序不可自定义
       root_Obj.insert("op", "adverise");//
       root_Obj.insert("topic", topic);
      // root_Obj.insert("type", type);

       //创建Json文档
       QJsonDocument root_Doc;
       root_Doc.setObject(root_Obj);
       QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
       QByteArray root_strdisp = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
       //QString strJson(root_str);
       //ui->client_msg->setText(strJson);
       ui->client_msg->setText(root_strdisp);
       emit C_sendb(root_str);//send Byte
}
void MainWindow::subscribe(QString topic,QString type)//订阅主题
{
       //创建JSON对象
       QJsonObject root_Obj;
       //添加键值对，值的类型自动识别，顺序不可自定义
       root_Obj.insert("op", "subscribe");//获取地图数据
       root_Obj.insert("topic", topic);
       if(type!="")root_Obj.insert("type", type);
       root_Obj.insert("throttle_rate", 1);

       //创建Json文档
       QJsonDocument root_Doc;
       root_Doc.setObject(root_Obj);
       QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
       QByteArray root_strdisp = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
       //QString strJson(root_str);
       //ui->client_msg->setText(strJson);
       ui->client_msg->setText(root_strdisp);
       emit C_sendb(root_str);//send Byte

}
void MainWindow::unsubscribe(QString topic)//取消订阅主题
{
    //创建JSON对象
    QJsonObject root_Obj;
    //添加键值对，值的类型自动识别，顺序不可自定义
    root_Obj.insert("op", "unsubscribe");//获取地图数据
    root_Obj.insert("topic", topic);
    //root_Obj.insert("type", type);

    //创建Json文档
    QJsonDocument root_Doc;
    root_Doc.setObject(root_Obj);
    QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
    QByteArray root_strdisp = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
    //QString strJson(root_str);
    //ui->client_msg->setText(strJson);
    ui->client_msg->setText(root_strdisp);
    emit C_sendb(root_str);//send Byte

}
void MainWindow::publish(QString topic,QJsonObject msg)//在指定主题上发布消息
{
    //创建JSON对象
       QJsonObject root_Obj;

       //添加键值对，值的类型自动识别，顺序不可自定义
       root_Obj.insert("op", "publish");//
       root_Obj.insert("topic", topic);
       root_Obj.insert("msg", msg);


       //创建Json文档
       QJsonDocument root_Doc;
       root_Doc.setObject(root_Obj);
       QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
       QByteArray root_strdisp = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
       //QString strJson(root_str);
       //ui->client_msg->setText(strJson);
       ui->client_msg->setText(root_strdisp);
       emit C_sendb(root_str);//send Byte

}
void MainWindow::callservice(QString service,QJsonObject msg)//调用指定的服务
{
    //创建JSON对象
       QJsonObject root_Obj;
       //添加键值对，值的类型自动识别，顺序不可自定义

       root_Obj.insert("op", "call_service");
       root_Obj.insert("service", service);
       root_Obj.insert("args", msg);

       //创建Json文档
       QJsonDocument root_Doc;
       root_Doc.setObject(root_Obj);
       QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
       QByteArray root_strdisp = root_Doc.toJson(QJsonDocument::Indented);   //标准JSON格式    QString strJson(root_str);
       //QString strJson(root_str);
       //ui->client_msg->setText(strJson);
       ui->client_msg->setText(root_strdisp);
       emit C_sendb(root_str);//send Byte
}

void MainWindow::on_pushButtonpos_2_clicked()
{
    QString topic="/robot_pose";
    QString type="geometry_msgs/Pose";
    subscribe(topic,type);
}

void MainWindow::on_pushButtonpos_3_clicked()
{
    QString topic="/robot_pose";
   // QString type="geometry_msgs/Pose";
    unsubscribe(topic);
}

void MainWindow::on_pushButtonemit_clicked()
{
    rospos.x=1.0;//posobj.value("x").toDouble();
    rospos.y=2.2;//posobj.value("y").toDouble();
    rospos.z=3.3;//posobj.value("z").toDouble();
    rospos.x1=4.4;//oriobj.value("x").toDouble();
    rospos.y1=5.5;//oriobj.value("y").toDouble();
    rospos.z1=6.6;//oriobj.value("z").toDouble();
    rospos.w1=7.7;//oriobj.value("w").toDouble();
    emit Initpos();
}
