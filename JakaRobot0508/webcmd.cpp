#include "webcmd.h"

Webcmd::Webcmd(QObject *parent) : QObject(parent)
{

}

void Webcmd::subcmd(QString topic,QString type)
{
    emit websub(topic,type);
}

void Webcmd::unsubcmd(QString topic)
{
    emit webunsub(topic);
}

void Webcmd::pubcmd(QString topic,QJsonObject msg)
{
    emit webpub(topic,msg);
}
void Webcmd::sercmd(QString service,QJsonObject msg)
{
    emit webser(service,msg);
}
void Webcmd::advcmd(QString topic,QString type)
{
    emit webadv(topic,type);
}
void Webcmd::unadvcmd(QString topic)
{
    emit webunadv(topic);
}
