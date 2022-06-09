#ifndef DIALOGGL_H
#define DIALOGGL_H

#include <QDialog>
#include "dialogusergl.h"
#include "dialogparaset.h"
//#include "dialogpaotoset.h"
//#include "dialogbjlabel.h"
//#include "dialogpfgl.h"
#include "dialogequset.h"
#include "dialogkktype.h"

namespace Ui {
class Dialoggl;
}

class Dialoggl : public QDialog
{
    Q_OBJECT

public:
    explicit Dialoggl(QWidget *parent = 0);
    ~Dialoggl();
    QString button_style1;
    QString button_style2;
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

  //  void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::Dialoggl *ui;
    Dialogusergl *dlguser;

   // Dialogequset *dlgequ;
    Dialogparaset *dlgset;
    Dialogkktype *dlgkktype;
    //Dialogpaotoset *dlgphoto;

    //Dialogpflabel *dlglabel;
};

#endif // DIALOGGL_H
