#ifndef DIALOGKKTYPE_H
#define DIALOGKKTYPE_H

#include <QDialog>
#include <QSqlTableModel>

namespace Ui {
class Dialogkktype;
}

class Dialogkktype : public QDialog
{
    Q_OBJECT

public:
    explicit Dialogkktype(QWidget *parent = nullptr);
    ~Dialogkktype();
    QString button_style1;
    QString button_style2;
    QSqlTableModel *model;

private slots:
    void on_pushButtonuseradd_clicked();

    void on_pushButtonuserdel_clicked();

    void on_pushButtonuserchg_clicked();

    void on_pushButtonok_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_pushButtoncancel_clicked();

private:
    Ui::Dialogkktype *ui;
    int optsel;
    QString qsname;
    QString qsnamebf;
    int qitype;
    int qistunum;



};

#endif // DIALOGKKTYPE_H
