#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include "WebSocketClient.h"
#include <QDebug>
 
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
    emit connection_state("success connected", true);   //true/false用于UI判断更新控件可用状态
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
