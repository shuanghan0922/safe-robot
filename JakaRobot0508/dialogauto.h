#ifndef DIALOGAUTO_H
#define DIALOGAUTO_H

#include <QDialog>
#include "mygauge2.h"
#include <QVBoxLayout>

namespace Ui {
class Dialogauto;
}

class Dialogauto : public QDialog
{
    Q_OBJECT

public:
    explicit Dialogauto(QWidget *parent = nullptr);
    ~Dialogauto();
    void paintEvent(QPaintEvent *);
    QTimer *timerstu;
    void kkopt(QString no,int st);//开关编号，到达状态
signals:
    void Sendquid(const QString id);//send to mainwindow
private slots:
    void dispstu();
    void on_pushButton_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_pushButton0_clicked();

    void on_pushButton2_clicked();

    void on_pushButton1_clicked();

private:
    Ui::Dialogauto *ui;
    myGauge2 *sels;
    myGauge2 *ears;
    myGauge2 *diss;
    myGauge2 *bres;

    QVBoxLayout *mainLayout1;
    QVBoxLayout *mainLayout2;
    QVBoxLayout *mainLayout3;
    QVBoxLayout *mainLayout4;
};

#endif // DIALOGAUTO_H
