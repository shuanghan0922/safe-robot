#include "dialogequset.h"
#include "ui_dialogequset.h"
#include <QSqlRecord>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include <QSqlError>
#include "ComData.h"
//#include <QMainWindow>
//#include "webcmd.h"

Dialogequset::Dialogequset(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialogequset)
{
    ui->setupUi(this);
   // dlgkk=new Dialogkkset(this);
    model=new QSqlTableModel(this);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
   // model->setQuery("select * from book");
    model->setTable("equpara");

    model->select();

    model->setHeaderData(0,Qt::Horizontal,tr("ID"));
    model->setHeaderData(1,Qt::Horizontal,tr("柜号"));
    model->setHeaderData(2,Qt::Horizontal,tr("名称"));
    model->setHeaderData(3,Qt::Horizontal,tr("室号"));
    model->setHeaderData(4,Qt::Horizontal,tr("类型"));
    model->setHeaderData(5,Qt::Horizontal,tr("JAKA x"));
    model->setHeaderData(6,Qt::Horizontal,tr("JAKA y"));
    model->setHeaderData(7,Qt::Horizontal,tr("JAKA z"));
    model->setHeaderData(8,Qt::Horizontal,tr("JAKA rx"));
    model->setHeaderData(9,Qt::Horizontal,tr("JAKA ry"));
    model->setHeaderData(10,Qt::Horizontal,tr("JAKA rz"));
    model->setHeaderData(11,Qt::Horizontal,tr("ROS shx"));
    model->setHeaderData(12,Qt::Horizontal,tr("ROS shy"));
    model->setHeaderData(13,Qt::Horizontal,tr("ROS shz"));
    model->setHeaderData(14,Qt::Horizontal,tr("ROS shx1"));
    model->setHeaderData(15,Qt::Horizontal,tr("ROS shy1"));
    model->setHeaderData(16,Qt::Horizontal,tr("ROS shz1"));
    model->setHeaderData(17,Qt::Horizontal,tr("ROS shw1"));
    model->setHeaderData(18,Qt::Horizontal,tr("高度增量"));

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
                                                         //(a)
    ui->tableView->setSortingEnabled(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
   // ui->tableView->setShowGrid(false);
    ui->tableView->setAlternatingRowColors(true);

    ui->tableView->setModel(model);
    ui->widget->hide();
    ui->frame->hide();

    ui->comboBoxpos->addItem("室外");
    ui->comboBoxpos->addItem("室内1");
    ui->comboBoxpos->addItem("室内2");
    ui->comboBoxpos->addItem("测试");
    //类型，柜体开关布局，界面设计，test:固定/0
                            //室1：按空开设置分：0：zkk1/zk1/zk2,1：zkk1/zk1,2：fb61/fb62/zk1
                            //室2：按路数设置分：
    ui->comboBoxtype->addItem("测试");
    ui->comboBoxtype->addItem("室1:zkk1/zk1/zk2");
    ui->comboBoxtype->addItem("室1:zkk1/zk1");
    ui->comboBoxtype->addItem("室1:fb61/fb62/zk1");
    ui->comboBoxtype->addItem("室2:4路");
    ui->comboBoxtype->addItem("室2:3路");
    ui->comboBoxtype->addItem("室2:2路");
    ui->comboBoxtype->addItem("室2:2路+1");

    /****************
    ui->comboBoxsels->addItem("手动");
    ui->comboBoxsels->addItem("远方");
    ui->comboBoxsels->addItem("就地");

    ui->comboBoxbres->addItem("分");
    ui->comboBoxbres->addItem("合");
    ui->comboBoxbres->addItem("0");

    ui->comboBoxdiss->addItem("分");
    ui->comboBoxdiss->addItem("合");
    ui->comboBoxdiss->addItem("0");

    ui->comboBoxears->addItem("分");
    ui->comboBoxears->addItem("合");
    ui->comboBoxears->addItem("0");

    ui->comboBoxkkgrp->addItem("三工位电机/温控照明/WJCQ电源");
    ui->comboBoxkkgrp->addItem("三工位电机/温控照明");
    ui->comboBoxkkgrp->addItem("T相电压/F相电压/温控照明");
    **************/
    /***************
    ui->comboBoxkk1->addItem("分");
    ui->comboBoxkk1->addItem("合");
    ui->comboBoxkk2->addItem("分");
    ui->comboBoxkk2->addItem("合");
    ui->comboBoxkk3->addItem("分");
    ui->comboBoxkk3->addItem("合");
    *****************/
    ui->pushButtonuserchg->setEnabled(false);
    ui->pushButtonuserdel->setEnabled(false);

    ui->pushButtonok->setEnabled(false);
    ui->pushButtoncancel->setEnabled(false);
    button_style1="QPushButton{background-color:white;color: black;border-radius: 10px;  border: 2px groove gray;border-style: outset;}";

    button_style2="QPushButton{background-color:rgb(235, 142, 85);border-radius: 10px;  border: 2px groove gray;border-style: inset; }";
}

void Dialogequset::disprospos()
{
    QString qd;
    //qd.sprintf("%f",rospos.x);
    qd=QString::number(rospos.x,'f',20);
    ui->lineEditx1_2->setText(qd);
    //qd.sprintf("%f",rospos.y);
    qd=QString::number(rospos.y,'f',20);
    ui->lineEdity1_2->setText(qd);
    //qd.sprintf("%f",rospos.z);
    qd=QString::number(rospos.z,'f',20);
    ui->lineEditz1_2->setText(qd);
    //qd.sprintf("%f",rospos.x1);
    qd=QString::number(rospos.x1,'f',20);
    ui->lineEditx2_2->setText(qd);
    //qd.sprintf("%f",rospos.y1);
    qd=QString::number(rospos.y1,'f',20);
    ui->lineEdity2_2->setText(qd);
    //qd.sprintf("%f",rospos.z1);
    qd=QString::number(rospos.z1,'f',20);
    ui->lineEditz2_2->setText(qd);
    //qd.sprintf("%f",rospos.w1);
    qd=QString::number(rospos.w1,'f',20);
    ui->lineEditw2_2->setText(qd);
}

Dialogequset::~Dialogequset()
{
    //dlgkk->close();
    delete dlgkk;
    delete ui;

}

void Dialogequset::on_pushButtonuseradd_clicked()
{
    ui->widget->show();
    ui->lineEditidx->setText("");
    ui->lineEditno->setText("");
    ui->lineEdittitle->setText("");
    ui->lineEdithigh->setText("");

    ui->pushButtonok->setEnabled(true);
    ui->pushButtoncancel->setEnabled(true);
    optsel=1; //add
    ui->pushButtonuseradd->setStyleSheet(button_style2);//
    ui->pushButtonuserchg->setStyleSheet(button_style1);
    ui->pushButtonuserdel->setStyleSheet(button_style1);//

    ui->lineEditidx->setEnabled(true);
    ui->lineEditno->setEnabled(true);
    ui->lineEdittitle->setEnabled(true);
    ui->lineEdithigh->setEnabled(true);
    ui->comboBoxpos->setEnabled(true);
    ui->comboBoxtype->setEnabled(true);

    ui->lineEditx->setEnabled(true);//JAKA
    ui->lineEdity->setEnabled(true);
    ui->lineEditz->setEnabled(true);
    ui->lineEditrx->setEnabled(true);
    ui->lineEditry->setEnabled(true);
    ui->lineEditrz->setEnabled(true);

    ui->lineEditx1->setEnabled(true);//ROSinpos=1;
    ui->lineEdity1->setEnabled(true);
    ui->lineEditz1->setEnabled(true);
    ui->lineEditx2->setEnabled(true);
    ui->lineEdity2->setEnabled(true);
    ui->lineEditz2->setEnabled(true);
    ui->lineEditw2->setEnabled(true);
    ui->pushButtonuserchg->setEnabled(false);
    ui->pushButtonuserdel->setEnabled(false);
}

void Dialogequset::on_comboBoxkkgrp_currentIndexChanged(int index)
{
      /***********************
   if(index==0)
   {

       ui->labelkk1->setText("zzk1");
       ui->labelkk2->setText("zk1");
       ui->labelkk3->setText("zk2");
      // ui->labelkk1_2->setText("zzk1");
      // ui->labelkk2_2->setText("zk1");
      // ui->labelkk3_2->setText("zk2");
       ui->lineEditkk1->setText("");
       ui->lineEditkk2->setText("");
       ui->lineEditkk3->setText("");
       ui->lineEditname1->setText("");
       ui->lineEditname2->setText("");
       ui->lineEditname3->setText("");
       ui->labelkk3->show();
       ui->labelkk3_2->show();
       ui->lineEditkk3->show();
       ui->lineEditname3->show();
   }

   if(index==1)
   {
       ui->labelkk1->setText("zk1");
       ui->labelkk2->setText("zk2");
       ui->labelkk3->setText("");
      // ui->labelkk1_2->setText("");
      // ui->labelkk2_2->setText("zk1");
      // ui->labelkk3_2->setText("zk2");
       ui->labelkk3->hide();
       ui->labelkk3_2->hide();

       ui->lineEditkk1->setText("");
       ui->lineEditkk2->setText("");
       ui->lineEditkk3->setText("");
       ui->lineEditname1->setText("");
       ui->lineEditname2->setText("");
       ui->lineEditname3->setText("");

       ui->lineEditkk3->hide();
       ui->lineEditname3->hide();

   }

   if(index==2)
   {
       ui->labelkk1->setText("FB61");
       ui->labelkk2->setText("FB62");
       ui->labelkk3->setText("zk1");

       ui->lineEditkk1->setText("");
       ui->lineEditkk2->setText("");
       ui->lineEditkk3->setText("");
       ui->lineEditname1->setText("");
       ui->lineEditname2->setText("");
       ui->lineEditname3->setText("");
       ui->labelkk3->show();
       ui->labelkk3_2->show();
       ui->lineEditkk3->show();
       ui->lineEditname3->show();
   }
**************/
    //ui->groupBoxkkp->show();
}

void Dialogequset::on_pushButtonuserdel_clicked()
{
    ui->pushButtonok->setEnabled(true);
    ui->pushButtoncancel->setEnabled(true);
    optsel=3; //del
    ui->pushButtonuseradd->setStyleSheet(button_style1);//
    ui->pushButtonuserchg->setStyleSheet(button_style1);
    ui->pushButtonuserdel->setStyleSheet(button_style2);//

    ui->lineEditidx->setEnabled(false);
    ui->lineEditno->setEnabled(false);
    ui->lineEdittitle->setEnabled(false);
    ui->lineEdithigh->setEnabled(false);
    ui->comboBoxpos->setEnabled(false);
    ui->comboBoxtype->setEnabled(false);

    ui->lineEditx->setEnabled(false);
    ui->lineEdity->setEnabled(false);
    ui->lineEditz->setEnabled(false);
    ui->lineEditrx->setEnabled(false);
    ui->lineEditry->setEnabled(false);
    ui->lineEditrz->setEnabled(false);

    ui->lineEditx1->setEnabled(false);
    ui->lineEdity1->setEnabled(false);
    ui->lineEditz1->setEnabled(false);
    ui->lineEditx2->setEnabled(false);
    ui->lineEdity2->setEnabled(false);
    ui->lineEditz2->setEnabled(false);
    ui->lineEditw2->setEnabled(false);

    ui->pushButtonuseradd->setEnabled(false);//
    ui->pushButtonuserchg->setEnabled(false);
}

void Dialogequset::on_pushButtonuserchg_clicked()
{
    ui->pushButtonok->setEnabled(true);
    ui->pushButtoncancel->setEnabled(true);
    optsel=2; //chg
    ui->pushButtonuseradd->setStyleSheet(button_style1);//
    ui->pushButtonuserchg->setStyleSheet(button_style2);
    ui->pushButtonuserdel->setStyleSheet(button_style1);//

    ui->lineEditidx->setEnabled(true);
    ui->lineEditno->setEnabled(true);
    ui->lineEdittitle->setEnabled(true);
    ui->lineEdithigh->setEnabled(true);
    ui->comboBoxpos->setEnabled(true);
    ui->comboBoxtype->setEnabled(true);

    ui->lineEditx->setEnabled(true);
    ui->lineEdity->setEnabled(true);
    ui->lineEditz->setEnabled(true);
    ui->lineEditrx->setEnabled(true);
    ui->lineEditry->setEnabled(true);
    ui->lineEditrz->setEnabled(true);

    ui->lineEditx1->setEnabled(true);
    ui->lineEdity1->setEnabled(true);
    ui->lineEditz1->setEnabled(true);
    ui->lineEditx2->setEnabled(true);
    ui->lineEdity2->setEnabled(true);
    ui->lineEditz2->setEnabled(true);
    ui->lineEditw2->setEnabled(true);
    ui->pushButtonuserdel->setEnabled(false);
    ui->pushButtonuseradd->setEnabled(false);
}

void Dialogequset::on_pushButtonok_clicked()
{
    if(optsel==1)//add
    {
        QSqlQuery query;

        qindex=ui->lineEditidx->text();
        qno=ui->lineEditno->text();
        qtitle=ui->lineEdittitle->text();
        qdz=ui->lineEdithigh->text().toDouble();
        qpos=ui->comboBoxpos->currentIndex();
        qtype=ui->comboBoxtype->currentIndex();

        sqid=qindex;
        sqtitle=qtitle;
        nlayout=qtype;
        slayout=ui->comboBoxtype->currentText();

        qjkx=ui->lineEditx->text().toDouble();
        qjky=ui->lineEdity->text().toDouble();
        qjkz=ui->lineEditz->text().toDouble();
        qjkpx=ui->lineEditrx->text().toDouble();
        qjkpy=ui->lineEditry->text().toDouble();
        qjkpz=ui->lineEditrz->text().toDouble();

        qrosx=ui->lineEditx1->text().toDouble();
        qrosy=ui->lineEdity1->text().toDouble();
        qrosz=ui->lineEditz1->text().toDouble();
        qrosx1=ui->lineEditx2->text().toDouble();
        qrosy1=ui->lineEdity2->text().toDouble();
        qrosz1=ui->lineEditz2->text().toDouble();
        qrosw1=ui->lineEditw2->text().toDouble();
        if(qindex.isEmpty()||qno.isEmpty()||qtitle.isEmpty())
        {
           QMessageBox::warning(this,tr("警告"),tr("ID号,编号和名称不能为空!"),QMessageBox::Yes);
           ui->pushButtonuseradd->setStyleSheet(button_style1);//
           ui->pushButtonuserchg->setStyleSheet(button_style1);
           ui->pushButtonuserdel->setStyleSheet(button_style1);//
           ui->pushButtonuseradd->setEnabled(true);
           ui->widget->hide();
            return;
        }
        QSqlRecord record = model->record();
         // 从1开始遍历，遇到相同的ID，退出
        // 防止唯一主键重复导致提交失败，因为提交失败tableview还是会更新修改后的（视图）
       //  bool bFlag = false;
         for(int index = 0;index < model->rowCount(); index++)
            if(model->index(index, 0).data() == qindex)
            {
                QMessageBox::information(this,"提示","该ID号已存在,退出！");
                return;
            }

      //   qDebug()<<Dialogadduser::strID;
         record.setValue(0, qindex);
         record.setValue(1, qno);
         record.setValue(2, qtitle);
         record.setValue(3, qpos);
         record.setValue(4, qtype);
         record.setValue(5, qjkx);
         record.setValue(6, qjky);
         record.setValue(7, qjkz);
         record.setValue(8, qjkpx);
         record.setValue(9, qjkpy);
         record.setValue(10, qjkpz);
         record.setValue(11, qrosx);
         record.setValue(12, qrosy);
         record.setValue(13, qrosz);
         record.setValue(14, qrosx1);
         record.setValue(15, qrosy1);
         record.setValue(16, qrosz1);
         record.setValue(17, qrosw1);
         record.setValue(18, qdz);
        model->insertRecord(model->rowCount(), record);
         // 每次手动提交，都会重新刷新tableView，保持mode和tableView一致
       //  model->submitAll();
         if (model->submitAll()) // 提交所有被修改的数据到数据库中
         {
           QMessageBox::information(this,"提示","增加成功！");
 /***********************************
           bool value;
           QString tm=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
           QString item="添加用户";
           QString user=logid;
           QString demo="添加用户信息：用户名："+qID+"姓名："+qname;
           value=query.exec("INSERT INTO syslog VALUES('"+tm+"','"+logid+"','"+item+"','"+demo+"')");
           if(value)
           {
               QMessageBox::information(this,"提示","添加用户操作记录成功！");
              // this->close();
           }
           else
           {
               QMessageBox::information(this,"提示","添加用户操作记录失败！");
              // this->close();
           }
           **********************/
         }
         else
         {
           QMessageBox::information(this,"提示","增加失败！");
             // QMessageBox::warning(this, tr("修改用户信息"),tr("数据库错误: %1").arg(model->lastError().text()));
         }

         ui->pushButtonok->setEnabled(false);
         ui->pushButtoncancel->setEnabled(false);

     }//optsel==1

    if(optsel==2) //chg
    {
        QSqlQuery query;
        QString qID1;
        //Dialoguserchg dlg;
        qindex=ui->lineEditidx->text();
        qno=ui->lineEditno->text();
        qtitle=ui->lineEdittitle->text();
        qdz=ui->lineEdithigh->text().toDouble();
        qpos=ui->comboBoxpos->currentIndex();
        qtype=ui->comboBoxtype->currentIndex();

        sqid=qindex;
        sqtitle=qtitle;
        nlayout=qtype;
        slayout=ui->comboBoxtype->currentText();

        qjkx=ui->lineEditx->text().toDouble();
        qjky=ui->lineEdity->text().toDouble();
        qjkz=ui->lineEditz->text().toDouble();
        qjkpx=ui->lineEditrx->text().toDouble();
        qjkpy=ui->lineEditry->text().toDouble();
        qjkpz=ui->lineEditrz->text().toDouble();

        qrosx=ui->lineEditx1->text().toDouble();
        qrosy=ui->lineEdity1->text().toDouble();
        qrosz=ui->lineEditz1->text().toDouble();
        qrosx1=ui->lineEditx2->text().toDouble();
        qrosy1=ui->lineEdity2->text().toDouble();
        qrosz1=ui->lineEditz2->text().toDouble();
        qrosw1=ui->lineEditw2->text().toDouble();

        if(qindex.isEmpty()||qno.isEmpty()||qtitle.isEmpty())
        {
           QMessageBox::warning(this,tr("警告"),tr("ID号,编号和名称不能为空!"),QMessageBox::Yes);
           ui->pushButtoncancel->setEnabled(false);
           ui->pushButtonok->setEnabled(false);
           ui->pushButtonuseradd->setStyleSheet(button_style1);//
           ui->pushButtonuserchg->setStyleSheet(button_style1);
           ui->pushButtonuserdel->setStyleSheet(button_style1);//
           ui->pushButtonuseradd->setEnabled(true);
           ui->widget->hide();
            return;
        }


        if(qindex!=qid)
        {
           QSqlRecord record = model->record();
            // 从1开始遍历，遇到相同的ID，退出
           // 防止唯一主键重复导致提交失败，因为提交失败tableview还是会更新修改后的（视图）#include <QComboBox>
          //  bool bFlag = false;
            for(int index = 0; index < model->rowCount(); index++)
               if(model->index(index, 0).data() == qindex)
               {
                   QMessageBox::information(this,"提示","ID号已存在,请检查和修改！");
                   ui->pushButtoncancel->setEnabled(false);
                   ui->pushButtonok->setEnabled(false);
                   ui->pushButtonuseradd->setStyleSheet(button_style1);//
                   ui->pushButtonuserchg->setStyleSheet(button_style1);
                   ui->pushButtonuserdel->setStyleSheet(button_style1);//
                   ui->pushButtonuseradd->setEnabled(true);
                   ui->widget->hide();
                   return;
               }
         }

         int row = ui->tableView->currentIndex().row();
     //  ui->tableView->setSortingEnabled(true);

         QModelIndex index_Id = model->index(row, 0);//选中行第1列的内容
         QModelIndex index_No = model->index(row, 1);//选中行第2列的内容
         QModelIndex index_Title = model->index(row, 2);//选中行第3列的内容
         QModelIndex index_Pos= model->index(row, 3);//选中行第4列的内容
         QModelIndex index_Type = model->index(row, 4);//选中行第5列的内
         QModelIndex index_Jkx = model->index(row, 5);//选中行第3列的内容
         QModelIndex index_Jky= model->index(row, 6);//选中行第4列的内容
         QModelIndex index_Jkz = model->index(row, 7);//选中行第5列的内
         QModelIndex index_Jkpx = model->index(row, 8);
         QModelIndex index_Jkpy = model->index(row, 9);
         QModelIndex index_Jkpz = model->index(row, 10);
         QModelIndex index_Rosx = model->index(row, 11);
         QModelIndex index_Rosy = model->index(row, 12);
         QModelIndex index_Rosz = model->index(row, 13);
         QModelIndex index_Rosx1 = model->index(row, 14);
         QModelIndex index_Rosy1 = model->index(row, 15);
         QModelIndex index_Rosz1 = model->index(row, 16);
         QModelIndex index_Rosw1 = model->index(row, 17);
         QModelIndex index_Dz = model->index(row, 18);

         model->setData(index_Id, QVariant::fromValue(qindex));
         model->setData(index_No, QVariant::fromValue(qno));
         model->setData(index_Title, QVariant::fromValue(qtitle));
         model->setData(index_Pos, QVariant::fromValue(qpos));
         model->setData(index_Type, QVariant::fromValue(qtype));
         model->setData(index_Jkx, QVariant::fromValue(qjkx));
         model->setData(index_Jky, QVariant::fromValue(qjky));
         model->setData(index_Jkz, QVariant::fromValue(qjkz));
         model->setData(index_Jkpx, QVariant::fromValue(qjkpx));
         model->setData(index_Jkpy, QVariant::fromValue(qjkpy));
         model->setData(index_Jkpz, QVariant::fromValue(qjkpz));
         model->setData(index_Rosx, QVariant::fromValue(qrosx));
         model->setData(index_Rosy, QVariant::fromValue(qrosy));
         model->setData(index_Rosz, QVariant::fromValue(qrosz));
         model->setData(index_Rosx1, QVariant::fromValue(qrosx1));
         model->setData(index_Rosy1, QVariant::fromValue(qrosy1));
         model->setData(index_Rosz1, QVariant::fromValue(qrosz1));
         model->setData(index_Rosw1, QVariant::fromValue(qrosw1));
         model->setData(index_Dz, QVariant::fromValue(qdz));
      // /**********************
      // QSqlTableModel *pMode = dynamic_cast<QSqlTableModel *>(ui->tableView->model());
       model->database().transaction(); //开始事务操作
       if (model->submitAll()) // 提交所有被修改的数据到数据库中
       {
           int idx=equid2index(qindex);
           model->database().commit(); //提交成功，事务将真正修改数据库数据
           equparaset[idx].id=qindex;
           equparaset[idx].no=qno;
           equparaset[idx].title=qtitle;
           equparaset[idx].roomno=qpos;
           equparaset[idx].type=qtype;
           equparaset[idx].x=qjkx;
           equparaset[idx].y=qjky;
           equparaset[idx].z=qjkz;
           equparaset[idx].rx=qjkpx;
           equparaset[idx].ry=qjkpy;
           equparaset[idx].rz=qjkpz;
           equparaset[idx].shx=qrosx;
           equparaset[idx].shy=qrosy;
           equparaset[idx].shz=qrosz;
           equparaset[idx].shx1=qrosx1;
           equparaset[idx].shy1=qrosy1;
           equparaset[idx].shz1=qrosz1;
           equparaset[idx].shw1=qrosw1;
           equparaset[idx].calibration_dz=qdz;

           /***************************
           bool value;
           QString tm=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
           QString item="修改用户";
           QString user=logid;
           QString demo="修改用户信息：用户名："+qID+"用户姓名："+qname;
           value=query.exec("INSERT INTO syslog VALUES('"+tm+"','"+logid+"','"+item+"','"+demo+"')");
           if(value)
           {
               QMessageBox::information(this,"提示","修改用户信息操作记录成功！");

              // this->close();
           }
           else
           {
               QMessageBox::information(this,"提示","修改用户信息操作记录失败！");
              // this->close();
           }
           ************************/
       }
       else
       {
           model->database().rollback(); //提交失败，事务回修改产品滚
           QMessageBox::warning(this, tr("修改用户信息"),tr("数据库错误: %1").arg(model->lastError().text()));
           model->revertAll(); //撤销修改
       }
       ui->pushButtonok->setEnabled(false);
       ui->pushButtonuserchg->setEnabled(false);
       ui->pushButtonuserdel->setEnabled(false);

    }//optsel==2

    if(optsel==3) //del
    {

        QSqlQuery query;
        model->removeRow(ui->tableView->currentIndex().row());
           // 每次手动提交，都会重新刷新tableView，保持mode和tableView一致
       // model->submitAll();
        if (model->submitAll()) // 提交所有被修改的数据到数据库中
        {
          QMessageBox::information(this,"提示","删除用户成功！");
          /****************************
          bool value;
          QString tm=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
          QString item="删除用户";
          QString user=logid;
          QString demo="删除用户信息：用户名："+qID+"姓名："+qname;
          value=query.exec("INSERT INTO syslog VALUES('"+tm+"','"+logid+"','"+item+"','"+demo+"')");
          if(value)
          {
              QMessageBox::information(this,"提示","删除用户操作记录成功！");
             // this->close();
          }
          else
          {
              QMessageBox::information(this,"提示","删除用户操作记录失败！");
             // this->close();
          }
          **********************/
        }
        else
        {
          QMessageBox::information(this,"提示","删除用户失败！");
            // QMessageBox::warning(this, tr("修改用户信息"),tr("数据库错误: %1").arg(model->lastError().text()));
        }

        ui->pushButtonok->setEnabled(false);
        ui->pushButtonuserchg->setEnabled(false);
        ui->pushButtonuserdel->setEnabled(false);
    }//optsel==3
    ui->widget->hide();

    ui->pushButtonuseradd->setStyleSheet(button_style1);//
    ui->pushButtonuserchg->setStyleSheet(button_style1);
    ui->pushButtonuserdel->setStyleSheet(button_style1);//
    ui->pushButtonuseradd->setEnabled(true);

}

void Dialogequset::on_tableView_clicked(const QModelIndex &index)
{
    QString qx,qy,qz,qdh,qpx,qpy,qpz,qrx,qry,qrz,qrx1,qry1,qrz1,qrw1;
    QSqlRecord record=model->record(index.row());
    qid=record.value("id").toString();
    qno=record.value("no").toString();
    qtitle=record.value("title").toString();
    qdh=record.value("dz").toString();

    qpos=record.value("roomno").toInt();
    qtype=record.value("type").toInt();

    qx=record.value("x").toString();
    qy=record.value("y").toString();
    qz=record.value("z").toString();
    qpx=record.value("rx").toString();
    qpy=record.value("ry").toString();
    qpz=record.value("rz").toString();

    qrx=record.value("shx").toString();
    qry=record.value("shy").toString();
    qrz=record.value("shz").toString();
    qrx1=record.value("shx1").toString();
    qry1=record.value("shy1").toString();
    qrz1=record.value("shz1").toString();
    qrw1=record.value("shw1").toString();

    ui->widget->show();

    ui->lineEditidx->setText(qid);
    ui->lineEditno->setText(qno);
    ui->lineEdittitle->setText(qtitle);
    ui->lineEdithigh->setText(qdh);
    ui->comboBoxpos->setCurrentIndex(qpos);
    ui->comboBoxtype->setCurrentIndex(qtype);

    sqid=qid;
    sqtitle=qtitle;
    nlayout=qtype;
    slayout=ui->comboBoxtype->currentText();

    ui->lineEditx->setText(qx);
    ui->lineEdity->setText(qy);
    ui->lineEditz->setText(qz);
    ui->lineEditrx->setText(qpx);
    ui->lineEditry->setText(qpy);
    ui->lineEditrz->setText(qpz);

    ui->lineEditx1->setText(qrx);
    ui->lineEdity1->setText(qry);
    ui->lineEditz1->setText(qrz);
    ui->lineEditx2->setText(qrx1);
    ui->lineEdity2->setText(qry1);
    ui->lineEditz2->setText(qrz1);
    ui->lineEditw2->setText(qrw1);

    ui->lineEditidx->setEnabled(false);
    ui->lineEditno->setEnabled(false);
    ui->lineEdittitle->setEnabled(false);
    ui->lineEdithigh->setEnabled(false);
    ui->comboBoxpos->setEnabled(false);
    ui->comboBoxtype->setEnabled(false);

    ui->lineEditx->setEnabled(false);
    ui->lineEdity->setEnabled(false);
    ui->lineEditz->setEnabled(false);
    ui->lineEditrx->setEnabled(false);
    ui->lineEditry->setEnabled(false);
    ui->lineEditrz->setEnabled(false);

    ui->lineEditx1->setEnabled(false);
    ui->lineEdity1->setEnabled(false);
    ui->lineEditz1->setEnabled(false);
    ui->lineEditx2->setEnabled(false);
    ui->lineEdity2->setEnabled(false);
    ui->lineEditz2->setEnabled(false);
    ui->lineEditw2->setEnabled(false);

    ui->pushButtonuserchg->setEnabled(true);
    ui->pushButtonuserdel->setEnabled(true);

}

void Dialogequset::on_pushButtoncancel_clicked()
{
    ui->widget->hide();
    ui->pushButtonuserchg->setEnabled(false);
    ui->pushButtonuserdel->setEnabled(false);

    ui->pushButtonuseradd->setStyleSheet(button_style1);//
    ui->pushButtonuserchg->setStyleSheet(button_style1);
    ui->pushButtonuserdel->setStyleSheet(button_style1);//

    ui->pushButtonuseradd->setEnabled(true);
}

void Dialogequset::on_pushButton_4_clicked()
{

    dlgkk=new Dialogkkset(this);
    dlgkk->show();
}

void Dialogequset::on_pushButton_clicked()//record initpos
{
    //roscmd=271;
    QString topic="/robot_pose";
    QString type="geometry_msgs/Pose";
    //Webcmd *cmd=new Webcmd();
    webcmd->subcmd(topic,type);
    ui->frame->show();
    


   
 
/*************
 ui->lineEditx1->setText(qshx);
 ui->lineEdity1->setText(qshy);
 ui->lineEditz1->setText(qshz);
 ui->lineEditx2->setText(qshx1);
 ui->lineEdity2->setText(qshy1);
 ui->lineEditz2->setText(qshz1);
 ui->lineEditw2->setText(qshw1);
***************/

}

void Dialogequset::on_pushButton_2_clicked()
{
    //roscmd=270;
    QString topic="/robot_pose";
   // QString type="geometry_msgs/Pose";
    webcmd->unsubcmd(topic);
    ui->frame->hide();
    ui->lineEditx1->setText(ui->lineEditx1_2->text());
    ui->lineEdity1->setText(ui->lineEdity1_2->text());
    ui->lineEditz1->setText(ui->lineEditz1_2->text());
    ui->lineEditx2->setText(ui->lineEditx2_2->text());
    ui->lineEdity2->setText(ui->lineEdity2_2->text());
    ui->lineEditz2->setText(ui->lineEditz2_2->text());
    ui->lineEditw2->setText(ui->lineEditw2_2->text());

}
