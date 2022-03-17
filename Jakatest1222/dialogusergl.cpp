#include "dialogusergl.h"
#include "ui_dialogusergl.h"
//#include "dialogadduser.h"
//#include "dialoguserchg.h"
#include <QSqlRecord>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include <QSqlError>
#include "ComData.h"

Dialogusergl::Dialogusergl(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialogusergl)
{
    ui->setupUi(this);

    ui->pushButton_4->hide();

    model=new QSqlTableModel(this);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
   // model->setQuery("select * from book");
    model->setTable("user");
    QString id;
    if(logright==0)id="0";
    if(logright==1)id="1";
    if(logright==2)id="2";
    if(logright==3)id="3";
    model->setFilter("type<='"+id+"'");
    model->select();

    model->setHeaderData(0,Qt::Horizontal,tr("用户名"));
    model->setHeaderData(1,Qt::Horizontal,tr("姓名"));
    model->setHeaderData(2,Qt::Horizontal,tr("密码"));
    model->setHeaderData(3,Qt::Horizontal,tr("类型"));
    model->setHeaderData(4,Qt::Horizontal,tr("权限"));
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
                                                         //(a)
    ui->tableView->setSortingEnabled(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
   // ui->tableView->setShowGrid(false);
    ui->tableView->setAlternatingRowColors(true);

    ui->tableView->setModel(model);
    ui->widget->hide();

    if(logright==3)  //厂家
    {
        ui->comboBoxtype->addItem("操作员");
        ui->comboBoxtype->addItem("工程师");
        ui->comboBoxtype->addItem("管理员");
        ui->comboBoxtype->addItem("厂家");
    }

    if(logright==2)
    {
        ui->comboBoxtype->addItem("操作员");
        ui->comboBoxtype->addItem("工程师");
        ui->comboBoxtype->addItem("管理员");
       // ui->comboBoxtype->addItem("厂家");
    }

    if(logright==1)
    {
        ui->comboBoxtype->addItem("操作员");
        ui->comboBoxtype->addItem("工程师");
        //ui->comboBoxtype->addItem("管理员");
        //ui->comboBoxtype->addItem("厂家");
    }


    ui->pushButtonuserchg->setEnabled(false);
    ui->pushButtonuserdel->setEnabled(false);

    ui->pushButtonok->setEnabled(false);
    ui->pushButtoncancel->setEnabled(false);
    button_style1="QPushButton{background-color:white;color: black;border-radius: 10px;  border: 2px groove gray;border-style: outset;}";

    button_style2="QPushButton{background-color:rgb(235, 142, 85);border-radius: 10px;  border: 2px groove gray;border-style: inset; }";

}



Dialogusergl::~Dialogusergl()
{
    delete ui;
}

void Dialogusergl::on_tableView_clicked(const QModelIndex &index)//选择用户
{
    QSqlRecord record=model->record(index.row());
    qname=record.value("name").toString();
    qID=record.value("ID").toString();
    qpw=record.value("pw").toString();
    qctype=record.value("ctype").toString();
    qntype=record.value("type").toInt();

    ui->widget->show();


    ui->comboBoxtype->setCurrentIndex(qntype);
    ui->lineEdit_user->setText(qID);
    ui->lineEdit_pw->setText(qpw);
    ui->lineEdit_name->setText(qname);

    ui->comboBoxtype->setEnabled(false);
    ui->lineEdit_user->setEnabled(false);
    ui->lineEdit_pw->setEnabled(false);
    ui->lineEdit_name->setEnabled(false);

    ui->pushButtonuserchg->setEnabled(true);
    ui->pushButtonuserdel->setEnabled(true);


}

void Dialogusergl::on_pushButtonuseradd_clicked()//添加
{
    ui->widget->show();
    ui->lineEdit_user->setText("");
    ui->lineEdit_name->setText("");
    ui->lineEdit_pw->setText("");
    ui->comboBoxtype->setCurrentText("");
    ui->pushButtonok->setEnabled(true);
    ui->pushButtoncancel->setEnabled(true);
    optsel=1; //add
    ui->pushButtonuseradd->setStyleSheet(button_style2);//
    ui->pushButtonuserchg->setStyleSheet(button_style1);
    ui->pushButtonuserdel->setStyleSheet(button_style1);//

    ui->comboBoxtype->setEnabled(true);
    ui->lineEdit_user->setEnabled(true);
    ui->lineEdit_pw->setEnabled(true);
    ui->lineEdit_name->setEnabled(true);

    ui->pushButtonuserchg->setEnabled(false);
    ui->pushButtonuserdel->setEnabled(false);

}

void Dialogusergl::on_pushButtonuserdel_clicked()//删除
{
    ui->pushButtonok->setEnabled(true);
    ui->pushButtoncancel->setEnabled(true);
    optsel=3; //del
    ui->pushButtonuseradd->setStyleSheet(button_style1);//
    ui->pushButtonuserchg->setStyleSheet(button_style1);
    ui->pushButtonuserdel->setStyleSheet(button_style2);//

    ui->comboBoxtype->setEnabled(false);
    ui->lineEdit_user->setEnabled(false);
    ui->lineEdit_pw->setEnabled(false);
    ui->lineEdit_name->setEnabled(false);

    ui->pushButtonuseradd->setEnabled(false);//
    ui->pushButtonuserchg->setEnabled(false);


}

void Dialogusergl::on_pushButtonuserchg_clicked()//修改
{
    ui->pushButtonok->setEnabled(true);
    ui->pushButtoncancel->setEnabled(true);
    optsel=2; //chg
    ui->pushButtonuseradd->setStyleSheet(button_style1);//
    ui->pushButtonuserchg->setStyleSheet(button_style2);
    ui->pushButtonuserdel->setStyleSheet(button_style1);//

    ui->comboBoxtype->setEnabled(true);
    ui->lineEdit_user->setEnabled(true);
    ui->lineEdit_pw->setEnabled(true);
    ui->lineEdit_name->setEnabled(true);

    ui->pushButtonuserdel->setEnabled(false);
    ui->pushButtonuseradd->setEnabled(false);
}

void Dialogusergl::on_pushButtonok_clicked()//确定
{
   if(optsel==1)//add
   {
       QSqlQuery query;
       qID=ui->lineEdit_user->text();
       qname=ui->lineEdit_name->text();
       qpw=ui->lineEdit_pw->text();
       qctype=ui->comboBoxtype->currentText();
       qntype=ui->comboBoxtype->currentIndex();
       if(qname.isEmpty()||qpw.isEmpty()||qID.isEmpty())
       {
          QMessageBox::warning(this,tr("警告"),tr("用户名,姓名和密码不能为空!"),QMessageBox::Yes);
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
           if(model->index(index, 0).data() == qID)
           {
               QMessageBox::information(this,"提示","用户名已存在,退出！");
               return;
           }

     //   qDebug()<<Dialogadduser::strID;
        record.setValue(0, qID);
        record.setValue(1, qname);
        record.setValue(2, qpw);
        record.setValue(3, qctype);
        record.setValue(4, qntype);
       model->insertRecord(model->rowCount(), record);
        // 每次手动提交，都会重新刷新tableView，保持mode和tableView一致
      //  model->submitAll();
        if (model->submitAll()) // 提交所有被修改的数据到数据库中
        {
          QMessageBox::information(this,"提示","增加用户成功！");
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
          QMessageBox::information(this,"提示","增加用user户失败！");
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
       qID1=ui->lineEdit_user->text();
       qname=ui->lineEdit_name->text();
       qpw=ui->lineEdit_pw->text();
       qctype=ui->comboBoxtype->currentText();
       qntype=ui->comboBoxtype->currentIndex();
       if(qname.isEmpty()||qpw.isEmpty()||qID.isEmpty())
       {
          QMessageBox::warning(this,tr("警告"),tr("用户名,姓名和密码不能为空!"),QMessageBox::Yes);
          ui->pushButtoncancel->setEnabled(false);
          ui->pushButtonok->setEnabled(false);
          ui->pushButtonuseradd->setStyleSheet(button_style1);//
          ui->pushButtonuserchg->setStyleSheet(button_style1);
          ui->pushButtonuserdel->setStyleSheet(button_style1);//
          ui->pushButtonuseradd->setEnabled(true);
          ui->widget->hide();
           return;
       }


       if(qID1!=qID)
       {
          QSqlRecord record = model->record();
           // 从1开始遍历，遇到相同的ID，退出
          // 防止唯一主键重复导致提交失败，因为提交失败tableview还是会更新修改后的（视图）#include <QComboBox>
         //  bool bFlag = false;
           for(int index = 0; index < model->rowCount(); index++)
              if(model->index(index, 0).data() == qID1)
              {
                  QMessageBox::information(this,"提示","用户已存在,请检查和修改！");
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
          QModelIndex index_Name = model->index(row, 1);//选中行第2列的内容
          QModelIndex index_Pwd = model->index(row, 2);//选中行第3列的内容
          QModelIndex index_Ctype = model->index(row, 3);//选中行第4列的内容
          QModelIndex index_Ntype = model->index(row, 4);//选中行第5列的内
          model->setData(index_Id, QVariant::fromValue(qID1));
          model->setData(index_Name, QVariant::fromValue(qname));
          model->setData(index_Pwd, QVariant::fromValue(qpw));
          model->setData(index_Ctype, QVariant::fromValue(qctype));
          model->setData(index_Ntype, QVariant::fromValue(qntype));



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

void Dialogusergl::on_pushButtoncancel_clicked()//取消
{
    ui->widget->hide();
    ui->pushButtonuserchg->setEnabled(false);
    ui->pushButtonuserdel->setEnabled(false);

    ui->pushButtonuseradd->setStyleSheet(button_style1);//
    ui->pushButtonuserchg->setStyleSheet(button_style1);
    ui->pushButtonuserdel->setStyleSheet(button_style1);//

    ui->pushButtonuseradd->setEnabled(true);

}
