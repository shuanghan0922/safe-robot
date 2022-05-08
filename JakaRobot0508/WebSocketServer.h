#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <QObject>
#include <QHostAddress>
#include <QWebSocketServer>
 
class WSServer : public QObject
{
    Q_OBJECT
public:
    explicit WSServer(QObject *parent = nullptr);
    ~WSServer();

signals:
    void NewClient_conncted(const QString client_info);
    void Client_disconncted(const QString client_info);

    void NewTextMessage(const QString ClientInfo, const QString message);
    void NewBinaryMessage(const QString ClientInfo, const QByteArray data);
    void occur_error(const QString errorString);
 
public slots:
    void Server_start(const qint32 port = 10086);
    void Server_stop();
    void Server_sendmsg(const QString ClientInfo, const QString message, const bool isAllClients);
    void Server_sendBinarymsg(const QString ClientInfo, const QByteArray data, const bool isAllClients);

    void remove_client(const QString ClientInfo);
protected slots:
    void HasNewConnection();
    void on_ClientDisconnected();
    void on_textMessageReceived(const QString &message);
    void on_binaryMessageReceived(const QByteArray data);
    void on_serverError(QWebSocketProtocol::CloseCode error_code);
    void on_clientError(QAbstractSocket::SocketError error_code);

private:
    QWebSocketServer* WebSocketServer;
    QHash<QString, QWebSocket*> WSClients;
    QHostAddress Server_Address;
    qint32 Server_Port;
};
#endif // WEBSOCKETSERVERMANAGER_H
