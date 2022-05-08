#ifndef WEBCMD_H
#define WEBCMD_H

#include <QObject>
#include <QJsonObject>

class Webcmd : public QObject
{
    Q_OBJECT
public:
    explicit Webcmd(QObject *parent = nullptr);
    QString topic,type;
    QString service;
    QJsonObject msg;
    void subcmd(QString topic,QString type);
    void unsubcmd(QString topic);
    void pubcmd(QString topic,QJsonObject msg);
    void sercmd(QString service,QJsonObject msg);
    void advcmd(QString topic,QString type);
    void unadvcmd(QString topic);
signals:
    void websub(QString topic,QString type);
    void webunsub(QString topic);
    void webpub(QString topic,QJsonObject msg);
    void webser(QString service,QJsonObject msg);
    void webadv(QString topic,QString type);
    void webunadv(QString topic);
};

#endif // WEBCMD_H
