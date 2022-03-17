#include "dialogkktype.h"
#include "ui_dialogkktype.h"
#include <QSqlRecord>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include <QSqlError>
#include "ComData.h"

Dialogkktype::Dialogkktype(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialogkktype)
{
    ui->setupUi(this);
    model=new QSqlTableModel(this);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
   // model->setQuery("select * from book");
    model->setTable("kktype");

    model->select();

    model->setHeaderData(0,Qt::Horizontal,tr("名称"));
    model->setHeaderData(1,Qt::Horizontal,tr("类型"));
    model->setHeaderData(2,Qt::Horizontal,tr("状态数"));

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
                                                         //(a)
    ui->tableView->setSortingEnabled(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
   // ui->tableView->setShowGrid(false);
    ui->tableView->setAlternatingRowColors(true);

    ui->tableView->setModel(model);
    ui->widget->hide();

    ui->pushButtonuserchg->setEnabled(false);
    ui->pushButtonuserdel->setEnabled(false);

    ui->pushButtonok->setEnabled(false);
    ui->pushButtoncancel->setEnabled(false);
    button_style1="QPushButton{background-color:white;color: black;border-radius: 10px;  border: 2px groove gray;border-style: outset;}";

    button_style2="QPushButton{background-color:rgb(235, 142, 85);border-radius: 10px;  border: 2px groove gray;border-style: inset; }";

}

Dialogkktype::~Dialogkktype()
{
    delete ui;
}

void Dialogkktype::on_pushButtonuseradd_clicked()
{
    ui->widget->show();
    ui->lineEditname->setText("");
    ui->lineEdittype->setText("");
    ui->lineEditstu->setText("");

    ui->pushButtonok->setEnabled(true);
    ui->pushButtoncancel->setEnabled(true);
    optsel=1; //add
    ui->pushButtonuseradd->setStyleSheet(button_style2);//
    ui->pushButtonuserchg->setStyleSheet(button_style1);
    ui->pushButtonuserdel->setStyleSheet(button_style1);//

    ui->lineEditname->setEnabled(true);
    ui->lineEdittype->setEnabled(true);
    ui->lineEditstu->setEnabled(true);

    ui->pushButtonuserchg->setEnabled(false);
    ui->pushButtonuserdel->setEnabled(false);
}

void Dialogkktype::on_pushButtonuserdel_clicked()
{
    ui->pushButtonok->setEnabled(true);
    ui->pushButtoncancel->setEnabled(true);
    optsel=3; //del
    ui->pushButtonuseradd->setStyleSheet(button_style1);//
    ui->pushButtonuserchg->setStyleSheet(button_style1);
    ui->pushButtonuserdel->setStyleSheet(button_style2);//

    ui->pushButtonuseradd->setEnabled(false);//
    ui->pushButtonuserchg->setEnabled(false);
}

void Dialogkktype::on_pushButtonuserchg_clicked()
{
    ui->pushButtonok->setEnabled(true);
    ui->pushButtoncancel->setEnabled(true);
    optsel=2; //chg
    ui->pushButtonuseradd->setStyleSheet(button_style1);//
    ui->pushButtonuserchg->setStyleSheet(button_style2);
    ui->pushButtonuserdel->setStyleSheet(button_style1);//

    ui->lineEditname->setEnabled(true);
    ui->lineEdittype->setEnabled(true);
    ui->lineEditstu->setEnabled(true);

    ui->pushButtonuserdel->setEnabled(false);
    ui->pushButtonuseradd->setEnabled(false);
}

void Dialogkktype::on_pushButtonok_clicked()
{
    if(optsel==1)//add
    {
        QSqlQuery query;

        qsname=ui->lineEditname->text();
        qitype=ui->lineEdittype->text().toInt();
        qistunum=ui->lineEditstu->text().toInt();

        if(qsname.isEmpty()||qitype<0||qistunum<=0)
        {
           QMessageBox::warning(this,tr("警告"),tr("检查类型，状态数，名称不能为空!"),QMessageBox::Yes);
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
            if(model->index(index, 0).data() == qsname)
            {
                QMessageBox::information(this,"提示","该名称已存在,退出！");
                return;
            }

      //   qDebug()<<Dialogadduser::strID;
         record.setValue(0, qsname);
         record.setValue(1, qitype);
         record.setValue(2, qistunum);
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
        qsname=ui->lineEditname->text();
        qitype=ui->lineEdittype->text().toUInt();
        qistunum=ui->lineEditstu->text().toInt();

        if(qsname.isEmpty()||qitype<0||qistunum<=0)
        {
           QMessageBox::warning(this,tr("警告"),tr("检查类型和状态数，名称不能为空!"),QMessageBox::Yes);
           ui->pushButtoncancel->setEnabled(false);
           ui->pushButtonok->setEnabled(false);
           ui->pushButtonuseradd->setStyleSheet(button_style1);//
           ui->pushButtonuserchg->setStyleSheet(button_style1);
           ui->pushButtonuserdel->setStyleSheet(button_style1);//
           ui->pushButtonuseradd->setEnabled(true);
           ui->widget->hide();
            return;
        }


        if(qsname!=qsnamebf)
        {
           QSqlRecord record = model->record();
            // 从1开始遍历，遇到相同的ID，退出
           // 防止唯一主键重复导致提交失败，因为提交失败tableview还是会更新修改后的（视图）#include <QComboBox>
          //  bool bFlag = false;
            for(int index = 0; index < model->rowCount(); index++)
               if(model->index(index, 0).data() == qsname)
               {
                   QMessageBox::information(this,"提示","名称已存在,请检查和修改！");
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

         QModelIndex index_Name = model->index(row, 0);//选中行第1列的内容
         QModelIndex index_Type = model->index(row, 1);//选中行第2列的内容
         QModelIndex index_Stunum = model->index(row, 2);//选中行第3列的内容

         model->setData(index_Name, QVariant::fromValue(qsname));
         model->setData(index_Type, QVariant::fromValue(qitype));
         model->setData(index_Stunum, QVariant::fromValue(qistunum));
         // /**********************
      // QSqlTableModel *pMode = dynamic_cast<QSqlTableModel *>(ui->tableView->model());
       model->database().transaction(); //开始事务操作
       if (model->submitAll()) // 提交所有被修改的数据到数据库中
       {
           model->database().commit(); //提交成功，事务将真正修改数据库数据
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

void Dialogkktype::on_tableView_clicked(const QModelIndex &index)
{
    QString qx,qy,qz,qpx,qpy,qpz,qrx,qry,qrz,qrx1,qry1,qrz1,qrw1;
    QSqlRecord record=model->record(index.row());
    qsnamebf=record.value("name").toString();
    qitype=record.value("type").toInt();
    qistunum=record.value("stunum").toInt();

    ui->widget->show();

    ui->lineEditname->setText(qsnamebf);
    ui->lineEdittype->setText(QString::number(qitype));
    ui->lineEditstu->setText(QString::number(qistunum));

    ui->lineEditname->setEnabled(false);
    ui->lineEdittype->setEnabled(false);
    ui->lineEditstu->setEnabled(false);

    ui->pushButtonuserchg->setEnabled(true);
    ui->pushButtonuserdel->setEnabled(true);
}

void Dialogkktype::on_pushButtoncancel_clicked()
{
    ui->widget->hide();
    ui->pushButtonuserchg->setEnabled(false);
    ui->pushButtonuserdel->setEnabled(false);

    ui->pushButtonuseradd->setStyleSheet(button_style1);//
    ui->pushButtonuserchg->setStyleSheet(button_style1);
    ui->pushButtonuserdel->setStyleSheet(button_style1);//

    ui->pushButtonuseradd->setEnabled(true);
}
