#include "dialogjakamovetest.h"
#include "ui_dialogjakamovetest.h"
#include "ComData.h"
#include <QMessageBox>


#define PI 3.1415926

DialogJakamovetest::DialogJakamovetest(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogJakamovetest)
{
    ui->setupUi(this);
    ui->comboBox->addItem("基坐标系");
    ui->comboBox->addItem("关节空间");
    ui->comboBox->addItem("工具坐标系");
    ui->comboBox->setCurrentIndex(0);
    JKrobot.set_user_frame_id(0);
}

DialogJakamovetest::~DialogJakamovetest()
{
    delete ui;

}

void DialogJakamovetest::on_pushButton_clicked()
{
    JointValue refP;
    CartesianPose cart;
    if(Jkstatus.is_socket_connect!=1)
    {
        QMessageBox::information(this,"提示","JAKA控制器连接失败，请检查！");
        return;
    }
    cart.tran.x = ui->lineEditx->text().toDouble();//     100;
    cart.tran.y = ui->lineEdity->text().toDouble();//200;
    cart.tran.z = ui->lineEditz->text().toDouble();//300;
    cart.rpy.rx = ui->lineEditrx->text().toDouble()* PI / 180;//120 * PI / 180;
    cart.rpy.ry = ui->lineEditry->text().toDouble()* PI / 180;//90 * PI / 180;
    cart.rpy.rz = ui->lineEditrz->text().toDouble()* PI / 180;//-90 * PI / 180;
    //笛卡尔空间运动,其中 ABS 代表绝对运动,FALSE 代表指令是非阻塞的,10 代表速度为 10mm/s
    //printf("rx=%f , ry=%f, rz=%f\n", cart.rpy.rx, cart.rpy.ry, cart.rpy.rz);
    JKrobot.linear_move(&cart, ABS, TRUE, 10);


    //refP.jVal[0] = 0*PI/180; refP.jVal[1] = 90 * PI / 180; refP.jVal[2] = -90 * PI / 180;
    //refP.jVal[3] = 90 * PI / 180; refP.jVal[4] = 90 * PI / 180; refP.jVal[5] = 0 * PI / 180;
    //JKrobot.joint_move(&refP, MoveMode::ABS, TRUE, 0.2);
}

void DialogJakamovetest::on_comboBox_currentIndexChanged(int index)
{
    JKrobot.set_user_frame_id(index);
}
