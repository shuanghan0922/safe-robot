#ifndef DIALOGKKSET_H
#define DIALOGKKSET_H
#include <QSqlTableModel>
#include <QDialog>

namespace Ui {
class Dialogkkset;
}

class Dialogkkset : public QDialog
{
    Q_OBJECT

public:
    explicit Dialogkkset(QWidget *parent = nullptr);
    ~Dialogkkset();
    QString button_style1;
    QString button_style2;
    QSqlTableModel *model;

private slots:
    void on_pushButtonadd_clicked();

    void on_pushButtondel_clicked();

    void on_pushButtonchg_clicked();

    void on_pushButtonok_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_pushButtoncancel_clicked();

    void on_pushButton_clicked();

    void on_comboBoxtype_currentIndexChanged(int index);

private:
    Ui::Dialogkkset *ui;
    int optsel;
    QString qnosel;
    //QString qindex;//="";
    QString qno;//="";
    QString qtitle;//="";
    int qvalue;//="";
    int qtype;//="";
    int qpos;
    //QString qfb61;//="";
    //QString qfb62;//="";
    double qjkx;//=""
    double qjky;//="";
    double qjkz;//="";
    double qjkpx;//="";
    double qjkpy;//=""
    double qjkpz;//="";
    double qjkpa;
    double qjkpx1;//="";
    double qjkpy1;//=""
    double qjkpz1;//="";
    double qjkpa1;
    int qpixw;
    int qpixh;
    int qpixx;
    int qpixy;
    int qcolor;
    int qcolor1;
    int qcolor2;
    int qcolor3;
    int qcolor4;
    int qcolor5;
    int qcolor6;
    int qcolor7;
    int qcolor8;
    int qcolor9;
    int qcolor10;
    int qcolor11;
    int qcolor12;

};

#endif // DIALOGKKSET_H
