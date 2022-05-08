#include "dialoglogin.h"
#include "ui_dialoglogin.h"
#include <QMessageBox>
#include <QSqlQuery>
#include "ComData.h"
#include <QDateTime>

Dialoglogin::Dialoglogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialoglogin)
{
    ui->setupUi(this);
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);


}

Dialoglogin::~Dialoglogin()
{
    delete ui;
}

void Dialoglogin::on_pblogin_clicked()//确定
{
   int valid=0;
   QSqlQuery query;
   QString name0;
   QString pw0;
   QString id0;
   QString name =ui->lineEdit->text(); //从界面获取id的值
   QString pw=ui->lineEdit_2->text();
   static int lognum=0;

   if(lognum>2)
   {
      QMessageBox::warning(this,tr("警告"),tr("登录错误超过三次，退出登录!"),QMessageBox::Yes);
      lognum=0;
      close();
      return;
   }
if(name.isEmpty()||pw.isEmpty())
{
   QMessageBox::warning(this,tr("警告"),tr("请输入用户名和密码!"),QMessageBox::Yes);
    return;
}

//*********************
   query.exec("select * from user");
   while(query.next())//指向第一条记录
   {
   name0=query.value(0).toString();
   pw0=query.value(2).toString();

   if(!QString::compare(name,name0)&&!QString::compare(pw,pw0)) //查到该用户
   {
   valid=1;
   logid=name0;
   logname=query.value(1).toString();
   logright=query.value(4).toInt();
   blog=true;
   lognum=0;
   break;
   }
   else
   {
       valid=0;
       blog=false;
       logid="";
       logname="";
       logright=0;
   }
}


   if(valid) //如果合法
   {
       bool value;
       /**************************************
       QString tm=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
       QString item="登录";
       QString user=logid;
       QString demo="登录信息：用户名："+logid+"姓名："+logname;
     //  value=query.exec("INSERT INTO syslog VALUES('"+tm+"','"+logid+"','"+item+"','"+demo+"')");
       if(value)
       {
          // QMessageBox::information(this,"提示","登录操作记录成功！");
          // this->close();
       }
       else
       {
           QMessageBox::information(this,"提示","登录操作记录失败！");
          // this->close();
       }
       *******************************/
       accept();
   }
   else
   {
     bool value;
     QMessageBox::warning(this,tr("警告"),tr("用户名或密码错误!"),QMessageBox::Yes);
    //如果不正确，弹出警告对话框

    blog=false;
    lognum++;
    /**********************
    QString tm=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString item="登录失败";
    QString user=name;
    QString demo="登录信息：用户名："+name+"密码："+pw;
    value=query.exec("INSERT INTO syslog VALUES('"+tm+"','"+user+"','"+item+"','"+demo+"')");
    if(value)
    {
       // QMessageBox::information(this,"提示","登录操作记录成功！");
       // this->close();
    }
    else
    {
        QMessageBox::information(this,"提示","登录操作记录失败！");
       // this->close();
    }
    *******************/
   }



}

void Dialoglogin::on_pushButton_2_clicked()//重置
{
       ui->lineEdit->clear();
       ui->lineEdit_2->clear();
}

void Dialoglogin::on_Dialoglogin_destroyed()
{
    close();
}
