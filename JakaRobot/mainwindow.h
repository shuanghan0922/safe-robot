#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//#include "JAKAZuRobot.h"
#include <QMainWindow>
#include "dialoggl.h"
#include "dialogtest.h"
#include "dialogmanual.h"
#include "dialogauto.h"
#include "dialogpose.h"
#include "dialogjakamovetest.h"
#include "WebSocketClient.h"
#include "WebSocketServer.h"
#include <QThread>
#include <QMessageBox>
#include <QDateTime>
#include <QListWidgetItem>
//#include "ComData.h"
//#include "webcmd.h"
#include <QKeyEvent>
//#include "dialogvideo.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //JAKAZuRobot test;
    void KeyPressEvent(QKeyEvent *ev);
    void move(double vl,double va);
    QTimer *timerstu;
    //QTimer *timerroscmd;
    QString button_style1;
    QString button_style2;
    void paintEvent(QPaintEvent *);

    void adverise(QString topic,QString type);//创建发布主题
    void unadverise(QString topic);//停止发布主题
    void subscribe(QString topic,QString type);//订阅主题
    void unsubscribe(QString topic);//取消订阅主题
    void publish(QString topic,QJsonObject msg);//在指定主题上发布消息
    void callservice(QString service,QJsonObject msg);//调用指定的服务


public slots:
    //客户端
    void C_recive(const QString message);
    void C_reciveb(const QByteArray data);//QString message);
    void C_connection(const QString msg, const bool state);
    void C_error(const QString msg);
    //服务端
    void S_recive(const QString ClientInfo, const QString message);
    void S_reciveb(const QString ClientInfo, const QByteArray data);
    void add_client(const QString ClientInfo);
    void remove_client(const QString ClientInfo);
    void S_error(const QString error_message);
    //接收手动开关信号
    void Man_Rosmove(QString id);
    //rec webcmd
    void web_sub(QString topic,QString type);
    void web_unsub(QString topic);
    void web_adv(QString topic,QString type);
    void web_unadv(QString topic);
    void web_pub(QString topic,QJsonObject msg);
    void web_ser(QString service,QJsonObject msg);

signals:
    //to客户端
    void C_connect(const QString url);
    void C_disconnect();
    void C_send(const QString message);
    void C_sendb(const QByteArray data);
    //to服务端
    void S_start(const qint32 port = 10086);
    void S_stop();
    void S_send(const QString ClientInfo, const QString message, const bool isAllClients);
    void S_sendb(const QString ClientInfo, const QByteArray data, const bool isAllClients);
    void S_remove(const QString ClientInfo);
    void Initpos();

private slots:
    void dispstu();
    void sendroscmd();
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_client_connect_clicked();

    void on_client_disconnect_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_23_clicked();

    void on_pushButton_24_clicked();

    void on_pushButton_25_clicked();

    void on_pushButton_34_clicked();

    void on_pushButton_35_clicked();

    void on_pushButton_36_clicked();

    void on_pushButton_37_clicked();

    void on_pushButton_38_clicked();

    void on_pushButton_26_clicked();

    void on_pushButton_27_clicked();

    void on_pushButton_39_clicked();

    void on_pushButton_22_clicked();

    void on_pushButton_28_clicked();

    void on_pushButton_30_clicked();

    void on_pushButton_29_clicked();

    void on_pushButton_31_clicked();

    void on_pushButton_40_clicked();

    void on_pushButton_41_clicked();

    void on_pushButton_42_clicked();

    void on_pushButton_43_clicked();

    void on_pushButton_44_clicked();

    void on_pushButton_32_clicked();

    void on_pushButton_45_clicked();

    void on_pushButton_33_clicked();

    void on_Cclear_clicked();

    void on_pushButton_46_clicked();

    void on_pushButton_47_clicked();

    void on_pushButton_48_clicked();

    void on_pushButtonjosn_clicked();

    void on_pushButtonnavst_clicked();

    void on_pushButton_51_clicked();

    void on_pushButton_52_clicked();

    void on_pushButtonnavst_2_clicked();

    void on_pushButtonpos_clicked();

    void on_pushButtonpos_2_clicked();

    void on_pushButtonpos_3_clicked();

    void on_pushButtonemit_clicked();

    void on_pushButtonvideo_clicked();

private:
    Ui::MainWindow *ui;
    Dialoggl *dlggl;
    Dialogtest *dlgtest;
    DialogManual *dlgman;
    Dialogauto *dlgauto;
    Dialogpose *dlgpose;
    //DialogVideo *dlgvideo;
   // Dialogequset *dlgequ;
    DialogJakamovetest *dlgmove;

    pthread_t pthjaka;
    pthread_t pthros;
    pthread_t pthbutton;
    pthread_t pthprocess;

    WSClient* wsclient;
   // Webcmd *webcmd;
    QThread* client_thread;
    WSServer* wsserver;
    QThread* server_thread;
    QString Clientcur;

    QLabel *statusLabel1p; //声明两个标签对象，用于显示状态信息
    QLabel* statusLabel1t;
    QLabel* statusLabel2p; //声明两个标签对象，用于显示状态信息
    QLabel* statusLabel2t;
    QLabel* statusLabel3p; //声明两个标签对象，用于显示状态信息
    QLabel* statusLabel3t;
    QLabel* statusLabel4p; //声明两个标签对象，用于显示状态信息
    QLabel* statusLabel4t;
    QLabel* statusLabel5p; //声明两个标签对象，用于显示状态信息
    QLabel* statusLabel5t;
    QLabel* statusLabel6p; //声明两个标签对象，用于显示状态信息
    QLabel* statusLabel6t;
    void init_statusBar(); //初始化状态栏
    QString receiverFileName;//接收文件名
    bool brecvfile;//允许接受文件
 };
#endif // MAINWINDOW_H
