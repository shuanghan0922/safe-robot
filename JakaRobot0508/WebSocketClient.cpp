#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include "WebSocketClient.h"
#include <QDebug>
#include <QJsonDocument>
//#include <QJsonArray>
#include <QJsonObject>
#include <unistd.h>

WSClient::WSClient(QObject *parent)
    : QObject(parent)
{
    WSClientObj = new QWebSocket;   //构造函数未设置参数和指定父对象
    WSClientObj->setParent(this);   //这里必须设置父对象，否则多线程运行报错
    //客户端需要处理的信号：连接成功、断开连接、发生错误、收到消息
    connect(WSClientObj, &QWebSocket::connected, this, &WSClient::on_connected);
    connect(WSClientObj, &QWebSocket::disconnected, this, &WSClient::on_disconnected);
    connect(WSClientObj, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(on_error(QAbstractSocket::SocketError)));
    connect(WSClientObj, &QWebSocket::textMessageReceived, this, &WSClient::on_textMessageReceived);
    connect(WSClientObj, &QWebSocket::binaryMessageReceived, this, &WSClient::on_binaryMessageReceived);
}

 
WSClient::~WSClient()
{

}
//连接服务端
void WSClient::ConnectTo(const QString url)
{
    WSClientObj->open(QUrl(url));
}
//断开连接
void WSClient::Disconnect()
{
    WSClientObj->close();
}
 //发消息，字符串
void WSClient::sendTextMessage(const QString message)
{
    WSClientObj->sendTextMessage(message);
}
 //发消息，二进制
void WSClient::sendBinaryMessage(const QByteArray data)
{
    WSClientObj->sendBinaryMessage(data);
}
 //连接成功，通知UI
void WSClient::on_connected()
{
    qDebug() << __FILE__ << __LINE__ << "connected";
    //qDebug() << __FILE__ << __LINE__ << "connected";
    //创建JSON对象
       QJsonObject root_Obj,root_Obj1;
       //添加键值对，值的类型自动识别，顺序不可自定义
       QJsonObject msg;
       msg.insert("msg", 1);
       QString service="/robuster/robuster_base/control";
       root_Obj.insert("op", "call_service");
       root_Obj.insert("service", service);
       root_Obj.insert("args", msg);
       //创建Json文档
       QJsonDocument root_Doc;
       root_Doc.setObject(root_Obj);
       QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
       sendBinaryMessage(root_str);//启动底盘驱动

       usleep(100000);
       service="/robuster/navigation/control";
       root_Obj1.insert("op", "call_service");
       root_Obj1.insert("service", service);
       root_Obj1.insert("args", msg);
       root_Doc.setObject(root_Obj1);
       QByteArray root_str1 = root_Doc.toJson(QJsonDocument::Compact);  //紧凑格式
       sendBinaryMessage(root_str1);//启动底盘驱动
       emit connection_state("success connected", true);   //true/false用于UI判断更新控件可用状态

    //emit connection_state("success connected", true);   //true/false用于UI判断更新控件可用状态
}
 //断开连接，通知UI
void WSClient::on_disconnected()
{
    qDebug() << __FILE__ << __LINE__ << "disconnected";
    emit connection_state("success disconnected", false);
}
 //报错，通知UI
void WSClient::on_error(QAbstractSocket::SocketError error)
{
    qDebug() << __FILE__ << __LINE__ << (int)error << WSClientObj->errorString();
    emit occur_error("error message:" + WSClientObj->errorString() + "\nerror code:" + QString::number(error));
}
 //接收消息，通知UI
void WSClient::on_textMessageReceived(const QString& message)
{
    emit NewTextMessage(message);
}

void WSClient::on_binaryMessageReceived(const QByteArray data) //QString& message)
{
    emit NewBinaryMessage(data);//message);
}
