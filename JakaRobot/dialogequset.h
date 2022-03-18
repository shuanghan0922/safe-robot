#ifndef DIALOGEQUSET_H
#define DIALOGEQUSET_H
#include <QSqlTableModel>
#include <QDialog>
#include "dialogkkset.h"

namespace Ui {
class Dialogequset;
}

class Dialogequset : public QDialog
{
    Q_OBJECT

public:
    explicit Dialogequset(QWidget *parent = nullptr);
    ~Dialogequset();
    QString button_style1;
    QString button_style2;
    QSqlTableModel *model;

public slots:
    void disprospos();

private slots:
    void on_pushButtonuseradd_clicked();

    void on_comboBoxkkgrp_currentIndexChanged(int index);

    void on_pushButtonuserdel_clicked();

    void on_pushButtonuserchg_clicked();

    void on_pushButtonok_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_pushButtoncancel_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_clicked();



    void on_pushButton_2_clicked();

private:
    Ui::Dialogequset *ui;
    Dialogkkset *dlgkk;
    int optsel;
    QString qid;
    QString qindex;//="";
    QString qno;//="";
    QString qtitle;//="";
    int qpos;//="";
    int qtype;//="";
    int qbres;//="";
    int qdiss;//="";
    int qears;//=""
    int qkkgrp;//="";
    QString qzk3;//="";
    QString qzk1;//=""
    QString qzk2;//="";
    QString qzkname3;//="";
    QString qzkname1;//=""
    QString qzkname2;
    //QString qfb61;//="";
    //QString qfb62;//="";
    double qjkx;//=""
    double qjky;//="";
    double qjkz;//="";
    double qjkpx;//="";
    double qjkpy;//=""
    double qjkpz;//="";
    double qrosx;//="";
    double qrosy;//="";
    double qrosz;
    double qrosx1;//="";
    double qrosy1;//="";
    double qrosz1;
    double qrosw1;
    double qdz;//标定点高度增量
};

#endif // DIALOGEQUSET_H
