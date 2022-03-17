#ifndef DIALOGLOGIN_H
#define DIALOGLOGIN_H

#include <QDialog>

namespace Ui {
class Dialoglogin;
}

class Dialoglogin : public QDialog
{
    Q_OBJECT

public:
    explicit Dialoglogin(QWidget *parent = 0);
    ~Dialoglogin();

private slots:
    void on_pblogin_clicked();

    void on_pushButton_2_clicked();

    void on_Dialoglogin_destroyed();

private:
    Ui::Dialoglogin *ui;
};

#endif // DIALOGLOGIN_H
