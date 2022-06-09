#include "dialogauto.h"
#include "ui_dialogauto.h"
#include <QMessageBox>
#include "ComData.h"
#include <QSqlQuery>

Dialogauto::Dialogauto(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialogauto)
{
    ui->setupUi(this);
    mainLayout1=new QVBoxLayout();
    mainLayout1->setSpacing(0);
    mainLayout1->setContentsMargins(0,2,0,40);

    sels=new myGauge2(ui->groupBoxsels);//     frmsels);//test);
    sels->setAnimating(false);//true

    mainLayout1->addWidget(sels);

   // ui->frmsels->setLayout(mainLayout1);
    ui->groupBoxsels->setLayout(mainLayout1);
    //sels->setVisible(true);
    sels->initVariables("手动","就地","远方");
    int value=50;
    sels->setValue(value);
    //-----------------------------------------------
    mainLayout2=new QVBoxLayout();
    mainLayout2->setSpacing(0);
    mainLayout2->setContentsMargins(0,0,0,40);
    bres=new myGauge2(ui->groupBoxbres);//     frmbres);//test);
    bres->setAnimating(false);//true

    mainLayout2->addWidget(bres);

   // ui->frmbres->setLayout(mainLayout1);
    ui->groupBoxbres->setLayout(mainLayout2);
    //bres->setVisible(true);
    bres->initVariables("分闸","O","合闸");
    value=70;
    bres->setValue(value);
   //------------------------------------------------------
    mainLayout3=new QVBoxLayout();
    mainLayout3->setSpacing(0);
    mainLayout3->setContentsMargins(0,0,0,40);
    diss=new myGauge2(ui->groupBoxdiss);//     frmdiss);//test);
    diss->setAnimating(false);//true

    mainLayout3->addWidget(diss);

   // ui->frmdiss->setLayout(mainLayout1);
    ui->groupBoxdiss->setLayout(mainLayout3);
    //diss->setVisible(true);
    diss->initVariables("分闸","O","合闸");
    value=30;
    diss->setValue(value);
   //-------------------------------------------------------

    mainLayout4=new QVBoxLayout();
    mainLayout4->setSpacing(0);
    mainLayout4->setContentsMargins(0,0,0,40);
    ears=new myGauge2(ui->groupBoxears);//     frmears);//test);
    ears->setAnimating(false);//true

    mainLayout4->addWidget(ears);

   // ui->frmears->setLayout(mainLayout1);
    ui->groupBoxears->setLayout(mainLayout4);
    //ears->setVisible(true);
    ears->initVariables("分闸","O","合闸");
    value=50;
    ears->setValue(value);
    QSqlQuery query;
    bool value1=query.exec("select * from kkpara order by no");
    if(!value1) QMessageBox::information(this,"提示","开关参数表连接接失败！");

    while(query.next())//指向第一条记录
    {
      QString selno=query.value(1).toString().simplified();
      if(selno.left(2)!="00")
      ui->comboBox->addItem(selno);


    }

    ui->comboBox->setCurrentIndex(0);
    QString initno=ui->comboBox->currentText();
    int idx=kkno2index(initno);
    gequid=kkparaset[idx].id;
    int idxid=equid2index(gequid);
    int room=equparaset[idxid].roomno;
    if(room==1)//1 room
    {
      if(initno.right(2)=="01")
      {
          ui->pushButton2->show();
          ui->pushButton0->setText("手动");
          ui->pushButton2->setText("就地");
          ui->pushButton1->setText("远方");

      }
      else
      {
          ui->pushButton2->hide();
          ui->pushButton0->setText("分/左");
         // ui->pushButton0->setText("就地");
          ui->pushButton1->setText("合/右");
      }
    }



    //ui->tabWidget->setCurrentIndex(0);
    gchg1=true;

    timerstu = new QTimer(this);
    connect(timerstu,SIGNAL(timeout()),this,SLOT(dispstu()));
    timerstu->start(1000);

    update();
}

void Dialogauto::dispstu()//disp
{
  //测试
  //室1
  if(runmodel!=1)return;
  if(1)//柜或开关发生变化时???
  {
      int idx=equid2index(gequid);
      ui->labelno->setText(equparaset[idx].no);
      ui->labelname->setText(equparaset[idx].title);

      for(int i=0;i<nkk;i++)
       if(kkparaset[i].id==gequid)
       {
           switch(kkparaset[i].pos)
           {
             case 1:switch(kkparaset[i].value)
                    {
                      case -1:sels->setValue(30);break;
                      case 1:sels->setValue(70);break;
                      case 0:sels->setValue(50);break;
                    }
                    break;
             case 2:switch(kkparaset[i].value)
                    {
                     case -1:bres->setValue(30);break;
                     case 1:bres->setValue(70);break;
                    }
                    break;
             case 3:switch(kkparaset[i].value)
                    {
                     case -1:diss->setValue(30);break;
                     case 1:diss->setValue(70);break;
                    }
                     break;
             case 4:switch(kkparaset[i].value)
                    {
                      case -1:ears->setValue(30);break;
                      case 1:ears->setValue(70);break;
                     }
                     break;
            }

        }

        gchg1=false;
        update();

     }//if gchg1





}

Dialogauto::~Dialogauto()
{
    delete ui;
}

void Dialogauto::paintEvent(QPaintEvent *)
{
    QImage image;
    QPixmap pix;
    QString pic1pon,pic2pon,pic3pon,pic1poff,pic2poff,pic3poff;
    QString strdir;
    pic1pon="1pon.png";
    pic2pon="2pon.png";
    pic3pon="3pon.png";
    pic1poff="1poff.png";
    pic2poff="2poff.png";
    pic3poff="3poff.png";

    strdir.sprintf("%s/",dirpf);
    pic1pon=strdir+pic1pon;
    pic2pon=strdir+pic2pon;
    pic3pon=strdir+pic3pon;
    pic1poff=strdir+pic1poff;
    pic2poff=strdir+pic2poff;
    pic3poff=strdir+pic3poff;

    int idx=equid2index(gequid);
    if(equparaset[idx].type==2)
    {
        ui->labelzk2->hide();
        ui->labelkk2name->hide();
    }
    else
    {
        ui->labelzk2->show();
        ui->labelkk2name->show();
    }

    for(int i=0;i<nkk;i++)
    {
     if(kkparaset[i].id==gequid)
     {
         switch(kkparaset[i].pos)
         {
           case 5:switch(kkparaset[i].value)
                  {
                    case 0:if(equparaset[idx].type==3)image.load(pic1poff);
                           else image.load(pic2poff);
                           ui->labelkk1name->setText(kkparaset[i].name);
                           break;
                    case 1:if(equparaset[idx].type==3)image.load(pic1pon);
                           else image.load(pic2pon);
                           ui->labelkk1name->setText(kkparaset[i].name);
                           break;
                  }
                  break;
           case 6:switch(kkparaset[i].value)
                  {
                   case 0:image.load(pic1poff);
                          ui->labelkk2name->setText(kkparaset[i].name);
                          break;
                   case 1:image.load(pic1pon);
                          ui->labelkk2name->setText(kkparaset[i].name);
                          break;
                  }
                  break;
           case 7:switch(kkparaset[i].value)
                  {
                   case 0:image.load(pic1poff);
                        //  ui->labelkk3name->setText(kkparaset[i].name);
                          break;
                   case 1:image.load(pic1pon);
                         // ui->labelkk3name->setText(kkparaset[i].name);
                          break;
                  }
                  break;
          }

         //************
         //QPainter painter(this);
           pix = QPixmap::fromImage(image);//.scaled(size(), Qt::KeepAspectRatio) );
       //  ui->label->resize(651,621);//**********************************************************8
         if(kkparaset[i].pos==5)
         {
          ui->labelzk1->setPixmap(pix );
          ui->labelzk1->setScaledContents(true);
         }
         if(kkparaset[i].pos==6)
         {
          ui->labelzk2->setPixmap(pix );
          ui->labelzk2->setScaledContents(true);
         }
         if(kkparaset[i].pos==7)
         {
          ui->labelzk3->setPixmap(pix );
          ui->labelzk3->setScaledContents(true);
         }
        // ***************
      }


    }//endfor

}

void Dialogauto::kkopt(QString no,int st)//开关编号，到达状态
{
    int kkidx;
    QSqlQuery query;
    bool value;
    double offsetx,offsety,offsetz,offsetrx,offsetry,offsetrz;
    QString qsl,qst;
    gno=no;
    gstate=st;
    kkidx=kkno2index(no);
    if(kkparaset[kkidx].value==st)
    {
        QMessageBox::information(this,"提示",kkparaset[kkidx].name+"已达到状态，请检查！");
        return;
    }

    //合闸操作
    emit Sendquid(kkparaset[kkidx].id);//send to mainwindow


    //修改状态及记录,以后删除，该在Process线程中开关操作完成时修改
    /****************
    kkparaset[kkidx].value=st;
    qst=QString::number(st);
    qsl="update kkpara set value='" + qst + "' where no='" + no + "'";
    value=query.exec(qsl);//"update equpara set x="+qx+",y="+qy+",z="+qz+",rx="+qrx+",ry="+qry+",rz="+qrz+" where id="+q0);
    if(value) QMessageBox::information(this,"提示","状态修改成功！以后需要改到Process线程中...");
    else QMessageBox::information(this,"提示","状态修改失败！");
    ***************/
}


void Dialogauto::on_pushButton_clicked()
{

}

void Dialogauto::on_comboBox_currentTextChanged(const QString &arg1)
{
    QString kkno=arg1;
    gno=kkno;
    //QMessageBox::information(this,"提示",kkno);
    int idx=kkno2index(kkno);
    gequid=kkparaset[idx].id;
    int idxid=equid2index(gequid);
    ui->labelno->setText(equparaset[idxid].no);
    ui->labelname->setText(equparaset[idxid].title);
    int room=equparaset[idxid].roomno;
    if(room==1)//1 room
    {
      if(kkno.right(2)=="01")
      {
          ui->pushButton2->show();
          ui->pushButton0->setText("手动");
          ui->pushButton2->setText("就地");
          ui->pushButton1->setText("远方");

      }
      else
      {
          ui->pushButton2->hide();
          ui->pushButton0->setText("分");
         // ui->pushButton0->setText("就地");
          ui->pushButton1->setText("合");
      }
    }

}

void Dialogauto::on_pushButton0_clicked()
{
    int idx=kkno2index(gno);
    int type=kkparaset[idx].type;
    if(type<20)gstate=0;
    else gstate=-1;
    kkopt(gno,gstate);
}

void Dialogauto::on_pushButton2_clicked()
{
    gstate=0;
    kkopt(gno,gstate);
}

void Dialogauto::on_pushButton1_clicked()
{

    gstate=1;
    kkopt(gno,gstate);
}
