#include "dialogparaset.h"
#include "ui_dialogparaset.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include "ComData.h"
//#include "capturethread.h"
#include <stdio.h>
#include <QFileDialog>
#include <QFile>
//#include "fins.h"


Dialogparaset::Dialogparaset(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialogparaset)
{
     ui->setupUi(this);
   // static int aa=0;
   // aa++;
  //  QMessageBox::information(this,"提示","参数构造函数执行次数"+QString::number(aa));
    //读参数

    //ui->comboBoxcp->setCurrentText(query.value(9).toString());
    //ui->labelcpname->setText(query.value(10).toString());
    //ui->lineEditpic->setText(QString::number(sysparaset.dayok));
    //ui->lineEditalarm->setText(QString::number(sysparaset.daybj));
   // ui->lineEditopt->setText(QString::number(sysparaset.dayop));
    ui->lineEditplcip->setText(sysparaset.addplc);
    ui->lineEditplcport->setText(QString::number(sysparaset.portplc));
    ui->lineEditplcip2->setText(sysparaset.addplc1);
    ui->lineEditplcport2->setText(QString::number(sysparaset.portplc1));
    ui->lineEditplcip3->setText(sysparaset.addplc2);
    ui->lineEditplcport3->setText(QString::number(sysparaset.portplc2));
  // ui->lineEditsecsip->setText(sysparaset.addsecs);
 //   ui->lineEditsecsport->setText(QString::number(sysparaset.portsecs));
    ui->lineEditpcip->setText(sysparaset.addpcplc);
   // QString qstep;
   // qstep.sprintf("%f",sysparaset.stepdj);
    //ui->lineEditdjstep->setText(qstep);
  //  qstep.sprintf("%f",sysparaset.stepupdj);
  //  ui->lineEditupdjstep->setText(qstep);

    QString qfreshold;   
    qfreshold.sprintf("%f",sysparaset.freshold1);
    ui->lineEditfreshold1->setText(qfreshold);
    qfreshold.sprintf("%f",sysparaset.freshold2);
    ui->lineEditfreshold2->setText(qfreshold);
    qfreshold.sprintf("%f",sysparaset.freshold3);
    ui->lineEditfreshold3->setText(qfreshold);
    qfreshold.sprintf("%f",sysparaset.freshold41);
    ui->lineEditfreshold41->setText(qfreshold);
    qfreshold.sprintf("%d",sysparaset.freshold42);
    ui->lineEditfreshold42->setText(qfreshold);
    qfreshold.sprintf("%f",sysparaset.freshold43);
    ui->lineEditfreshold43->setText(qfreshold);
    qfreshold.sprintf("%d",sysparaset.freshold44);
    ui->lineEditfreshold44->setText(qfreshold);

    QString qdb;
    qdb.sprintf("%f",sysparaset.dx);    
    ui->lineEditdx->setText(qdb);
    qdb.sprintf("%f",sysparaset.dy);
    ui->lineEditdy->setText(qdb);
    qdb.sprintf("%f",sysparaset.dz);
    ui->lineEditdz->setText(qdb);
    //qdb.sprintf("%f",In_parameter(0,0));
    qdb=QString::number(In_parameter(0,0),'f',14);
    ui->lineEditinpara00->setText(qdb);
    //qdb.sprintf("%f",In_parameter(0,1));
    qdb=QString::number(In_parameter(0,1),'f',14);
    ui->lineEditinpara01->setText(qdb);
    //qdb.sprintf("%f",In_parameter(0,2));
    qdb=QString::number(In_parameter(0,2),'f',14);
    ui->lineEditinpara02->setText(qdb);
    //qdb.sprintf("%f",In_parameter(1,0));
    qdb=QString::number(In_parameter(1,0),'f',14);
    ui->lineEditinpara10->setText(qdb);
    //qdb.sprintf("%f",In_parameter(1,1));
    qdb=QString::number(In_parameter(1,1),'f',14);
    ui->lineEditinpara11->setText(qdb);
    //qdb.sprintf("%f",In_parameter(1,2));
    qdb=QString::number(In_parameter(1,2),'f',14);
    ui->lineEditinpara12->setText(qdb);
    qdb=QString::number(In_parameter(2,0),'f',14);
    //qdb.sprintf("%f",In_parameter(2,0));
    ui->lineEditinpara20->setText(qdb);
    //qdb.sprintf("%f",In_parameter(2,1));
    qdb=QString::number(In_parameter(2,1),'f',14);
    ui->lineEditinpara21->setText(qdb);
    //qdb.sprintf("%f",In_parameter(2,2));
    qdb=QString::number(In_parameter(2,2),'f',14);
    ui->lineEditinpara22->setText(qdb);

    ui->checkBox->setChecked(sysparaset.bpp);
    ui->checkBox_2->setChecked(sysparaset.bsf);
    ui->checkBox_3->setChecked(sysparaset.bsj);
    ui->checkBox_4->setChecked(sysparaset.bsl);
//    disppf(); //read pfdata

}

void Dialogparaset::disppf() //read 配方
{
/***********************
    QString csLogFile;
    csLogFile.clear();
    if(!sysparaset.pfno.length()) return;
    csLogFile.append(tr("%1/%2.txt").arg(dirpf).arg(sysparaset.pfno));
   // csLogFile=dirprj+ui->lineEditpfno->text()+".txt";
    QFile g_pfFile(csLogFile);

    QString qnum,qid,qdt1,qunit1,qdt2,qunit2;

    int row=0; //ui->tableWidget->rowCount();

     ui->tableWidget->setRowCount(0);

      QTextStream in(&g_pfFile);
   //  QString qstr=in.readLine();

    if (g_pfFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
          while (!in.atEnd()) //while (!g_pfFile.atEnd())
          {


              ui->tableWidget->insertRow(row);

              QString str=in.readLine();

             // qnum = str.section(',', 0, 0);
              qid = str.section(',', 0, 0);
              qdt1 = str.section(',',1, 1);
              pfyj[row+1]=qdt1.toFloat();

              ui->tableWidget->setItem(row,0,new QTableWidgetItem(qid));
              ui->tableWidget->setItem(row,1,new QTableWidgetItem(qdt1));
              ui->tableWidget->setItem(row,2,new QTableWidgetItem(qunit1));

              row++;

          }
          ipfyj=row;
          g_pfFile.close();
      }
    else QMessageBox::information(this,"提示","不能打开配方文件："+csLogFile+"，请选择配方文件或检查配方文件！");

    csLogFile.clear();
    if(!sysparaset.pfno.length()) return;
    csLogFile.append(tr("%1/%2pos.txt").arg(dirpf).arg(sysparaset.pfno));
   // csLogFile=dirprj+ui->lineEditpfno->text()+".txt";
    QFile g_pfFile1(csLogFile);
//
    QString qno,qpos,qdjup,qdjdown;

    row=0; //ui->tableWidget->rowCount();

       QTextStream in1(&g_pfFile1);
   //  QString qstr=in.readLine();

    if (g_pfFile1.open(QIODevice::ReadOnly | QIODevice::Text))
    {
          while (!in1.atEnd()) //while (!g_pfFile.atEnd())
          {
            //  ui->tableWidgetsj->insertRow(row);

              QString str=in1.readLine();

             // qnum = str.section(',', 0, 0);
              qno = str.section(',', 0, 0);
              qpos = str.section(',',1, 1);
              pfxl[row*3+1]=qpos.toInt();
              qdjup = str.section(',', 2, 2);
              pfxl[row*3+2]=qdjup.toInt();
              qdjdown = str.section(',', 3, 3);
              pfxl[row*3+3]=qdjdown.toInt();

              row++;

          }
      //   ui->lineEditposnum->setText(QString::number(row));
          pfxl[0]=row*3;
          g_pfFile1.close();
      }
    else QMessageBox::information(this,"提示","不能打开视觉配方文件："+csLogFile+"，请选择配方文件或检查配方文件！");
*****************/

}


void Dialogparaset::paintEvent(QPaintEvent *)//重绘
{

    //坐标x，y;绘图长宽length，width;块行列row，col;块内行列krow，kcol;边间距gap，颜色col0：gray,1:red,2:green,3:yellow
   // drawcp(10,50,520,540,productpara.grow,productpara.gcol,productpara.row,productpara.col,5,3);

   // disppf(); //read pfdata
}
//产品示意图
//坐标x，y;绘图长宽length，width;块行列row，col;块内行列krow，kcol;边间距gap，颜色col
void Dialogparaset::drawcp(int x,int y,int width,int length,int row,int col,int krow,int kcol,int gap,int colrec)
{
    int w,l;//chip宽，长
    int xr,yr;//chip 坐标
    int irow,icol,ikrow,ikcol;//循环，当前芯片的块和块内位置
    QPainter painter(this);
    if (colrec==1)painter.setBrush(Qt::red);//red
    else if(colrec==2)painter.setBrush(Qt::green);//green
    else if(colrec==3)painter.setBrush(Qt::yellow);//yellow
    else painter.setBrush(Qt::gray);

    w=(width-2*gap)/(col*kcol+0.5*(kcol-1)*col+2*(col-1));
    l=(length-2*gap)/(row*krow+0.5*(krow-1)*row+2*(row-1));

    for(irow=0;irow<row;irow++)
        for(icol=0;icol<col;icol++)
            for(ikrow=0;ikrow<krow;ikrow++)
                for(ikcol=0;ikcol<kcol;ikcol++)
                {
                   xr=x+gap+icol*(kcol*w+(kcol-1)*0.5*w+2*w)+1.5*w*ikcol;
                   yr=y+gap+irow*(krow*l+(krow-1)*0.5*l+2*l)+1.5*l*ikrow;
                   painter.drawRect(xr, yr, w, l);

                }

}


void Dialogparaset::cpshow(int row, int col, bool status) //产品示意图default:
{
    QList<QLabel *> m_lstLabel;
    int count = row*col;
    for(int i = 0;i < count;i++)
    {
        QLabel *label1 = new QLabel(this);
        label1->setParent(this);
        label1->setGeometry(320,170,30,50);
        if(status == true)
        {
            label1->setStyleSheet("background-color:rgb(0,255,0)");
        }
        else
        {
            label1->setStyleSheet("background-color:rgb(255,0,0)");
        }
        m_lstLabel.append(label1);
    }

    for(int j = 0;j < col;j++)
    {
        for(int k = 0;k < row;k++)
        {
            m_lstLabel.at(k+row*j)->move(320-40*j,170-60*k);
        }
    }
}

Dialogparaset::~Dialogparaset()
{

    delete ui;
}

void Dialogparaset::on_pushButton_clicked()//确定
{

    QString str;
    QByteArray ba ;
    QSqlQuery query,sql_query;
    bool value;

     //bsysset=1;
     accept();
}

void Dialogparaset::on_pushButton_2_clicked()
{
    close();
}



void Dialogparaset::on_tabWidget_currentChanged(int index)
{

 //ui->lineEditpic->setText(QString::number(sysparaset.dayok));

}


void Dialogparaset::on_lineEditpic_returnPressed()//图片保存天数
{
    /******************
    QString qok;
    bool value;
    QSqlQuery query,sql_query;

    int dt=ui->lineEditpic->text().toInt();
    if(dt<=0)
    {
       QMessageBox::information(this,"提示","参数输入错误,请重新输入!");
       ui->lineEditpic->setText(QString::number(sysparaset.dayok));
       return;
    }

    sysparaset.dayok=ui->lineEditpic->text().toInt();
    qok.sprintf("%d",sysparaset.dayok);
    QString update_sql = "update syspara set dayok=:dayok";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":dayok", qok);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","OK图片参数保存失败！");
       this->close();
    }
*****************/
    //参数设置操作记录
    /*****************************
    // bool value;
    QString tm=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString item="参数设置";
    QString user=logid;
    QString demo="参数信息：OK图片保存天数:"+qok;
    value=query.exec("INSERT INTO syslog VALUES('"+tm+"','"+logid+"','"+item+"','"+demo+"')");
    if(value)
    {
      // QMessageBox::information(this,"提示","参数设置操作记录成功！");
      // this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","OK图片参数修改操作记录失败！");
      // this->close();
    }
*************************/
}

void Dialogparaset::on_lineEditalarm_returnPressed()//报警保存天数
{

}

void Dialogparaset::on_lineEditopt_returnPressed()//操作保存天数
{

}

void Dialogparaset::on_lineEditplcip_returnPressed()//IP
{
    QString qplcip;
    bool value;
    QSqlQuery query,sql_query;

    sysparaset.addplc=ui->lineEditplcip->text();
    qplcip=sysparaset.addplc;
    QString update_sql = "update syspara set addplc=:addplc";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":addplc", qplcip);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","PLC1 IP参数保存失败！");
       this->close();
    }

    //参数设置操作记录
    // bool value;
    /***************************************
    QString tm=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString item="参数设置";
    QString user=logid;
    QString demo="参数信息：PLC IP:"+qplcip;
    value=query.exec("INSERT INTO syslog VALUES('"+tm+"','"+logid+"','"+item+"','"+demo+"')");
    if(value)
    {
      // QMessageBox::information(this,"提示","参数设置操作记录成功！");
      // this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","PLC IP参数修改操作记录失败！");
      // this->close();
    }
    **************************************************/
}

void Dialogparaset::on_lineEditplcport_returnPressed()//port
{
    QString qplcport;
    bool value;
    QSqlQuery query,sql_query;


    sysparaset.portplc=ui->lineEditplcport->text().toInt();
    qplcport.sprintf("%d",sysparaset.portplc);

    QString update_sql = "update syspara set portplc=:portplc";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":portplc", qplcport);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","PLC1 端口参数保存失败！");
       this->close();
    }

    //参数设置操作记录
    // bool value;
    /****************************************
    QString tm=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString item="参数设置";
    QString user=logid;
    QString demo="参数信息：PLC 端口:"+qplcport;
    value=query.exec("INSERT INTO syslog VALUES('"+tm+"','"+logid+"','"+item+"','"+demo+"')");
    if(value)
    {
      // QMessageBox::information(this,"提示","参数设置操作记录成功！");
      // this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","PLC 端口参数修改操作记录失败！");
      // this->close();
    }
    *****************************/
}

void Dialogparaset::on_lineEditpcip_returnPressed()//PC IP
{
    QString qpcplcip;
    bool value;
    QSqlQuery query,sql_query;


    sysparaset.addpcplc=ui->lineEditpcip->text();
    qpcplcip=sysparaset.addpcplc;
    QString update_sql = "update syspara set addpcplc=:addpcplc";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":addpcplc", qpcplcip);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","PCPLC IP参数保存失败！");
       this->close();
    }

    //参数设置操作记录
    // bool value;
    /*******************************
    QString tm=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString item="参数设置";
    QString user=logid;
    QString demo="参数信息：PCPLC IP:"+qpcplcip;
    value=query.exec("INSERT INTO syslog VALUES('"+tm+"','"+logid+"','"+item+"','"+demo+"')");
    if(value)
    {
      // QMessageBox::information(this,"提示","参数设置操作记录成功！");
      // this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","PCPLC IP参数修改操作记录失败！");
      // this->close();
    }
    ***************************************/
}

void Dialogparaset::on_lineEditsecsip_returnPressed()//SECS IP
{

}

void Dialogparaset::on_lineEditsecsport_returnPressed()//SECS port
{

}

void Dialogparaset::on_lineEditdjstep_returnPressed()//点击步长
{

}

void Dialogparaset::on_lineEditupdjstep_returnPressed()//上下电机步长
{

}

void Dialogparaset::on_lineEditzfreshold_returnPressed()//正面阈值
{
    QString qzfreshold;
    bool value;
    QSqlQuery query,sql_query;
    float dt; //=ui->lineEditzfreshold->text().toFloat();
   // if(dt<=0||dt>1)
    {
       QMessageBox::information(this,"提示","参数输入错误,请重新输入!");
      // qzfreshold.sprintf("%f",sysparaset.zfreshold);
     //  ui->lineEditzfreshold->setText(qzfreshold);
       return;
    }

  //  sysparaset.zfreshold=ui->lineEditzfreshold->text().toFloat();
  //  qzfreshold.sprintf("%f",sysparaset.zfreshold);

    QString update_sql = "update syspara set zthreshold=:zthreshold";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":zthreshold", qzfreshold);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","正面阈值参数保存失败！");
       this->close();
    }

    //参数设置操作记录
    // bool value;
    /******
    QString tm=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString item="参数设置";
    QString user=logid;
    QString demo="参数信息：正面阈值参数:"+qzfreshold;
    value=query.exec("INSERT INTO syslog VALUES('"+tm+"','"+logid+"','"+item+"','"+demo+"')");
    if(value)
    {
      // QMessageBox::information(this,"提示","参数设置操作记录成功！");
      // this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","正面阈值参数修改操作记录失败！");
      // this->close();
    }
    ************************************/
}

void Dialogparaset::on_lineEditffreshold_returnPressed()//反面阈值
{
    /*************
    QString qffreshold;
    bool value;
    QSqlQuery query,sql_query;

    float dt=ui->lineEditffreshold->text().toFloat();
    if(dt<=0||dt>1)
    {
       QMessageBox::information(this,"提示","参数输入错误,请重新输入!");
       qffreshold.sprintf("%f",sysparaset.ffreshold);
       ui->lineEditffreshold->setText(qffreshold);
      // ui->lineEditffreshold->setText(QString::number(sysparaset.ffreshold));
       return;
    }
    sysparaset.ffreshold=ui->lineEditffreshold->text().toFloat();
    qffreshold.sprintf("%f",sysparaset.ffreshold);

    QString update_sql = "update syspara set fthreshold=:fthreshold";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":fthreshold", qffreshold);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","反面阈值参数保存失败！");
       this->close();
    }

    //参数设置操作记录
    // bool value;
    QString tm=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString item="参数设置";
    QString user=logid;
    QString demo="参数信息：反面阈值参数:"+qffreshold;
    value=query.exec("INSERT INTO syslog VALUES('"+tm+"','"+logid+"','"+item+"','"+demo+"')");
    if(value)
    {
      // QMessageBox::information(this,"提示","参数设置操作记录成功！");
      // this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","反面阈值参数修改操作记录失败！");
      // this->close();
    }
    **************/
}


void Dialogparaset::on_lineEditkg_returnPressed()//树脂量
{
    /************************
    QString qsz;
    bool value;
    QSqlQuery query,sql_query;

    int dt=ui->lineEditkg->text().toInt();
    if(dt<=0)
    {
       QMessageBox::information(this,"提示","参数输入错误,请重新输入!");
       ui->lineEditkg->setText(QString::number(sysparaset.szunit));
       return;
    }
    sysparaset.szunit=ui->lineEditkg->text().toInt();
    qsz.sprintf("%d",sysparaset.szunit);

    QString update_sql = "update syspara set szunit=:szunit";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":szunit", qsz);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","单模树脂消耗参数保存失败！");
       this->close();
    }

    //参数设置操作记录
    // bool value;
    QString tm=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString item="参数设置";
    QString user=logid;
    QString demo="参数信息：单模树脂消耗参数:"+qsz;
    value=query.exec("INSERT INTO syslog VALUES('"+tm+"','"+logid+"','"+item+"','"+demo+"')");
    if(value)
    {
      // QMessageBox::information(this,"提示","单模树脂消耗参数设置操作记录成功！");
      // this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","单模树脂消耗参数修改操作记录失败！");
      // this->close();
    }
    *******************/
}

void Dialogparaset::on_lineEditkg_2_returnPressed()//树脂提示
{
    /*****************************
    QString qszmin;
    bool value;
    QSqlQuery query,sql_query;

    int dt=ui->lineEditkg_2->text().toInt();
    if(dt<=0)
    {
       QMessageBox::information(this,"提示","参数输入错误,请重新输入!");
       ui->lineEditkg_2->setText(QString::number(sysparaset.szmin));
       return;
    }
    sysparaset.szmin=ui->lineEditkg_2->text().toInt();
    qszmin.sprintf("%d",sysparaset.szmin);

    QString update_sql = "update syspara set szmin=:szmin";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":szmin", qszmin);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","树脂阈值参数保存失败！");
       this->close();
    }

    //参数设置操作记录
    // bool value;
    QString tm=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString item="参数设置";
    QString user=logid;
    QString demo="参数信息：树脂阈值参数:"+qszmin;
    value=query.exec("INSERT INTO syslog VALUES('"+tm+"','"+logid+"','"+item+"','"+demo+"')");
    if(value)
    {
      // QMessageBox::information(this,"提示","树脂阈值参数设置操作记录成功！");
      // this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","树脂阈值参数修改操作记录失败！");
      // this->close();
    }
    ******************/
}

void Dialogparaset::on_lineEdit_returnPressed()//框架
{
    /*********************
    QString qkj;
    bool value;
    QSqlQuery query,sql_query;

    int dt=ui->lineEdit->text().toInt();
    if(dt<=0)
    {
       QMessageBox::information(this,"提示","参数输入错误,请重新输入!");
       ui->lineEdit->setText(QString::number(sysparaset.kjunit));
       return;
    }
    sysparaset.kjunit=ui->lineEdit->text().toInt();
    qkj.sprintf("%d",sysparaset.kjunit);

    QString update_sql = "update syspara set kjunit=:kjunit";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":kjunit", qkj);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","单模框架消耗参数保存失败！");
       this->close();
    }

    //参数设置操作记录
    // bool value;
    QString tm=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString item="参数设置";
    QString user=logid;
    QString demo="参数信息：单模框架消耗参数:"+qkj;
    value=query.exec("INSERT INTO syslog VALUES('"+tm+"','"+logid+"','"+item+"','"+demo+"')");
    if(value)
    {
      // QMessageBox::information(this,"提示","单模树脂消耗参数设置操作记录成功！");
      // this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","单模框架消耗参数修改操作记录失败！");
      // this->close();
    }
    *********************/
}

void Dialogparaset::on_lineEdit_2_returnPressed()//框架阈值
{
    /*************************
    QString qkjmin;
    bool value;
    QSqlQuery query,sql_query;

    int dt=ui->lineEdit_2->text().toInt();
    if(dt<=0)
    {
       QMessageBox::information(this,"提示","参数输入错误,请重新输入!");
       ui->lineEdit_2->setText(QString::number(sysparaset.kjmin));
       return;
    }
    sysparaset.kjmin=ui->lineEdit_2->text().toInt();
    qkjmin.sprintf("%d",sysparaset.kjmin);

    QString update_sql = "update syspara set kjmin=:kjmin";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":kjmin", qkjmin);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","框架阈值参数保存失败！");
       this->close();
    }

    //参数设置操作记录
    // bool value;
    QString tm=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString item="参数设置";
    QString user=logid;
    QString demo="参数信息：框架阈值参数:"+qkjmin;
    value=query.exec("INSERT INTO syslog VALUES('"+tm+"','"+logid+"','"+item+"','"+demo+"')");
    if(value)
    {
      // QMessageBox::information(this,"提示","框架阈值参数设置操作记录成功！");
      // this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","框架阈值参数修改操作记录失败！");
      // this->close();
    }
    **********************/
}

void Dialogparaset::on_pushButton_3_clicked()//延时参数
{
    /***********************
    QString qppdelay,qurruntm,qppruntm,qyjhmruntm,qyjqsruntm,qszruntm,qcqruntm,qcqqcruntm,qxlruntm;
    bool value;
    bool berr=0;
    QSqlQuery query,sql_query;
    unsigned char dataplc[100];
    int icmd;
    int ret,ret1,ret2;
    int dt[10];

    dt[0]=ui->lineEditppdelay->text().toInt();
    dt[1]=ui->lineEditurruntm->text().toInt();
    dt[2]=ui->lineEditppruntm->text().toInt();
    dt[3]=ui->lineEdityjhmruntm->text().toInt();
    dt[4]=ui->lineEdityjqsruntm->text().toInt();
    dt[5]=ui->lineEditszruntm->text().toInt();
    dt[6]=ui->lineEditcqruntm->text().toInt();
    dt[7]=ui->lineEditcqqcruntm->text().toInt();
    dt[8]=ui->lineEditxlruntm->text().toInt();
    dt[9]=ui->lineEditppdelay1->text().toInt();
    for(int i=0;i<10;i++)
       if(dt[i]<1||dt[i]>199) berr=1;

    if(berr)
    {
       QMessageBox::information(this,"提示","延时参数输入范围超出1~199错误,请检查并重新输入!");
      // ui->lineEditkg->setText(QString::number(sysparaset.szunit));
       return;
    }

    pfdo=0;
    pfwrite=1;
    while(!pfdo) usleep(100);

    icmd=dt[0];  //sysparaset.ppdelay;
    dataplc[1]=(unsigned char)icmd;
    dataplc[0]=(unsigned char)(icmd>>8);
  //  ret2=UDPWritePLC(sockplc,494,1,dataplc);//write words,send cmd

    icmd=dt[1]; //sysparaset.urruntm;  //;
    dataplc[3]=(unsigned char)icmd;
    dataplc[2]=(unsigned char)(icmd>>8);
   // ret2=UDPWritePLC(sockplc,495,1,dataplc);//write words,send cmd

    icmd=dt[2];//sysparaset.ppruntm;  //;
    dataplc[5]=(unsigned char)icmd;
    dataplc[4]=(unsigned char)(icmd>>8);
   // ret2=UDPWritePLC(sockplc,496,1,dataplc);//write words,send cmd

    icmd=dt[3];//sysparaset.yjhmruntm;  //;
    dataplc[7]=(unsigned char)icmd;
    dataplc[6]=(unsigned char)(icmd>>8);
   // ret2=UDPWritePLC(sockplc,497,1,dataplc);//write words,send cmd

    icmd=dt[4];//sysparaset.yjqsruntm;  //;
    dataplc[9]=(unsigned char)icmd;
    dataplc[8]=(unsigned char)(icmd>>8);
   // ret2=UDPWritePLC(sockplc,498,1,dataplc);//write words,send cmd

    icmd=dt[5];//sysparaset.szruntm;  //;
    dataplc[11]=(unsigned char)icmd;
    dataplc[10]=(unsigned char)(icmd>>8);
   // ret2=UDPWritePLC(sockplc,499,1,dataplc);//write words,send cmd

    icmd=dt[6];//sysparaset.cqruntm;  //;
    dataplc[13]=(unsigned char)icmd;
    dataplc[12]=(unsigned char)(icmd>>8);
   // ret2=UDPWritePLC(sockplc,500,1,dataplc);//write words,send cmd

    icmd=dt[7];//sysparaset.cqqcruntm;  //;
    dataplc[15]=(unsigned char)icmd;
    dataplc[14]=(unsigned char)(icmd>>8);
   // ret2=UDPWritePLC(sockplc,501,1,dataplc);//write words,send cmd

    icmd=dt[8];//sysparaset.xlruntm;  //;
    dataplc[17]=(unsigned char)icmd;
    dataplc[16]=(unsigned char)(icmd>>8);
   // ret2=UDPWritePLC(sockplc,502,1,dataplc);//write words,send cmd

    icmd=dt[9];//ysparaset.ppdelay1;  //;
    dataplc[19]=(unsigned char)icmd;
    dataplc[18]=(unsigned char)(icmd>>8);
   // ret2=UDPWritePLC(sockplc,502,1,dataplc);//write words,send cmd

    ret=UDPWritePLC(sockplc,494,10,dataplc);//write words,send cmd
    if(ret)
    {
        QMessageBox::information(this,"提示","延迟参数下载到PLC发生错误,请按确定重新下载!");
    }
    else QMessageBox::information(this,"提示","延迟数据已下载!");

    pfwrite=0;
*********************/

}

void Dialogparaset::on_lineEditcullfreshold_returnPressed()//Cull阈值
{
    /*****************
    QString qcullfreshold;
        bool value;
        QSqlQuery query,sql_query;
        float dt=ui->lineEditcullfreshold->text().toFloat();
        if(dt<=0||dt>1)
        {
           QMessageBox::information(this,"提示","参数输入错误,请重新输入!");
           qcullfreshold.sprintf("%f",sysparaset.cullfreshold);
           ui->lineEditcullfreshold->setText(qcullfreshold);
           return;
        }

        sysparaset.cullfreshold=ui->lineEditcullfreshold->text().toFloat();
        qcullfreshold.sprintf("%f",sysparaset.cullfreshold);

        QString update_sql = "update syspara set cullthreshold=:cullthreshold";
        sql_query.prepare(update_sql);

        sql_query.bindValue(":cullthreshold", qcullfreshold);

        value=sql_query.exec();

        if(value)
        {
          // QMessageBox::information(this,"提示","保存参数成功！");
         //  this->close();
        }
        else
        {
           QMessageBox::information(this,"提示","cull阈值参数保存失败！");
           this->close();
        }

        //参数设置操作记录
        // bool value;
        QString tm=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        QString item="参数设置";
        QString user=logid;
        QString demo="参数信息：cull阈值参数:"+qcullfreshold;
        value=query.exec("INSERT INTO syslog VALUES('"+tm+"','"+logid+"','"+item+"','"+demo+"')");
        if(value)
        {
          // QMessageBox::information(this,"提示","参数设置操作记录成功！");
          // this->close();
        }
        else
        {
           QMessageBox::information(this,"提示","cull阈值参数修改操作记录失败！");
          // this->close();
        }
        *************/
}

void Dialogparaset::on_lineEditboxfreshold_returnPressed()//box阈值
{
    /**************
        QString qboxfreshold;
        bool value;
        QSqlQuery query,sql_query;
        float dt=ui->lineEditboxfreshold->text().toFloat();
        if(dt<=0||dt>1)
        {
           QMessageBox::information(this,"提示","参数输入错误,请重新输入!");
           qboxfreshold.sprintf("%f",sysparaset.boxfreshold);
           ui->lineEditboxfreshold->setText(qboxfreshold);
           return;
        }

        sysparaset.boxfreshold=ui->lineEditboxfreshold->text().toFloat();
        qboxfreshold.sprintf("%f",sysparaset.boxfreshold);

        QString update_sql = "update syspara set boxthreshold=:boxthreshold";
        sql_query.prepare(update_sql);

        sql_query.bindValue(":boxthreshold", qboxfreshold);

        value=sql_query.exec();

        if(value)
        {
          // QMessageBox::information(this,"提示","保存参数成功！");
         //  this->close();
        }
        else
        {
           QMessageBox::information(this,"提示","box阈值参数保存失败！");
           this->close();
        }

        //参数设置操作记录
        // bool value;
        QString tm=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        QString item="参数设置";
        QString user=logid;
        QString demo="参数信息：box阈值参数:"+qboxfreshold;
        value=query.exec("INSERT INTO syslog VALUES('"+tm+"','"+logid+"','"+item+"','"+demo+"')");
        if(value)
        {
          // QMessageBox::information(this,"提示","参数设置操作记录成功！");
          // this->close();
        }
        else
        {
           QMessageBox::information(this,"提示","box阈值参数修改操作记录失败！");
          // this->close();
        }
        **************/
}

void Dialogparaset::on_lineEditfreshold1_returnPressed()//photo1
{
    QString qfreshold;
    bool value;
    QSqlQuery query,sql_query;
    float dt=ui->lineEditfreshold1->text().toFloat();
    if(dt<=0||dt>1)
    {
       QMessageBox::information(this,"提示","参数输入错误,请重新输入!");

       return;
    }

    sysparaset.freshold1=ui->lineEditfreshold1->text().toFloat();
    qfreshold.sprintf("%f",sysparaset.freshold1);

    QString update_sql = "update syspara set freshold1=:threshold";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":threshold", qfreshold);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","相机1阈值参数保存失败！");
       this->close();
    }

    //参数设置操作记录
    // bool value;
    /******************************************
    QString tm=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString item="参数设置";
    QString user=logid;
    QString demo="参数信息：相机1阈值参数:"+qfreshold;
    value=query.exec("INSERT INTO syslog VALUES('"+tm+"','"+logid+"','"+item+"','"+demo+"')");
    if(value)
    {
      // QMessageBox::information(this,"提示","参数设置操作记录成功！");
      // this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","相机1阈值参数修改操作记录失败！");
      // this->close();
    }
    ***********************/
}

void Dialogparaset::on_lineEditfreshold2_returnPressed()//photo2
{
    QString qfreshold;
    bool value;
    QSqlQuery query,sql_query;
    float dt=ui->lineEditfreshold2->text().toFloat();
    if(dt<=0||dt>1)
    {
       QMessageBox::information(this,"提示","参数输入错误,请重新输入!");

       return;
    }

    sysparaset.freshold2=ui->lineEditfreshold2->text().toFloat();
    qfreshold.sprintf("%f",sysparaset.freshold2);

    QString update_sql = "update syspara set freshold2=:threshold";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":threshold", qfreshold);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","相机2阈值参数保存失败！");
       this->close();
    }

    //参数设置操作记录
    // bool value;
    /***********************************
    QString tm=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString item="参数设置";
    QString user=logid;
    QString demo="参数信息：相机2阈值参数:"+qfreshold;
    value=query.exec("INSERT INTO syslog VALUES('"+tm+"','"+logid+"','"+item+"','"+demo+"')");
    if(value)
    {
      // QMessageBox::information(this,"提示","参数设置操作记录成功！");
      // this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","相机2阈值参数修改操作记录失败！");
      // this->close();
    }
    *****************************/
}

void Dialogparaset::on_lineEditfreshold3_returnPressed()//photo3
{
    QString qfreshold;
    bool value;
    QSqlQuery query,sql_query;
    float dt=ui->lineEditfreshold3->text().toFloat();
    if(dt<=0||dt>1)
    {
       QMessageBox::information(this,"提示","参数输入错误,请重新输入!");

       return;
    }

    sysparaset.freshold3=ui->lineEditfreshold3->text().toFloat();
    qfreshold.sprintf("%f",sysparaset.freshold3);

    QString update_sql = "update syspara set freshold3=:threshold";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":threshold", qfreshold);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","相机3阈值参数保存失败！");
       this->close();
    }

    //参数设置操作记录
    // bool value;
    /****************************************************
    QString tm=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString item="参数设置";
    QString user=logid;
    QString demo="参数信息：相机3阈值参数:"+qfreshold;
    value=query.exec("INSERT INTO syslog VALUES('"+tm+"','"+logid+"','"+item+"','"+demo+"')");
    if(value)
    {
      // QMessageBox::information(this,"提示","参数设置操作记录成功！");
      // this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","相机3阈值参数修改操作记录失败！");
      // this->close();
    }
    *****************************/
}

void Dialogparaset::on_lineEditfreshold41_returnPressed()//photo4 连接件相似度
{
    QString qfreshold;
    bool value;
    QSqlQuery query,sql_query;
    float dt=ui->lineEditfreshold41->text().toFloat();
    if(dt<=0||dt>1)
    {
       QMessageBox::information(this,"提示","参数输入错误,请重新输入!");

       return;
    }

    sysparaset.freshold41=ui->lineEditfreshold41->text().toFloat();
    qfreshold.sprintf("%f",sysparaset.freshold41);

    QString update_sql = "update syspara set freshold41=:threshold";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":threshold", qfreshold);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","相机4连接件相似度阈值参数保存失败！");
       this->close();
    }

    //参数设置操作记录
    // bool value;
    /*****************************************
    QString tm=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString item="参数设置";
    QString user=logid;
    QString demo="参数信息：相机4连接件相似度阈值参数:"+qfreshold;
    value=query.exec("INSERT INTO syslog VALUES('"+tm+"','"+logid+"','"+item+"','"+demo+"')");
    if(value)
    {
      // QMessageBox::information(this,"提示","参数设置操作记录成功！");
      // this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","相机4连接件相似度阈值参数修改操作记录失败！");
      // this->close();
    }
    ***************************/
}

void Dialogparaset::on_lineEditfreshold42_returnPressed()//photo4 连接件二值化
{
    QString qfreshold;
    bool value;
    QSqlQuery query,sql_query;
    float dt=ui->lineEditfreshold42->text().toInt();
    if(dt<=0||dt>255)
    {
       QMessageBox::information(this,"提示","参数输入错误,请重新输入!");

       return;
    }

    sysparaset.freshold42=ui->lineEditfreshold42->text().toInt();
    qfreshold.sprintf("%d",sysparaset.freshold42);

    QString update_sql = "update syspara set freshold42=:threshold";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":threshold", qfreshold);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","相机4连接件二值化阈值参数保存失败！");
       this->close();
    }

    //参数设置操作记录
    // bool value;
    /****************************************
    QString tm=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString item="参数设置";
    QString user=logid;
    QString demo="参数信息：相机4连接件二值化阈值参数:"+qfreshold;
    value=query.exec("INSERT INTO syslog VALUES('"+tm+"','"+logid+"','"+item+"','"+demo+"')");
    if(value)
    {
      // QMessageBox::information(this,"提示","参数设置操作记录成功！");
      // this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","相机4连接件二值化阈值参数修改操作记录失败！");
      // this->close();
    }
    *******************************/
}

void Dialogparaset::on_lineEditfreshold43_returnPressed()//photo4 芯片相似度
{
    QString qfreshold;
    bool value;
    QSqlQuery query,sql_query;
    float dt=ui->lineEditfreshold43->text().toFloat();
    if(dt<=0||dt>1)
    {
       QMessageBox::information(this,"提示","参数输入错误,请重新输入!");

       return;
    }

    sysparaset.freshold43=ui->lineEditfreshold43->text().toFloat();
    qfreshold.sprintf("%f",sysparaset.freshold43);

    QString update_sql = "update syspara set freshold43=:threshold";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":threshold", qfreshold);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","相机4芯片相似度阈值参数保存失败！");
       this->close();
    }

    //参数设置操作记录
    // bool value;
    /*******************************************
    QString tm=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString item="参数设置";
    QString user=logid;
    QString demo="参数信息：相机4芯片相似度阈值参数:"+qfreshold;
    value=query.exec("INSERT INTO syslog VALUES('"+tm+"','"+logid+"','"+item+"','"+demo+"')");
    if(value)
    {
      // QMessageBox::information(this,"提示","参数设置操作记录成功！");
      // this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","相机4芯片相似度阈值参数修改操作记录失败！");
      // this->close();
    }
    ******************************/
}

void Dialogparaset::on_lineEditfreshold44_returnPressed()//photo4 芯片二值化
{
    QString qfreshold;
    bool value;
    QSqlQuery query,sql_query;
    float dt=ui->lineEditfreshold44->text().toInt();
    if(dt<=0||dt>255)
    {
       QMessageBox::information(this,"提示","参数输入错误,请重新输入!");

       return;
    }

    sysparaset.freshold44=ui->lineEditfreshold44->text().toInt();
    qfreshold.sprintf("%d",sysparaset.freshold44);

    QString update_sql = "update syspara set freshold44=:threshold";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":threshold", qfreshold);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","相机4芯片二值化阈值参数保存失败！");
       this->close();
    }

    //参数设置操作记录
    // bool value;
    /*******************************************
    QString tm=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString item="参数设置";
    QString user=logid;
    QString demo="参数信息：相机4芯片二值化阈值参数:"+qfreshold;
    value=query.exec("INSERT INTO syslog VALUES('"+tm+"','"+logid+"','"+item+"','"+demo+"')");
    if(value)
    {
      // QMessageBox::information(this,"提示","参数设置操作记录成功！");
      // this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","相机4芯片二值化阈值参数修改操作记录失败！");
      // this->close();
    }
    ***********************/
}



void Dialogparaset::on_checkBox_3_clicked()
{
    /******************
    QString qcull2;
    bool value;
    QSqlQuery query,sql_query;

    sysparaset.bcullchipchk=ui->checkBox_3->isChecked();
    int a=sysparaset.bcullchipchk?1:0;
    qcull2.sprintf("%d",a);

    QString update_sql = "update syspara set bcullchipchk=:bcullchipchk";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":bcullchipchk", qcull2);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","cull芯片检测参数保存失败！");
       this->close();
    }
    ****************/
}

void Dialogparaset::on_radioButton_clicked()
{
    QString qcull;
    bool value;
    QSqlQuery query,sql_query;
    //if(ui->radioButton->isChecked())sysparaset.modelsel=0;
    //if(ui->radioButton_2->isChecked()) sysparaset.modelsel=1;
    //if(ui->radioButton_3->isChecked()) sysparaset.modelsel=2;

    qcull.sprintf("%d",sysparaset.modelsel);

    QString update_sql = "update syspara set modelsel=:modelsel";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":modelsel", qcull);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","cull芯片检测参数保存失败！");
       this->close();
    }

}

void Dialogparaset::on_radioButton_2_clicked()
{
    QString qcull;
    bool value;
    QSqlQuery query,sql_query;
    //if(ui->radioButton->isChecked())sysparaset.modelsel=0;
    //if(ui->radioButton_2->isChecked()) sysparaset.modelsel=1;
   // if(ui->radioButton_3->isChecked()) sysparaset.modelsel=2;

    qcull.sprintf("%d",sysparaset.modelsel);

    QString update_sql = "update syspara set modelsel=:modelsel";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":modelsel", qcull);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","cull芯片检测参数保存失败！");
       this->close();
    }

}

void Dialogparaset::on_radioButton_3_clicked()
{
    QString qcull;
    bool value;
    QSqlQuery query,sql_query;
    //if(ui->radioButton->isChecked())sysparaset.modelsel=0;
   // if(ui->radioButton_2->isChecked()) sysparaset.modelsel=1;
   // if(ui->radioButton_3->isChecked()) sysparaset.modelsel=2;

    qcull.sprintf("%d",sysparaset.modelsel);

    QString update_sql = "update syspara set modelsel=:modelsel";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":modelsel", qcull);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","cull芯片检测参数保存失败！");
       this->close();
    }

}



void Dialogparaset::on_lineEditplcip2_returnPressed()
{
    QString qplcip;
    bool value;
    QSqlQuery query,sql_query;

    sysparaset.addplc1=ui->lineEditplcip2->text();
    qplcip=sysparaset.addplc1;
    QString update_sql = "update syspara set addplc1=:addplc1";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":addplc1", qplcip);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","PLC2 IP参数保存失败！");
       this->close();
    }
}

void Dialogparaset::on_lineEditplcip3_returnPressed()
{
    QString qplcip;
    bool value;
    QSqlQuery query,sql_query;

    sysparaset.addplc2=ui->lineEditplcip3->text();
    qplcip=sysparaset.addplc2;
    QString update_sql = "update syspara set addplc2=:addplc2";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":addplc2", qplcip);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","PLC2 IP参数保存失败！");
       this->close();
    }
}

void Dialogparaset::on_lineEditplcport2_returnPressed()
{
    QString qplcport;
    bool value;
    QSqlQuery query,sql_query;


    sysparaset.portplc1=ui->lineEditplcport2->text().toInt();
    qplcport.sprintf("%d",sysparaset.portplc1);

    QString update_sql = "update syspara set portplc1=:portplc1";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":portplc1", qplcport);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","PLC2 端口参数保存失败！");
       this->close();
    }
}

void Dialogparaset::on_lineEditplcport3_returnPressed()
{
    QString qplcport;
    bool value;
    QSqlQuery query,sql_query;


    sysparaset.portplc2=ui->lineEditplcport3->text().toInt();
    qplcport.sprintf("%d",sysparaset.portplc2);

    QString update_sql = "update syspara set portplc2=:portplc2";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":portplc2", qplcport);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","PLC2 端口参数保存失败！");
       this->close();
    }
}
/**********************
void Dialogparaset::on_checkBox_clicked(bool checked)
{

    QString qplcport;
    bool value;
    QSqlQuery query,sql_query;

    if(checked)sysparaset.bpp=1;
    else sysparaset.bpp=0;
   // sysparaset.portplc2=ui->lineEditplcport3->text().toInt();
    qplcport.sprintf("%d",sysparaset.bpp);

    QString update_sql = "update syspara set pp=:bpp";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":bpp", qplcport);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","参数保存失败！");
       this->close();
    }
}


void Dialogparaset::on_checkBox_2_clicked(bool checked)
{
    QString qplcport;
    bool value;
    QSqlQuery query,sql_query;

    if(checked)sysparaset.bsf=1;
    else sysparaset.bsf=0;
   // sysparaset.portplc2=ui->lineEditplcport3->text().toInt();
    qplcport.sprintf("%d",sysparaset.bsf);

    QString update_sql = "update syspara set sf=:bsf";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":bsf", qplcport);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","参数保存失败！");
       this->close();
    }
}
*****************/
void Dialogparaset::on_checkBox_3_clicked(bool checked)
{
    QString qplcport;
    bool value;
    QSqlQuery query,sql_query;

    if(checked)sysparaset.bsj=1;
    else sysparaset.bsj=0;
   // sysparaset.portplc2=ui->lineEditplcport3->text().toInt();
    qplcport.sprintf("%d",sysparaset.bsj);

    QString update_sql = "update syspara set sj=:bsj";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":bsj", qplcport);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","参数保存失败！");
       this->close();
    }
}

void Dialogparaset::on_checkBox_4_clicked(bool checked)
{
    QString qplcport;
    bool value;
    QSqlQuery query,sql_query;

    if(checked)sysparaset.bsl=1;
    else sysparaset.bsl=0;
   // sysparaset.portplc2=ui->lineEditplcport3->text().toInt();
    qplcport.sprintf("%d",sysparaset.bsl);

    QString update_sql = "update syspara set sl=:bsl";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":bsl", qplcport);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","参数保存失败！");
       this->close();
    }
}

void Dialogparaset::on_lineEditdx_returnPressed()
{
    QString qdx;
    bool value;
    QSqlQuery query,sql_query;


    sysparaset.dx=ui->lineEditdx->text().toDouble();
    qdx.sprintf("%f",sysparaset.dx);

    QString update_sql = "update syspara set dx=:dx";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":dx", qdx);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","参数dx保存失败！");
       this->close();
    }
}

void Dialogparaset::on_lineEditdy_returnPressed()
{
    QString qdy;
    bool value;
    QSqlQuery query,sql_query;


    sysparaset.dy=ui->lineEditdy->text().toDouble();
    qdy.sprintf("%f",sysparaset.dy);

    QString update_sql = "update syspara set dy=:dy";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":dy", qdy);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","参数dy保存失败！");
       this->close();
    }
}

void Dialogparaset::on_lineEditdz_returnPressed()
{
    QString qdz;
    bool value;
    QSqlQuery query,sql_query;


    sysparaset.dz=ui->lineEditdz->text().toDouble();
    qdz.sprintf("%f",sysparaset.dz);

    QString update_sql = "update syspara set dz=:dz";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":dz", qdz);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","参数dz保存失败！");
       this->close();
    }
}

void Dialogparaset::on_lineEditinpara00_returnPressed()
{
    QString q00;
    bool value;
    QSqlQuery query,sql_query;

    In_parameter(0,0)=ui->lineEditinpara00->text().toDouble();
    //q00.sprintf("%f",In_parameter(0,0));
    q00=QString::number(In_parameter(0,0),'f',18);//这样就保留了小数点后x位小数

    QString update_sql = "update syspara set inpara00=:inpara00";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":inpara00", q00);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","相机内参矩阵保存失败！");
       this->close();
    }
}

void Dialogparaset::on_lineEditinpara01_returnPressed()
{
    QString q01;
    bool value;
    QSqlQuery query,sql_query;

    In_parameter(0,1)=ui->lineEditinpara01->text().toDouble();
    //q01.sprintf("%f",In_parameter(0,1));
    q01=QString::number(In_parameter(0,1),'f',18);

    QString update_sql = "update syspara set inpara01=:inpara01";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":inpara01", q01);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","相机内参矩阵保存失败！");
       this->close();
    }
}

void Dialogparaset::on_lineEditinpara02_returnPressed()
{
    QString q02;
    bool value;
    QSqlQuery query,sql_query;

    In_parameter(0,2)=ui->lineEditinpara02->text().toDouble();
    //q02.sprintf("%f",In_parameter(0,2));
    q02=QString::number(In_parameter(0,2),'f',18);

    QString update_sql = "update syspara set inpara02=:inpara02";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":inpara02", q02);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","相机内参矩阵保存失败！");
       this->close();
    }
}

void Dialogparaset::on_lineEditinpara10_returnPressed()
{
    QString q10;
    bool value;
    QSqlQuery query,sql_query;

    In_parameter(1,0)=ui->lineEditinpara10->text().toDouble();
    //q10.sprintf("%f",In_parameter(1,0));
    q10=QString::number(In_parameter(1,0),'f',18);

    QString update_sql = "update syspara set inpara10=:inpara10";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":inpara10", q10);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","相机内参矩阵保存失败！");
       this->close();
    }
}

void Dialogparaset::on_lineEditinpara11_returnPressed()
{
    QString q11;
    bool value;
    QSqlQuery query,sql_query;

    In_parameter(1,1)=ui->lineEditinpara11->text().toDouble();
    //q11.sprintf("%f",In_parameter(1,1));
    q11=QString::number(In_parameter(1,1),'f',18);

    QString update_sql = "update syspara set inpara11=:inpara11";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":inpara11", q11);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","相机内参矩阵保存失败！");
       this->close();
    }
}

void Dialogparaset::on_lineEditinpara12_returnPressed()
{
    QString q12;
    bool value;
    QSqlQuery query,sql_query;

    In_parameter(1,2)=ui->lineEditinpara12->text().toDouble();
    //q12.sprintf("%f",In_parameter(1,2));
    q12=QString::number(In_parameter(1,2),'f',18);

    QString update_sql = "update syspara set inpara12=:inpara12";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":inpara12", q12);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","相机内参矩阵保存失败！");
       this->close();
    }
}

void Dialogparaset::on_lineEditinpara20_returnPressed()
{
    QString q20;
    bool value;
    QSqlQuery query,sql_query;

    In_parameter(2,0)=ui->lineEditinpara20->text().toDouble();
    //q20.sprintf("%f",In_parameter(2,0));
    q20=QString::number(In_parameter(2,0),'f',18);

    QString update_sql = "update syspara set inpara20=:inpara20";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":inpara20", q20);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","相机内参矩阵保存失败！");
       this->close();
    }
}

void Dialogparaset::on_lineEditinpara21_returnPressed()
{
    QString q21;
    bool value;
    QSqlQuery query,sql_query;

    In_parameter(2,1)=ui->lineEditinpara21->text().toDouble();
    q21.sprintf("%f",In_parameter(2,1));
    q21=QString::number(In_parameter(2,1),'f',18);

    QString update_sql = "update syspara set inpara21=:inpara21";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":inpara21", q21);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","相机内参矩阵保存失败！");
       this->close();
    }
}

void Dialogparaset::on_lineEditinpara22_returnPressed()
{
    QString q22;
    bool value;
    QSqlQuery query,sql_query;

    In_parameter(2,2)=ui->lineEditinpara22->text().toDouble();
    q22.sprintf("%f",In_parameter(2,2));
    q22=QString::number(In_parameter(2,2),'f',18);

    QString update_sql = "update syspara set inpara22=:inpara22";
    sql_query.prepare(update_sql);

    sql_query.bindValue(":inpara22", q22);

    value=sql_query.exec();

    if(value)
    {
      // QMessageBox::information(this,"提示","保存参数成功！");
     //  this->close();
    }
    else
    {
       QMessageBox::information(this,"提示","相机内参矩阵保存失败！");
       this->close();
    }
}
