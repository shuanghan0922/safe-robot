#include "dialoggl.h"
#include "ui_dialoggl.h"
//#include "dialogbjlabel.h"
//#include "dialogchkset.h"
#include "ComData.h"

Dialoggl::Dialoggl(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialoggl)
{
    ui->setupUi(this);
    dlguser=new Dialogusergl(this);
    dlgequ=new Dialogequset(this);
    dlgset=new Dialogparaset(this);
    dlgkktype=new Dialogkktype(this);
    //dlgphoto=new Dialogpaotoset(this);
   // dlglabel=new Dialogbjlabel(this);

    ui->stackedWidget->addWidget(dlguser);
   // ui->stackedWidget->addWidget(dlgpf);
    ui->stackedWidget->addWidget(dlgequ);
    ui->stackedWidget->addWidget(dlgset);
    ui->stackedWidget->addWidget(dlgkktype);
   // ui->stackedWidget->addWidget(dlglabel);


    ui->stackedWidget->setCurrentWidget(dlgset);
    button_style1="QPushButton{background-color:white;color: black;border-radius: 10px;  border: 2px groove gray;border-style: outset;}";

    button_style2="QPushButton{background-color:rgb(235, 142, 85);border-radius: 10px;  border: 2px groove gray;border-style: inset; }";
    ui->pushButton->setStyleSheet(button_style1);//
    //ui->pushButton_3->hide();
    ui->pushButton_2->hide();//setStyleSheet(button_style1);//
    ui->pushButton_4->setStyleSheet(button_style2);
    ui->pushButton_5->hide();
    ui->pushButton_6->setStyleSheet(button_style1);
    ui->pushButton_7->hide(); //setStyleSheet(button_style1);
}

Dialoggl::~Dialoggl()
{
    dlguser->close();
  //  dlgpf->close();
    //dlgcp->close();
    dlgset->close();
    dlgequ->close();
    dlgkktype->close();
    delete dlguser;
   // delete dlgpf;
    delete dlgequ;
    delete dlgset;
    delete dlgkktype;
   // delete dlglabel;

    delete ui;
}

void Dialoggl::on_pushButton_clicked()//用户       *********************************/管理
{
    ui->stackedWidget->setCurrentWidget(dlguser);
    ui->pushButton->setStyleSheet(button_style2);//
    ui->pushButton_2->setStyleSheet(button_style1);
    ui->pushButton_3->setStyleSheet(button_style1);//
    ui->pushButton_4->setStyleSheet(button_style1);
    ui->pushButton_5->setStyleSheet(button_style1);
    ui->pushButton_6->setStyleSheet(button_style1);
    ui->pushButton_7->setStyleSheet(button_style1);
}

void Dialoggl::on_pushButton_2_clicked()//相机参数
{
   // ui->stackedWidget->setCurrentWidget(dlgphoto);
    ui->pushButton->setStyleSheet(button_style1);//
    ui->pushButton_2->setStyleSheet(button_style2);
    ui->pushButton_3->setStyleSheet(button_style1);//
    ui->pushButton_4->setStyleSheet(button_style1);
    ui->pushButton_5->setStyleSheet(button_style1);
    ui->pushButton_6->setStyleSheet(button_style1);
    ui->pushButton_7->setStyleSheet(button_style1);
   // init();
    //plccmd.model=2;  //往PLC 发示教模式
  //  plccmd.bphotomodel=1;

    //plccmdrt.model=2;


}

void Dialoggl::on_pushButton_4_clicked()//系统参数
{
   ui->stackedWidget->setCurrentWidget(dlgset);
   ui->pushButton->setStyleSheet(button_style1);//
   ui->pushButton_2->setStyleSheet(button_style1);
   ui->pushButton_3->setStyleSheet(button_style1);//
   ui->pushButton_4->setStyleSheet(button_style2);
   ui->pushButton_5->setStyleSheet(button_style1);
   ui->pushButton_6->setStyleSheet(button_style1);
   ui->pushButton_7->setStyleSheet(button_style1);
}

void Dialoggl::on_pushButton_3_clicked()//报警标签
{
    ui->stackedWidget->setCurrentWidget(dlgkktype);
    ui->pushButton->setStyleSheet(button_style1);//
    ui->pushButton_2->setStyleSheet(button_style1);
    ui->pushButton_3->setStyleSheet(button_style2);//
    ui->pushButton_4->setStyleSheet(button_style1);
    ui->pushButton_5->setStyleSheet(button_style1);
    ui->pushButton_6->setStyleSheet(button_style1);
    ui->pushButton_7->setStyleSheet(button_style1);
}

void Dialoggl::on_pushButton_5_clicked()//配方参数
{
    //ui->stackedWidget->setCurrentWidget(dlgpf);
    ui->pushButton->setStyleSheet(button_style1);//
    ui->pushButton_2->setStyleSheet(button_style1);
    ui->pushButton_3->setStyleSheet(button_style1);//
    ui->pushButton_4->setStyleSheet(button_style1);
    ui->pushButton_5->setStyleSheet(button_style2);
    ui->pushButton_6->setStyleSheet(button_style1);
    ui->pushButton_7->setStyleSheet(button_style1);
}

void Dialoggl::on_pushButton_6_clicked()//电气柜
{
    ui->stackedWidget->setCurrentWidget(dlgequ);
    ui->pushButton->setStyleSheet(button_style1);//
    ui->pushButton_2->setStyleSheet(button_style1);
    ui->pushButton_3->setStyleSheet(button_style1);//
    ui->pushButton_4->setStyleSheet(button_style1);
    ui->pushButton_5->setStyleSheet(button_style1);
    ui->pushButton_6->setStyleSheet(button_style2);
    ui->pushButton_7->setStyleSheet(button_style1);
}

void Dialoggl::on_pushButton_7_clicked()//传感器设置
{
   // ui->stackedWidget->setCurrentWidget(dlgcp);
    ui->pushButton->setStyleSheet(button_style1);//
    ui->pushButton_2->setStyleSheet(button_style1);
    ui->pushButton_3->setStyleSheet(button_style1);//
    ui->pushButton_4->setStyleSheet(button_style1);
    ui->pushButton_5->setStyleSheet(button_style1);
    ui->pushButton_6->setStyleSheet(button_style1);
    ui->pushButton_7->setStyleSheet(button_style2);
  //  Dialogchkset dlgchkset;
   // int flag=dlgchkset.exec();

 //   if(flag==!QDialog::Accepted) return ;
    //plccmd.bchkset=1;

}
