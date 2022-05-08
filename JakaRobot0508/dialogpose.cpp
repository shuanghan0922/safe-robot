#include "dialogpose.h"
#include "ui_dialogpose.h"
#include "ComData.h"

Dialogpose::Dialogpose(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialogpose)
{
    ui->setupUi(this);
}

Dialogpose::~Dialogpose()
{
    delete ui;
}

void Dialogpose::on_pushButton_clicked()
{
    double quat[4],eu[3];
    posset.px=ui->lineEditx1->text().toDouble();
    posset.py=ui->lineEdity1->text().toDouble();
    posset.pz=ui->lineEditz1->text().toDouble();
    quat[0]=ui->lineEditx2->text().toDouble();
    quat[1]=ui->lineEdity2->text().toDouble();
    quat[2]=ui->lineEditz2->text().toDouble();
    quat[3]=ui->lineEditw2->text().toDouble();
    quat2eular(quat,eu);
    eu[0]=0.0;
    eu[1]=0.0;
    eular2quat(eu,quat);
    posset.ox=quat[0];
    posset.oy=quat[1];
    posset.oz=quat[2];
    posset.ow=quat[3];
    posset.bsetpos=true;
    close();
}

void Dialogpose::on_pushButton_2_clicked()
{
    posset.bsetpos=false;
    close();
}
