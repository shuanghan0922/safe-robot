#include "dialogkkset.h"
#include "ui_dialogkkset.h"
#include "dialogequset.h"
#include "ComData.h"
#include <QSqlRecord>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include <QSqlError>

Dialogkkset::Dialogkkset(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialogkkset)
{
    ui->setupUi(this);
    ui->labelid->setText(sqid);
    ui->labelname->setText(sqtitle);
    ui->labeltype->setText(slayout);

    model=new QSqlTableModel(this);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
   // model->setQuery("select * from book");
    model->setTable("kkpara");
    model->setFilter("id='"+sqid+"'");

    model->select();

    model->setHeaderData(0,Qt::Horizontal,tr("柜ID"));
    model->setHeaderData(1,Qt::Horizontal,tr("编号"));
    model->setHeaderData(2,Qt::Horizontal,tr("名称"));   
    model->setHeaderData(3,Qt::Horizontal,tr("类型"));
    model->setHeaderData(4,Qt::Horizontal,tr("状态"));
    model->setHeaderData(5,Qt::Horizontal,tr("偏移量x"));
    model->setHeaderData(6,Qt::Horizontal,tr("偏移量y"));
    model->setHeaderData(7,Qt::Horizontal,tr("偏移量z"));
    model->setHeaderData(8,Qt::Horizontal,tr("坐标修正值x"));
    model->setHeaderData(9,Qt::Horizontal,tr("坐标修正值y"));
    model->setHeaderData(10,Qt::Horizontal,tr("坐标修正值z"));
    model->setHeaderData(11,Qt::Horizontal,tr("柜中位置"));
    model->setHeaderData(12,Qt::Horizontal,tr("截图像素w"));
    model->setHeaderData(13,Qt::Horizontal,tr("截图像素h"));
    model->setHeaderData(14,Qt::Horizontal,tr("截图坐标x"));
    model->setHeaderData(15,Qt::Horizontal,tr("截图坐标y"));
    model->setHeaderData(16,Qt::Horizontal,tr("颜色"));
    model->setHeaderData(17,Qt::Horizontal,tr("操作修正值x"));
    model->setHeaderData(18,Qt::Horizontal,tr("操作修正值y"));
    model->setHeaderData(19,Qt::Horizontal,tr("操作修正值z"));
    model->setHeaderData(20,Qt::Horizontal,tr("定位角度修正值"));
    model->setHeaderData(21,Qt::Horizontal,tr("操作角度修正值"));
    model->setHeaderData(22,Qt::Horizontal,tr("颜色阈值1"));
    model->setHeaderData(23,Qt::Horizontal,tr("颜色阈值2"));
    model->setHeaderData(24,Qt::Horizontal,tr("颜色阈值3"));
    model->setHeaderData(25,Qt::Horizontal,tr("颜色阈值4"));
    model->setHeaderData(26,Qt::Horizontal,tr("颜色阈值5"));
    model->setHeaderData(27,Qt::Horizontal,tr("颜色阈值6"));
    model->setHeaderData(28,Qt::Horizontal,tr("颜色阈值7"));
    model->setHeaderData(29,Qt::Horizontal,tr("颜色阈值8"));
    model->setHeaderData(30,Qt::Horizontal,tr("颜色阈值9"));
    model->setHeaderData(31,Qt::Horizontal,tr("颜色阈值10"));
    model->setHeaderData(32,Qt::Horizontal,tr("颜色阈值11"));
    model->setHeaderData(33,Qt::Horizontal,tr("颜色阈值12"));

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
                                                         //(a)
    ui->tableView->setSortingEnabled(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
   // ui->tableView->setShowGrid(false);
    ui->tableView->setAlternatingRowColors(true);

    ui->tableView->setModel(model);
    ui->widget->hide();
/***************
    ui->comboBoxtype->addItem("按钮");
    ui->comboBoxtype->addItem("空开1P");
    ui->comboBoxtype->addItem("空开2P");
    ui->comboBoxtype->addItem("空开3P");
    ui->comboBoxtype->addItem("旋扭2档不带自锁");
    ui->comboBoxtype->addItem("旋扭2档带自锁");
    ui->comboBoxtype->addItem("旋扭3档");
******************/
    QSqlQuery query;
    bool value=query.exec("select * from kktype order by type");
    if(!value) QMessageBox::information(this,"提示","开关类型表连接接失败！");
    int i=0;
    while(query.next())//指向第一条记录
    {
      ui->comboBoxtype->addItem(query.value(0).toString());
      kktype[i].name=query.value(0).toString();
      kktype[i].type=query.value(1).toInt();
      kktype[i].stunum=query.value(2).toInt();
      i++;
    }

    nkktype=i;

//颜色值，0-无色，1-黑色，2-红色，3-绿色，4-蓝色5-带灯红色，6-带灯绿色
    ui->comboBoxcolor->addItem("无色");
    ui->comboBoxcolor->addItem("黑色");
    ui->comboBoxcolor->addItem("红色");
    ui->comboBoxcolor->addItem("绿色");
    ui->comboBoxcolor->addItem("蓝色");
    ui->comboBoxcolor->addItem("带灯红色");
    ui->comboBoxcolor->addItem("带灯绿色");
//****************
    ui->pushButtonchg->setEnabled(false);
    ui->pushButtondel->setEnabled(false);

    ui->pushButtonok->setEnabled(false);
    ui->pushButtoncancel->setEnabled(false);
    button_style1="QPushButton{background-color:white;color: black;border-radius: 10px;  border: 2px groove gray;border-style: outset;}";

    button_style2="QPushButton{background-color:rgb(235, 142, 85);border-radius: 10px;  border: 2px groove gray;border-style: inset; }";

}

Dialogkkset::~Dialogkkset()
{
    delete ui;
}

void Dialogkkset::on_pushButtonadd_clicked()
{
    ui->widget->show();
   // ui->lineEditidx->setText("");
    ui->lineEditno->setText("");
    ui->lineEdittitle->setText("");
    ui->comboBoxtype->setCurrentText("");
    ui->comboBoxstate->setCurrentText("");
    ui->comboBoxcolor->setCurrentText("");
    ui->lineEditpos->setText("1");

    ui->pushButtonok->setEnabled(true);
    ui->pushButtoncancel->setEnabled(true);
    optsel=1; //add
    ui->pushButtonadd->setStyleSheet(button_style2);//
    ui->pushButtonchg->setStyleSheet(button_style1);
    ui->pushButtondel->setStyleSheet(button_style1);//

    //ui->lineEditidx->setEnabled(true);
    ui->lineEditno->setEnabled(true);
    ui->lineEdittitle->setEnabled(true);
    ui->comboBoxstate->setEnabled(true);
    ui->comboBoxtype->setEnabled(true);
    ui->comboBoxcolor->setEnabled(true);

    ui->lineEditx->setEnabled(true);
    ui->lineEdity->setEnabled(true);
    ui->lineEditz->setEnabled(true);

    ui->lineEditx_1->setEnabled(true);
    ui->lineEdity_1->setEnabled(true);
    ui->lineEditz_1->setEnabled(true);
    ui->lineEdita_1->setEnabled(true);
    ui->lineEditx_2->setEnabled(true);
    ui->lineEdity_2->setEnabled(true);
    ui->lineEditz_2->setEnabled(true);
    ui->lineEdita_2->setEnabled(true);
    ui->lineEditpos->setEnabled(true);

    ui->lineEditpixw->setEnabled(true);
    ui->lineEditpixh->setEnabled(true);
    ui->lineEditpixx->setEnabled(true);
    ui->lineEditpixy->setEnabled(true);

    ui->lineEditc1->setEnabled(true);
    ui->lineEditc2->setEnabled(true);
    ui->lineEditc3->setEnabled(true);
    ui->lineEditc4->setEnabled(true);
    ui->lineEditc5->setEnabled(true);
    ui->lineEditc6->setEnabled(true);
    ui->lineEditc7->setEnabled(true);
    ui->lineEditc8->setEnabled(true);
    ui->lineEditc9->setEnabled(true);
    ui->lineEditc10->setEnabled(true);
    ui->lineEditc11->setEnabled(true);
    ui->lineEditc12->setEnabled(true);

    ui->pushButtonchg->setEnabled(false);
    ui->pushButtondel->setEnabled(false);
}

void Dialogkkset::on_pushButtondel_clicked()
{
    ui->pushButtonok->setEnabled(true);
    ui->pushButtoncancel->setEnabled(true);
    optsel=3; //del
    ui->pushButtonadd->setStyleSheet(button_style1);//
    ui->pushButtonchg->setStyleSheet(button_style1);
    ui->pushButtondel->setStyleSheet(button_style2);//

    //ui->lineEditidx->setEnabled(false);
    ui->lineEditno->setEnabled(false);
    ui->lineEdittitle->setEnabled(false);
    ui->comboBoxstate->setEnabled(false);
    ui->comboBoxtype->setEnabled(false);
    ui->comboBoxcolor->setEnabled(false);

    ui->lineEditx->setEnabled(false);
    ui->lineEdity->setEnabled(false);
    ui->lineEditz->setEnabled(false);

    ui->lineEditx_1->setEnabled(false);
    ui->lineEdity_1->setEnabled(false);
    ui->lineEditz_1->setEnabled(false);
    ui->lineEdita_1->setEnabled(false);
    ui->lineEditx_2->setEnabled(false);
    ui->lineEdity_2->setEnabled(false);
    ui->lineEditz_2->setEnabled(false);
    ui->lineEdita_2->setEnabled(false);
    ui->lineEditpos->setEnabled(false);

    ui->lineEditpixw->setEnabled(false);
    ui->lineEditpixh->setEnabled(false);
    ui->lineEditpixx->setEnabled(false);
    ui->lineEditpixy->setEnabled(false);

    ui->lineEditc1->setEnabled(false);
    ui->lineEditc2->setEnabled(false);
    ui->lineEditc3->setEnabled(false);
    ui->lineEditc4->setEnabled(false);
    ui->lineEditc5->setEnabled(false);
    ui->lineEditc6->setEnabled(false);
    ui->lineEditc7->setEnabled(false);
    ui->lineEditc8->setEnabled(false);
    ui->lineEditc9->setEnabled(false);
    ui->lineEditc10->setEnabled(false);
    ui->lineEditc11->setEnabled(false);
    ui->lineEditc12->setEnabled(false);

    ui->pushButtonadd->setEnabled(false);//
    ui->pushButtonchg->setEnabled(false);
}

void Dialogkkset::on_pushButtonchg_clicked()
{
    ui->pushButtonok->setEnabled(true);
    ui->pushButtoncancel->setEnabled(true);
    optsel=2; //chg
    ui->pushButtonadd->setStyleSheet(button_style1);//
    ui->pushButtonchg->setStyleSheet(button_style2);
    ui->pushButtondel->setStyleSheet(button_style1);//

   // ui->lineEditidx->setEnabled(true);
    ui->lineEditno->setEnabled(true);
    ui->lineEdittitle->setEnabled(true);
    ui->comboBoxstate->setEnabled(true);
    ui->comboBoxtype->setEnabled(true);
    ui->comboBoxcolor->setEnabled(true);

    ui->lineEditx->setEnabled(true);
    ui->lineEdity->setEnabled(true);
    ui->lineEditz->setEnabled(true);

    ui->lineEditx_1->setEnabled(true);
    ui->lineEdity_1->setEnabled(true);
    ui->lineEditz_1->setEnabled(true);
    ui->lineEdita_1->setEnabled(true);
    ui->lineEditx_2->setEnabled(true);
    ui->lineEdity_2->setEnabled(true);
    ui->lineEditz_2->setEnabled(true);
    ui->lineEdita_2->setEnabled(true);
    ui->lineEditpos->setEnabled(true);

    ui->lineEditpixw->setEnabled(true);
    ui->lineEditpixh->setEnabled(true);
    ui->lineEditpixx->setEnabled(true);
    ui->lineEditpixy->setEnabled(true);

    ui->lineEditc1->setEnabled(true);
    ui->lineEditc2->setEnabled(true);
    ui->lineEditc3->setEnabled(true);
    ui->lineEditc4->setEnabled(true);
    ui->lineEditc5->setEnabled(true);
    ui->lineEditc6->setEnabled(true);
    ui->lineEditc7->setEnabled(true);
    ui->lineEditc8->setEnabled(true);
    ui->lineEditc9->setEnabled(true);
    ui->lineEditc10->setEnabled(true);
    ui->lineEditc11->setEnabled(true);
    ui->lineEditc12->setEnabled(true);

    ui->pushButtondel->setEnabled(false);
    ui->pushButtonadd->setEnabled(false);
}

void Dialogkkset::on_pushButtonok_clicked()
{
    if(optsel==1)//add
    {
        QSqlQuery query;

        //qindex=ui->lineEditidx->text();
        qno=ui->lineEditno->text();
        qtitle=ui->lineEdittitle->text();

        int sel=ui->comboBoxstate->currentIndex();
        int idx=ui->comboBoxtype->currentIndex();
        qtype=kktype[idx].type;

        if(qtype<20)qvalue=sel;
        else
        {
           if(qtype<22)
           {
              if(sel==0)qvalue=-1;
              else qvalue=1;
           }
           if(qtype==22)
           {
               if(sel==0)qvalue=-1;
               if(sel==1)qvalue=0;
               if(sel==2)qvalue=1;
           }
        }

/**************
        if(qvalue==2&&qtype!=6)
        {
            QMessageBox::warning(this,tr("警告"),tr("开关状态值错误，请检查!"),QMessageBox::Yes);
            return;
        }
***************/
        qjkx=ui->lineEditx->text().toDouble();
        qjky=ui->lineEdity->text().toDouble();
        qjkz=ui->lineEditz->text().toDouble();
        qjkpx=ui->lineEditx_1->text().toDouble();
        qjkpy=ui->lineEdity_1->text().toDouble();
        qjkpz=ui->lineEditz_1->text().toDouble();
        qjkpa=ui->lineEdita_1->text().toDouble();
        qjkpx1=ui->lineEditx_2->text().toDouble();
        qjkpy1=ui->lineEdity_2->text().toDouble();
        qjkpz1=ui->lineEditz_2->text().toDouble();
        qjkpa1=ui->lineEdita_2->text().toDouble();
        qpos=ui->lineEditpos->text().toInt();

        qpixw=ui->lineEditpixw->text().toInt();
        qpixh=ui->lineEditpixh->text().toInt();
        qpixx=ui->lineEditpixx->text().toInt();
        qpixy=ui->lineEditpixy->text().toInt();
        qcolor=ui->comboBoxcolor->currentIndex();
        qcolor1=ui->lineEditc1->text().toInt();
        qcolor2=ui->lineEditc2->text().toInt();
        qcolor3=ui->lineEditc3->text().toInt();
        qcolor4=ui->lineEditc4->text().toInt();
        qcolor5=ui->lineEditc5->text().toInt();
        qcolor6=ui->lineEditc6->text().toInt();
        qcolor7=ui->lineEditc7->text().toInt();
        qcolor8=ui->lineEditc8->text().toInt();
        qcolor9=ui->lineEditc9->text().toInt();
        qcolor10=ui->lineEditc10->text().toInt();
        qcolor11=ui->lineEditc11->text().toInt();
        qcolor12=ui->lineEditc12->text().toInt();

        if(qno.isEmpty()||qtitle.isEmpty())
        {
           QMessageBox::warning(this,tr("警告"),tr("编号和名称不能为空!"),QMessageBox::Yes);
           ui->pushButtonadd->setStyleSheet(button_style1);//
           ui->pushButtonchg->setStyleSheet(button_style1);
           ui->pushButtondel->setStyleSheet(button_style1);//
           ui->pushButtonadd->setEnabled(true);
           ui->widget->hide();
            return;
        }
        QSqlRecord record = model->record();
         // 从1开始遍历，遇到相同的ID，退出
        // 防止唯一主键重复导致提交失败，因为提交失败tableview还是会更新修改后的（视图）
       //  bool bFlag = false;
         for(int index = 0;index < model->rowCount(); index++)
            if(model->index(index, 1).data() == qno)
            {
                QMessageBox::information(this,"提示","该编号已存在,退出！");
                return;
            }

      //   qDebug()<<Dialogadduser::strID;
         record.setValue(0, sqid);
         record.setValue(1, qno);
         record.setValue(2, qtitle);
         record.setValue(3, qtype);
         record.setValue(4, qvalue);
         record.setValue(5, qjkx);
         record.setValue(6, qjky);
         record.setValue(7, qjkz);
         record.setValue(8, qjkpx);
         record.setValue(9, qjkpy);
         record.setValue(10, qjkpz);
         record.setValue(11, qpos);
         record.setValue(12, qpixw);
         record.setValue(13, qpixh);
         record.setValue(14, qpixx);
         record.setValue(15, qpixy);
         record.setValue(16, qcolor);
         record.setValue(17, qjkpx1);
         record.setValue(18, qjkpy1);
         record.setValue(19, qjkpz1);
         record.setValue(20, qjkpa);
         record.setValue(21, qjkpa1);
         record.setValue(22, qcolor1);
         record.setValue(23, qcolor2);
         record.setValue(24, qcolor3);
         record.setValue(25, qcolor4);
         record.setValue(26, qcolor5);
         record.setValue(27, qcolor6);
         record.setValue(28, qcolor7);
         record.setValue(29, qcolor8);
         record.setValue(30, qcolor9);
         record.setValue(31, qcolor10);
         record.setValue(32, qcolor11);
         record.setValue(33, qcolor12);

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
       // qindex=ui->lineEditidx->text();
        qno=ui->lineEditno->text();
        qtitle=ui->lineEdittitle->text();
        //qvalue=ui->comboBoxstate->currentIndex();
        //qtype=ui->comboBoxtype->currentIndex();

        int sel=ui->comboBoxstate->currentIndex();
        int idx=ui->comboBoxtype->currentIndex();
        qtype=kktype[idx].type;

        if(qtype<20)qvalue=sel;
        else
        {
           if(qtype<22)
           {
              if(sel==0)qvalue=-1;
              else qvalue=1;
           }
           if(qtype==22)
           {
               if(sel==0)qvalue=-1;
               if(sel==1)qvalue=0;
               if(sel==2)qvalue=1;
           }
        }

/************
        if(qvalue==2&&qtype!=6)
        {
            QMessageBox::warning(this,tr("警告"),tr("开关状态值错误，请检查!"),QMessageBox::Yes);
            return;
        }
************/

        qjkx=ui->lineEditx->text().toDouble();
        qjky=ui->lineEdity->text().toDouble();
        qjkz=ui->lineEditz->text().toDouble();
        qjkpx=ui->lineEditx_1->text().toDouble();
        qjkpy=ui->lineEdity_1->text().toDouble();
        qjkpz=ui->lineEditz_1->text().toDouble();
        qjkpa=ui->lineEdita_1->text().toDouble();
        qjkpx1=ui->lineEditx_2->text().toDouble();
        qjkpy1=ui->lineEdity_2->text().toDouble();
        qjkpz1=ui->lineEditz_2->text().toDouble();
        qjkpa1=ui->lineEdita_2->text().toDouble();
        qpos=ui->lineEditpos->text().toInt();

        qpixw=ui->lineEditpixw->text().toInt();
        qpixh=ui->lineEditpixh->text().toInt();
        qpixx=ui->lineEditpixx->text().toInt();
        qpixy=ui->lineEditpixy->text().toInt();
        qcolor=ui->comboBoxcolor->currentIndex();
        qcolor1=ui->lineEditc1->text().toInt();
        qcolor2=ui->lineEditc2->text().toInt();
        qcolor3=ui->lineEditc3->text().toInt();
        qcolor4=ui->lineEditc4->text().toInt();
        qcolor5=ui->lineEditc5->text().toInt();
        qcolor6=ui->lineEditc6->text().toInt();
        qcolor7=ui->lineEditc7->text().toInt();
        qcolor8=ui->lineEditc8->text().toInt();
        qcolor9=ui->lineEditc9->text().toInt();
        qcolor10=ui->lineEditc10->text().toInt();
        qcolor11=ui->lineEditc11->text().toInt();
        qcolor12=ui->lineEditc12->text().toInt();

        if(qno.isEmpty()||qtitle.isEmpty())
        {
           QMessageBox::warning(this,tr("警告"),tr("编号和名称不能为空!"),QMessageBox::Yes);
           ui->pushButtoncancel->setEnabled(false);
           ui->pushButtonok->setEnabled(false);
           ui->pushButtonadd->setStyleSheet(button_style1);//
           ui->pushButtonchg->setStyleSheet(button_style1);
           ui->pushButtondel->setStyleSheet(button_style1);//
           ui->pushButtonadd->setEnabled(true);
           ui->widget->hide();
            return;
        }


        if(qnosel!=qno)
        {
           QSqlRecord record = model->record();
            // 从1开始遍历，遇到相同的ID，退出
           // 防止唯一主键重复导致提交失败，因为提交失败tableview还是会更新修改后的（视图）#include <QComboBox>
          //  bool bFlag = false;
            for(int index = 0; index < model->rowCount(); index++)
               if(model->index(index, 1).data() == qno)
               {
                   QMessageBox::information(this,"提示","编号已存在,请检查和修改！");
                   ui->pushButtoncancel->setEnabled(false);
                   ui->pushButtonok->setEnabled(false);
                   ui->pushButtonadd->setStyleSheet(button_style1);//
                   ui->pushButtonchg->setStyleSheet(button_style1);
                   ui->pushButtondel->setStyleSheet(button_style1);//
                   ui->pushButtonadd->setEnabled(true);
                   ui->widget->hide();
                   return;
               }
         }

         int row = ui->tableView->currentIndex().row();
     //  ui->tableView->setSortingEnabled(true);

         QModelIndex index_Id = model->index(row, 0);//选中行第1列的内容
         QModelIndex index_No = model->index(row, 1);//选中行第2列的内容
         QModelIndex index_Title = model->index(row, 2);//选中行第3列的内容
         QModelIndex index_Type= model->index(row, 3);//选中行第4列的内容
         QModelIndex index_Value = model->index(row, 4);//选中行第5列的内
         QModelIndex index_Jkx = model->index(row, 5);//选中行第3列的内容
         QModelIndex index_Jky= model->index(row, 6);//选中行第4列的内容
         QModelIndex index_Jkz = model->index(row, 7);//选中行第5列的内
         QModelIndex index_Jkpx = model->index(row, 8);
         QModelIndex index_Jkpy = model->index(row, 9);
         QModelIndex index_Jkpz = model->index(row, 10);
         QModelIndex index_Pos = model->index(row, 11);
         QModelIndex index_Pixw = model->index(row, 12);
         QModelIndex index_Pixh = model->index(row, 13);
         QModelIndex index_Pixw1 = model->index(row, 14);
         QModelIndex index_Pixh1 = model->index(row, 15);
         QModelIndex index_Pcolor = model->index(row, 16);
         QModelIndex index_Jkpx1 = model->index(row, 17);
         QModelIndex index_Jkpy1 = model->index(row, 18);
         QModelIndex index_Jkpz1 = model->index(row, 19);
         QModelIndex index_Jkpa = model->index(row, 20);
         QModelIndex index_Jkpa1 = model->index(row, 21);
         QModelIndex index_Pcolor1 = model->index(row, 22);
         QModelIndex index_Pcolor2 = model->index(row, 23);
         QModelIndex index_Pcolor3 = model->index(row, 24);
         QModelIndex index_Pcolor4 = model->index(row, 25);
         QModelIndex index_Pcolor5 = model->index(row, 26);
         QModelIndex index_Pcolor6 = model->index(row, 27);
         QModelIndex index_Pcolor7 = model->index(row, 28);
         QModelIndex index_Pcolor8 = model->index(row, 29);
         QModelIndex index_Pcolor9 = model->index(row, 30);
         QModelIndex index_Pcolor10 = model->index(row, 31);
         QModelIndex index_Pcolor11 = model->index(row, 32);
         QModelIndex index_Pcolor12 = model->index(row, 33);

         model->setData(index_Id, QVariant::fromValue(sqid));
         model->setData(index_No, QVariant::fromValue(qno));
         model->setData(index_Title, QVariant::fromValue(qtitle));
         model->setData(index_Type, QVariant::fromValue(qtype));
         model->setData(index_Value, QVariant::fromValue(qvalue));
         model->setData(index_Jkx, QVariant::fromValue(qjkx));
         model->setData(index_Jky, QVariant::fromValue(qjky));
         model->setData(index_Jkz, QVariant::fromValue(qjkz));
         model->setData(index_Jkpx, QVariant::fromValue(qjkpx));
         model->setData(index_Jkpy, QVariant::fromValue(qjkpy));
         model->setData(index_Jkpz, QVariant::fromValue(qjkpz));
         model->setData(index_Pos, QVariant::fromValue(qpos));
         model->setData(index_Pixw, QVariant::fromValue(qpixw));
         model->setData(index_Pixh, QVariant::fromValue(qpixh));
         model->setData(index_Pixw1, QVariant::fromValue(qpixx));
         model->setData(index_Pixh1, QVariant::fromValue(qpixy));
         model->setData(index_Pcolor, QVariant::fromValue(qcolor));
         model->setData(index_Jkpx1, QVariant::fromValue(qjkpx1));
         model->setData(index_Jkpy1, QVariant::fromValue(qjkpy1));
         model->setData(index_Jkpz1, QVariant::fromValue(qjkpz1));
         model->setData(index_Jkpa, QVariant::fromValue(qjkpa));
         model->setData(index_Jkpa1, QVariant::fromValue(qjkpa1));
         model->setData(index_Pcolor1, QVariant::fromValue(qcolor1));
         model->setData(index_Pcolor2, QVariant::fromValue(qcolor2));
         model->setData(index_Pcolor3, QVariant::fromValue(qcolor3));
         model->setData(index_Pcolor4, QVariant::fromValue(qcolor4));
         model->setData(index_Pcolor5, QVariant::fromValue(qcolor5));
         model->setData(index_Pcolor6, QVariant::fromValue(qcolor6));
         model->setData(index_Pcolor7, QVariant::fromValue(qcolor7));
         model->setData(index_Pcolor8, QVariant::fromValue(qcolor8));
         model->setData(index_Pcolor9, QVariant::fromValue(qcolor9));
         model->setData(index_Pcolor10, QVariant::fromValue(qcolor10));
         model->setData(index_Pcolor11, QVariant::fromValue(qcolor11));
         model->setData(index_Pcolor12, QVariant::fromValue(qcolor12));

      // /**********************
      // QSqlTableModel *pMode = dynamic_cast<QSqlTableModel *>(ui->tableView->model());
       model->database().transaction(); //开始事务操作
       if (model->submitAll()) // 提交所有被修改的数据到数据库中
       {
           int idx=kkno2index(qno);
           model->database().commit(); //提交成功，事务将真正修改数据库数据

           kkparaset[idx].id=sqid;
           kkparaset[idx].no=qno;
           kkparaset[idx].name=qtitle;
           kkparaset[idx].type=qtype;
           kkparaset[idx].value=qvalue;
           kkparaset[idx].dx=qjkx;//相对于标定点的偏移量dy,dz,dx, 单位mm
           kkparaset[idx].dy=qjky;
           kkparaset[idx].dz=qjkz;

           kkparaset[idx].locate_dx=qjkpx;//开关定位坐标修正值，单位mm,du
           kkparaset[idx].locate_dy=qjkpy;
           kkparaset[idx].locate_dz=qjkpz;
           kkparaset[idx].locate_angle6=qjkpa;
           kkparaset[idx].opt_x=qjkpx1;//开关操作坐标修正值dx，dy，dz（单位mm），以及角度修正值（单位度）
           kkparaset[idx].opt_y=qjkpy1;
           kkparaset[idx].opt_z=qjkpz1;
           kkparaset[idx].opt_angle6=qjkpa1;

           kkparaset[idx].pos=qpos;
           kkparaset[idx].pix_w=qpixw;  //宽度像素值 拍照位置
           kkparaset[idx].pix_h=qpixh;  //高度像素值
           kkparaset[idx].pix_x=qpixx;  //宽度像素值 操作位置
           kkparaset[idx].pix_y=qpixy;  //高度像素值
           kkparaset[idx].color=qcolor;  //颜色值，0-无色，1-黑色，2-红色，3-绿色，4-蓝色5-带灯红色，6-带灯绿色
           kkparaset[idx].color_value1=qcolor1;
           kkparaset[idx].color_value2=qcolor2;
           kkparaset[idx].color_value3=qcolor3;
           kkparaset[idx].color_value4=qcolor5;
           kkparaset[idx].color_value5=qcolor5;
           kkparaset[idx].color_value6=qcolor6;
           kkparaset[idx].color_value7=qcolor7;
           kkparaset[idx].color_value8=qcolor8;
           kkparaset[idx].color_value9=qcolor9;
           kkparaset[idx].color_value10=qcolor10;
           kkparaset[idx].color_value11=qcolor11;
           kkparaset[idx].color_value12=qcolor12;
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
       ui->pushButtonchg->setEnabled(false);
       ui->pushButtondel->setEnabled(false);

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
        ui->pushButtonchg->setEnabled(false);
        ui->pushButtondel->setEnabled(false);
    }//optsel==3
    ui->widget->hide();

    ui->pushButtonadd->setStyleSheet(button_style1);//
    ui->pushButtonchg->setStyleSheet(button_style1);
    ui->pushButtondel->setStyleSheet(button_style1);//
    ui->pushButtonadd->setEnabled(true);
}

void Dialogkkset::on_tableView_clicked(const QModelIndex &index)
{
    QString qx,qy,qz,qpx,qpy,qpz,qpa,qpxw,qpxh,qpxx,qpxy,qp;
    QString qpx1,qpy1,qpz1,qpa1;
    QString qc1,qc2,qc3,qc4,qc5,qc6;
    QString qc7,qc8,qc9,qc10,qc11,qc12;
    QSqlRecord record=model->record(index.row());
    int sel,qcr;
    //qid=record.value("id").toString();
    qnosel=record.value("no").toString();
    qtitle=record.value("name").toString();
    qtype=record.value("type").toInt();
    qvalue=record.value("value").toInt();

    qx=record.value("x").toString();
    qy=record.value("y").toString();
    qz=record.value("z").toString();
    qpx=record.value("rx").toString();
    qpy=record.value("ry").toString();
    qpz=record.value("rz").toString();
    qp=record.value("pos").toString();

    qpxw=record.value("pixw").toString();
    qpxh=record.value("pixh").toString();
    qpxx=record.value("pixx").toString();
    qpxy=record.value("pixy").toString();
    qcr=record.value("color").toInt();

    qpx1=record.value("optx").toString();
    qpy1=record.value("opty").toString();
    qpz1=record.value("optz").toString();

    qpa=record.value("angle6").toString();
    qpa1=record.value("optangle6").toString();

    qc1=record.value("color1").toString();
    qc2=record.value("color2").toString();
    qc3=record.value("color3").toString();
    qc4=record.value("color4").toString();
    qc5=record.value("color5").toString();
    qc6=record.value("color6").toString();
    qc7=record.value("color7").toString();
    qc8=record.value("color8").toString();
    qc9=record.value("color9").toString();
    qc10=record.value("color10").toString();
    qc11=record.value("color11").toString();
    qc12=record.value("color12").toString();

    ui->widget->show();

    //ui->lineEditidx->setText(qid);
    ui->lineEditno->setText(qnosel);
    ui->lineEdittitle->setText(qtitle);

    int idx=kktype2index(qtype);
    ui->comboBoxtype->setCurrentIndex(idx);

    ui->comboBoxcolor->setCurrentIndex(qcr);
    if(qtype<20)
    {
       //for(int i=0;i<=ui->comboBoxstate->count();i++)
       //    ui->comboBoxstate->removeItem(i);
       ui->comboBoxstate->clear();
       ui->comboBoxstate->addItem("分闸");
       ui->comboBoxstate->addItem("合闸");
       ui->comboBoxstate->setCurrentIndex(qvalue);
    }
    else
    {
       if(qtype<22)
       {
          ui->comboBoxstate->clear();
          ui->comboBoxstate->addItem("左");
          ui->comboBoxstate->addItem("右");
          if(qvalue==-1)sel=0;
          else sel=1;

          ui->comboBoxstate->setCurrentIndex(sel);
       }
       if(qtype==22)
       {
           ui->comboBoxstate->clear();
           ui->comboBoxstate->addItem("左");
           ui->comboBoxstate->addItem("中");
           ui->comboBoxstate->addItem("右");
           if(qvalue==-1)sel=0;
           if(qvalue==0)sel=1;
           if(qvalue==1)sel=2;
           ui->comboBoxstate->setCurrentIndex(sel);
       }
    }

    ui->lineEditx->setText(qx);
    ui->lineEdity->setText(qy);
    ui->lineEditz->setText(qz);
    ui->lineEditx_1->setText(qpx);
    ui->lineEdity_1->setText(qpy);
    ui->lineEditz_1->setText(qpz);
    ui->lineEdita_1->setText(qpa);
    ui->lineEditx_2->setText(qpx1);
    ui->lineEdity_2->setText(qpy1);
    ui->lineEditz_2->setText(qpz1);
    ui->lineEdita_2->setText(qpa1);
    ui->lineEditpos->setText(qp);

    ui->lineEditpixw->setText(qpxw);
    ui->lineEditpixh->setText(qpxh);
    ui->lineEditpixx->setText(qpxx);
    ui->lineEditpixy->setText(qpxy);
    ui->lineEditc1->setText(qc1);
    ui->lineEditc2->setText(qc2);
    ui->lineEditc3->setText(qc3);
    ui->lineEditc4->setText(qc4);
    ui->lineEditc5->setText(qc5);
    ui->lineEditc6->setText(qc6);
    ui->lineEditc7->setText(qc7);
    ui->lineEditc8->setText(qc8);
    ui->lineEditc9->setText(qc9);
    ui->lineEditc10->setText(qc10);
    ui->lineEditc11->setText(qc11);
    ui->lineEditc12->setText(qc12);

    //ui->lineEditidx->setEnabled(false);
    ui->lineEditno->setEnabled(false);
    ui->lineEdittitle->setEnabled(false);
    ui->comboBoxstate->setEnabled(false);
    ui->comboBoxtype->setEnabled(false);
    ui->comboBoxcolor->setEnabled(false);

    ui->lineEditx->setEnabled(false);
    ui->lineEdity->setEnabled(false);
    ui->lineEditz->setEnabled(false);
    ui->lineEditx_1->setEnabled(false);
    ui->lineEdity_1->setEnabled(false);
    ui->lineEditz_1->setEnabled(false);
    ui->lineEdita_1->setEnabled(false);
    ui->lineEditx_2->setEnabled(false);
    ui->lineEdity_2->setEnabled(false);
    ui->lineEditz_2->setEnabled(false);
    ui->lineEdita_2->setEnabled(false);
    ui->lineEditpos->setEnabled(false);

    ui->lineEditc1->setEnabled(false);
    ui->lineEditc2->setEnabled(false);
    ui->lineEditc3->setEnabled(false);
    ui->lineEditc4->setEnabled(false);
    ui->lineEditc5->setEnabled(false);
    ui->lineEditc6->setEnabled(false);
    ui->lineEditc7->setEnabled(false);
    ui->lineEditc8->setEnabled(false);
    ui->lineEditc9->setEnabled(false);
    ui->lineEditc10->setEnabled(false);
    ui->lineEditc11->setEnabled(false);
    ui->lineEditc12->setEnabled(false);

    ui->lineEditpixw->setEnabled(false);
    ui->lineEditpixh->setEnabled(false);
    ui->lineEditpixx->setEnabled(false);
    ui->lineEditpixy->setEnabled(false);

    ui->pushButtonchg->setEnabled(true);
    ui->pushButtondel->setEnabled(true);
}

void Dialogkkset::on_pushButtoncancel_clicked()
{
    ui->widget->hide();
    ui->pushButtonchg->setEnabled(false);
    ui->pushButtondel->setEnabled(false);

    ui->pushButtonadd->setStyleSheet(button_style1);//
    ui->pushButtonchg->setStyleSheet(button_style1);
    ui->pushButtondel->setStyleSheet(button_style1);//

    ui->pushButtonadd->setEnabled(true);
}

void Dialogkkset::on_pushButton_clicked()
{
    this->close();
}

void Dialogkkset::on_comboBoxtype_currentIndexChanged(int index)
{
    int ntype=kktype[index].type;
    if(ntype<20)
    {
       //for(int i=0;i<=ui->comboBoxstate->count();i++)
       //    ui->comboBoxstate->removeItem(i);
       ui->comboBoxstate->clear();
       ui->comboBoxstate->addItem("分闸");
       ui->comboBoxstate->addItem("合闸");
       //ui->comboBoxstate->setCurrentIndex(qvalue);
    }
    else
    {
       if(ntype<22)
       {
          ui->comboBoxstate->clear();
          ui->comboBoxstate->addItem("左");
          ui->comboBoxstate->addItem("右");
          //if(qvalue==-1)sel=0;
          //else sel=1;

          //ui->comboBoxstate->setCurrentIndex(sel);
       }
       if(ntype==22)
       {
           ui->comboBoxstate->clear();
           ui->comboBoxstate->addItem("左");
           ui->comboBoxstate->addItem("中");
           ui->comboBoxstate->addItem("右");
          // if(qvalue==-1)sel=0;
          // if(qvalue==0)sel=1;
          // if(qvalue==1)sel=2;
          // ui->comboBoxstate->setCurrentIndex(sel);
       }
    }
}
