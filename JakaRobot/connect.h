#ifndef CONNECT_H
#define CONNECT_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QObject>
#include <QMessageBox>
#include <QSqlError>
#include "ComData.h"

static bool createConnection()
{
    /**********MYSQL
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("/home/hfdxp/myprj/book");
    db.setUserName("hfdxp");
    db.setPassword("12345");
    ************************/

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    char fn[50];
    sprintf(fn,"%s/%s",dirpf,"testdt.db");
    db.setDatabaseName(fn);
    db.setPassword("123456");
    if(!db.open())
    {
       // QMessageBox::critical(0,qApp->tr("无法打开数据库"),qApp->tr("无法建立数据库连接!\n"),QMessageBox::Cancel);

        return false;
    }

    QSqlQuery query;
    query.exec("create table book(ID Interger primary key,Name varchar(20),Type varchar(20),Count Interger)");//test
    query.exec("create table syslog(tm varchar(20) primary key,user varchar(10),item varchar(20),demo varchar(100))");
    query.exec("create table alarm(id varchar(8),stm varchar(20),item varchar(40),type varchar(4),sol varchar(40),stu varchar(4),etm varchar(20),errtm Interger,gaptm Interger)");
    query.exec("create table user(ID varchar(16) primary key,name varchar(16),pw varchar(6),ctype varchar(10),type Interger)");
    query.exec("create table cppf(pfno varchar(10) primary key,name varchar(20),data Interger)");
    query.exec("create table productpara(name varchar(20) primary key,grow Interger,gcol Interger,row Interger,col  Interger,growspac REAL,gcolspac Interger,rowspac REAL, colspac REAL,length REAL,width REAL)");
    query.exec("create table product(tm varchar(20),picz varchar(40),chkz Interger)");
    query.exec("create table syspara(auto Interger,sl Interger,pp Interger,cy ,ur Interger,sf Interger,sj Interger,pfno varchar(10),pfname varchar(20),cpno varchar(10),cpname varchar(20),dayok Interger, daybj Interger,\
               dayop Interger,addplc varchar(16),portplc Interger,addsecs varchar(16),portsecs Interger,addpcplc varchar(16),addpcsecs varchar(16),addpcphoto varchar(16),stepdj REAL,stepupdj REAL,zthreshold REAL,\
               fthreshold REAL,dytm Interger,szunit Interger,kjunit Interger,sjposnum Interger,szmin Interger,kjmin Interger,ppdelay Interger,urruntm Interger, ppruntm Interger,yjhmruntm Interger,yjqsruntm Interger,\
               szruntm Interger,cqruntm Interger,cqqcruntm Interger,xlruntm Interger,szkg Interger,ppnum Interger,cullthreshold REAL,boxthreshold REAL,freshold1 REAL,freshold2 REAL,freshold3,REAL,\
               freshold41 REAL,freshold42 REAL,freshold43 REAL,freshold44 REAL)");
    query.exec("create table pflabel(ID varchar(30) primary key,num Interger,unit1 varchar(10),unit2 varchar(10))");
    query.exec("create table runtime(yy Interger,dd Interger,hh Interger,mm Interger,ss Interger,nm Interger,nhelp Interger,err Interger,tmerr Interger,xl Interger,mtbf Interger,mtba Interger,errtol Interger,mperh Interger)");
   // query.exec("create table runtimelog(tm varchar(20),yy Interger,dd Interger,hh Interger,mm Interger)");
    query.exec("create table bj(id varchar(8) primary key,stm varchar(20),item varchar(40),type varchar(4),sol varchar(40),stu varchar(4),etm varchar(20),errtm Interger,gaptm Interger)");
    query.exec("create table bjlabel(id varchar(8) primary key,item varchar(40),type varchar(4),sol varchar(40))");
    return true;

}

#endif // CONNECT_H

