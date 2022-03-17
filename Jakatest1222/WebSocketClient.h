#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include <QObject>
#include <QWebSocket>
 
class WSClient : public QObject
{
    Q_OBJECT
public:
    explicit WSClient(QObject *parent = nullptr);
    ~WSClient();
 
signals:
    void connection_state(const QString msg, const bool state);
    void occur_error(const QString errorString);
    void NewTextMessage(const QString message);
    void NewBinaryMessage(const QByteArray data);//QString message);
 
public slots:
    void ConnectTo(const QString url);
    void Disconnect();
    void sendTextMessage(const QString message);
    void sendBinaryMessage(const QByteArray data);
 
protected slots:
    void on_connected();
    void on_disconnected();
    void on_error(QAbstractSocket::SocketError error);
    void on_textMessageReceived(const QString &message);
    void on_binaryMessageReceived(const QByteArray data);//QString &message);
 
private:
    QWebSocket* WSClientObj;
};
 
#endif
