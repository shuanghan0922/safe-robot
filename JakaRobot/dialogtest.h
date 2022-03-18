#ifndef DIALOGTEST_H
#define DIALOGTEST_H

#include <QDialog>

namespace Ui {
class Dialogtest;
}

class Dialogtest : public QDialog
{
    Q_OBJECT

public:
    explicit Dialogtest(QWidget *parent = nullptr);
    ~Dialogtest();

private slots:
    void on_pushButton2p1_clicked();

    void on_pushButton2p0_clicked();

    void on_pushButton3p1_clicked();

    void on_pushButton3p0_clicked();

    void on_pushButton_clicked();

    void on_pushButton3l_clicked();

    void on_pushButton3r_clicked();

    void on_pushButton2l_clicked();

    void on_pushButton2r_clicked();

    void on_pushButtonjzj_clicked();

    void on_pushButtonjzs_clicked();

    //void on_pushButtonjzs_clicked();
    
private:
    Ui::Dialogtest *ui;
};

#endif // DIALOGTEST_H
