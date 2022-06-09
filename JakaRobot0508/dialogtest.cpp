#include "dialogtest.h"
#include "ui_dialogtest.h"
#include "ComData.h"
#include <QMessageBox>
#include <QInputDialog>

Dialogtest::Dialogtest(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialogtest)
{
    ui->setupUi(this);
}

Dialogtest::~Dialogtest()
{
    delete ui;
}

void Dialogtest::on_pushButton2p1_clicked()
{
    if(Jkstatus.is_socket_connect!=1)
    {
        QMessageBox::information(this,"提示","JAKA控制器连接失败，请检查！");
        return;
    }
    int res=JKrobot.program_load("nowairkeyup");
    if(res!=0)QMessageBox::information(this,"提示","2p合加载失败，请检查！");
    JKrobot.program_run();
}

void Dialogtest::on_pushButton2p0_clicked()
{
    if(Jkstatus.is_socket_connect!=1)
    {
        QMessageBox::information(this,"提示","JAKA控制器连接失败，请检查！");
        return;
    }
    int res=JKrobot.program_load("nowairkeydown");
    if(res!=0)QMessageBox::information(this,"提示","2p分加载失败，请检查！");
    JKrobot.program_run();
}

void Dialogtest::on_pushButton3p1_clicked()
{
    if(Jkstatus.is_socket_connect!=1)
    {
        QMessageBox::information(this,"提示","JAKA控制器连接失败，请检查！");
        return;
    }
    int res=JKrobot.program_load("nowairkeyup");
    if(res!=0)QMessageBox::information(this,"提示","3p合加载失败，请检查！");
    JKrobot.program_run();
}

void Dialogtest::on_pushButton3p0_clicked()
{
    if(Jkstatus.is_socket_connect!=1)
    {
        QMessageBox::information(this,"提示","JAKA控制器连接失败，请检查！");
        return;
    }
    int res=JKrobot.program_load("nowairkeydown");
    if(res!=0)QMessageBox::information(this,"提示","3p分加载失败，请检查！");
    JKrobot.program_run();
}

void Dialogtest::on_pushButton_clicked()
{
    if(Jkstatus.is_socket_connect!=1)
    {
        QMessageBox::information(this,"提示","JAKA控制器连接失败，请检查！");
        return;
    }
    int res=JKrobot.program_load("nowbutton");
    if(res!=0)QMessageBox::information(this,"提示","按钮加载失败，请检查！");
    JKrobot.program_run();
}

void Dialogtest::on_pushButton3l_clicked()
{
    if(Jkstatus.is_socket_connect!=1)
    {
        QMessageBox::information(this,"提示","JAKA控制器连接失败，请检查！");
        return;
    }
    int res=JKrobot.program_load("nowrotkll");
    if(res!=0)QMessageBox::information(this,"提示","旋扭左侧加载失败，请检查！");
    JKrobot.program_run();
}

void Dialogtest::on_pushButton3r_clicked()
{
    if(Jkstatus.is_socket_connect!=1)
    {
        QMessageBox::information(this,"提示","JAKA控制器连接失败，请检查！");
        return;
    }
    int res=JKrobot.program_load("nowrotkrl");
    if(res!=0)QMessageBox::information(this,"提示","旋扭右侧加载失败，请检查！");
    JKrobot.program_run();
}

void Dialogtest::on_pushButton2l_clicked()
{
    if(Jkstatus.is_socket_connect!=1)return;
    int res=JKrobot.program_load("nowrotklln");
    if(res!=0)QMessageBox::information(this,"提示","带自锁旋扭左侧加载失败，请检查！");
    JKrobot.program_run();
}

void Dialogtest::on_pushButton2r_clicked()
{
    if(Jkstatus.is_socket_connect!=1)
    {
        QMessageBox::information(this,"提示","JAKA控制器连接失败，请检查！");
        return;
    }
    int res=JKrobot.program_load("nowrotkrln");
    if(res!=0)QMessageBox::information(this,"提示","带自锁旋扭右侧加载失败，请检查！");
    JKrobot.program_run();
}

void Dialogtest::on_pushButtonjzj_clicked()//夹爪紧
{

    bool ok;
    int num=0;
    int kk=0;
    if(Jkstatus.is_socket_connect!=1)
    {
        QMessageBox::information(this,"提示","JAKA控制器连接失败，请检查！");
        return;
    }
    int num1 = QInputDialog::getInt(this, "脉冲数设置","请输入脉冲数", 100, 0, 10000, 100, &ok);
    if (ok) num=num1;
    // QMessageBox::information(this,"提示",QString::number(num));
    JKrobot.set_digital_output(IO_TOOL,0,1);//???
    JKrobot.set_digital_output(IO_TOOL,1,0);//???
    usleep(100000);
    JKrobot.set_digital_output(IO_TOOL,1,1);//???
    /***********
    while(kk<num1)
    {
        JKrobot.set_digital_output(IO_TOOL,1,0);//???
        usleep(100000);
        JKrobot.set_digital_output(IO_TOOL,1,1);//???
        //if(num==kk)break;
        kk++;
        usleep(100000);
    }
*************/
}

void Dialogtest::on_pushButtonjzs_clicked()
{
    bool ok;
    int num=0;
    int kk=0;
    if(Jkstatus.is_socket_connect!=1)
    {
        QMessageBox::information(this,"提示","JAKA控制器连接失败，请检查！");
        return;
    }
    int num1 = QInputDialog::getInt(this, "脉冲数设置","请输入脉冲数", 100, 0, 10000, 100, &ok);
    if (ok) num=num1;
    // QMessageBox::information(this,"提示",QString::number(num));
    JKrobot.set_digital_output(IO_TOOL,0,0);
    JKrobot.set_digital_output(IO_TOOL,1,0);//???
    usleep(100000);
    JKrobot.set_digital_output(IO_TOOL,1,1);//???
    /******************
    while(1)
    {
        JKrobot.set_digital_output(IO_TOOL,1,0);//???
        usleep(100000);
        JKrobot.set_digital_output(IO_TOOL,1,1);//???
        if(num==kk)break;
        kk++;
        usleep(100000);
    }
    *************/
}

