#ifndef DIALOGUSERGL_H
#define DIALOGUSERGL_H
#include <QSqlTableModel>
#include <QDialog>

namespace Ui {
class Dialogusergl;
}

class Dialogusergl : public QDialog
{
    Q_OBJECT
     QSqlTableModel *model;

public:
    explicit Dialogusergl(QWidget *parent = 0);
    ~Dialogusergl();
     QString button_style1;
     QString button_style2;
private slots:
    // void on_pushButton_clicked();

   //  void on_pushButton_3_clicked();

   //  void on_pushButton_2_clicked();

     void on_tableView_clicked(const QModelIndex &index);

     void on_pushButtonuseradd_clicked();

     void on_pushButtonuserdel_clicked();

     void on_pushButtonuserchg_clicked();

     void on_pushButtonok_clicked();

     void on_pushButtoncancel_clicked();

private:
    Ui::Dialogusergl *ui;
    int optsel;
    QString qname;//="";
    QString qpw;//="";
    QString qctype;//="";
    QString qID;//="";
    int qntype;//=0;
};

#endif // DIALOGUSERGL_H
