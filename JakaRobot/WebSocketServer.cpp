#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include "WebSocketServer.h"
#include <QDebug>
#include <QWebSocket>
 
WSServer::WSServer(QObject *parent)
    : QObject(parent)
{
    WebSocketServer = new QWebSocketServer("LocalServer", QWebSocketServer::NonSecureMode, this);
    connect(WebSocketServer, &QWebSocketServer::newConnection, this, &WSServer::HasNewConnection);

    connect(WebSocketServer, SIGNAL(serverError(QWebSocketProtocol::CloseCode)),
            this             , SLOT(on_serverError(QWebSocketProtocol::CloseCode)));




}

WSServer::~WSServer()
{

}
 
void WSServer::Server_start(qint32 port)
{
    Server_Port = port;
    WebSocketServer->listen(QHostAddress(QHostAddress::Any), Server_Port);  //开启监听，任意地址，指定端口
}
 
void WSServer::Server_stop()
{
    foreach(QWebSocket* websocket, WSClients.values())
    {
        websocket->close(); //断开所有客户端连接
    }
    WebSocketServer->close();   //关闭服务
    WSClients.clear();  //清除哈希表
}

void WSServer::remove_client(const QString ClientInfo)
{
    if(WSClients.contains(ClientInfo))
    {
        WSClients.value(ClientInfo)->close();
        WSClients.remove(ClientInfo);   //移除指定客户端，相当于强制下线
    }
}
//服务端错误处理
void WSServer::on_serverError(QWebSocketProtocol::CloseCode error_code)
{
    emit occur_error("error message:server error," + WebSocketServer->errorString()
                     + "\nerror code:" + QString::number(error_code));
}
//客户端连接错误处理
void WSServer::on_clientError(QAbstractSocket::SocketError error_code)
{
    QWebSocket* error_client = qobject_cast<QWebSocket*>(sender());
    emit occur_error("error message:client error," + error_client->errorString()
                     + "\nerror code:" + QString::number(error_code));
}
 //有新的客户端接入，处理客户端的断开连接信号、错误信号、消息信号，将客户端加入哈希表管理
void WSServer::HasNewConnection()
{
    QWebSocket* NewClient = WebSocketServer->nextPendingConnection();
    connect(NewClient, &QWebSocket::disconnected, this, &WSServer::on_ClientDisconnected);
    connect(NewClient, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(on_clientError(QAbstractSocket::SocketError)));
    // 客户端发来消息时，既会触发textFrame，也会触发textMessage，此处只写text
    connect(NewClient, &QWebSocket::textMessageReceived, this, &WSServer::on_textMessageReceived);
    connect(NewClient, &QWebSocket::binaryMessageReceived, this, &WSServer::on_binaryMessageReceived);


    QString NewClientInfo = QString("%1-%2").arg(NewClient->peerAddress().toString()).arg(NewClient->peerPort());

    WSClients.insert(NewClientInfo, NewClient);

    emit NewClient_conncted(NewClientInfo);
}
 //客户端断开连接
void WSServer::on_ClientDisconnected()
{
    qDebug() <<__FILE__ << __LINE__ << __FUNCTION__;
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());
    if(!client)
    {
        return;
    }
    QString ClientInfo = QString("%1-%2").arg(client->peerAddress().toString()).arg(client->peerPort());
    emit Client_disconncted(ClientInfo);    //发出信号给到UI线程

    WSClients.remove(ClientInfo);   //移除客户端
}
 //接收客户端消息
void WSServer::on_textMessageReceived(const QString &message)
{
    //qDebug() <<__FILE__ << __LINE__ << __FUNCTION__;
    QWebSocket* client = qobject_cast<QWebSocket *>(sender());
    if(!client)
    {
        return;
    }
    QString ClientInfo = QString("%1-%2").arg(client->peerAddress().toString()).arg(client->peerPort());
    emit NewTextMessage(ClientInfo, message);   //发出信号给到UI线程
}

void WSServer::on_binaryMessageReceived(const QByteArray data)
{
   // qDebug() <<__FILE__ << __LINE__ << __FUNCTION__;
    QWebSocket* client = qobject_cast<QWebSocket *>(sender());
    if(!client)
    {
        return;
    }
    QString ClientInfo = QString("%1-%2").arg(client->peerAddress().toString()).arg(client->peerPort());
    emit NewBinaryMessage(ClientInfo, data);   //发出信号给到UI线程
}
//服务器发消息，UI界面可选择发至所有客户端或指定客户端
void WSServer::Server_sendmsg(QString ClientInfo, QString message, bool isAllClients)
{
   if(!isAllClients)
   {
       if(WSClients.contains(ClientInfo))
       {
           WSClients.value(ClientInfo)->sendTextMessage(message);
       }
   }
   else
   {
       foreach(QWebSocket* websocket, WSClients.values())
       {
           websocket->sendTextMessage(message);
       }
   }
}

void WSServer::Server_sendBinarymsg(QString ClientInfo, QByteArray data, bool isAllClients)
{
   if(!isAllClients)
   {
       if(WSClients.contains(ClientInfo))
       {
           WSClients.value(ClientInfo)->sendBinaryMessage(data);
       }
   }
   else
   {
       foreach(QWebSocket* websocket, WSClients.values())
       {
           websocket->sendBinaryMessage(data);
       }
   }
}
