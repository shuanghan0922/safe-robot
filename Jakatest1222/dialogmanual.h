#ifndef DIALOGMANUAL_H
#define DIALOGMANUAL_H
#include "mygauge2.h"

#include <QDialog>
#include <QVBoxLayout>

namespace Ui {
class DialogManual;
}

class DialogManual : public QDialog
{
    Q_OBJECT

public:
    explicit DialogManual(QWidget *parent = nullptr);
    ~DialogManual();
    void paintEvent(QPaintEvent *);
    QTimer *timerstu;
    void kkopt(QString no,int st);//开关编号，到达状态
    void drawlamp(int x,int y,int radius,float scale,int col);
signals:
    void Sequid(const QString id);//send to mainwindow

private slots:
    void dispstu();
    void on_pushButton_clicked();

    void on_pushButtonman_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton2pon_clicked();

    void on_pushButton2poff_clicked();

    void on_pushButton3pon_clicked();

    void on_pushButton3poff_clicked();

    void on_pushButtonon21_clicked();

    void on_pushButtonoff21_clicked();

    void on_pushButtonon22_clicked();

    void on_pushButtonoff22_clicked();

    void on_pushButton22min_clicked();

    void on_pushButtonon23_clicked();

    void on_pushButtonoff23_clicked();

    void on_pushButtonon24_clicked();

    void on_pushButtonoff24_clicked();

    void on_pushButtonon25_clicked();

    void on_pushButtonoff25_clicked();

    void on_pushButton1on_clicked();

    void on_pushButton1off_clicked();

    void on_pushButton2on_clicked();

    void on_pushButton2off_clicked();

    void on_pushButton3on_clicked();

    void on_pushButton3off_clicked();

    void on_pushButton4on_clicked();

    void on_pushButton4off_clicked();

    void on_pushButton31on_clicked();

    void on_pushButton31off_clicked();

    void on_pushButton32on_clicked();

    void on_pushButton32off_clicked();

    void on_pushButton32min_clicked();

    void on_pushButton33on_clicked();

    void on_pushButton33off_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButtonloc_clicked();

    void on_pushButtonfar_clicked();

    void on_pushButtonzk1off_clicked();

    void on_pushButtonzk1on_clicked();

    void on_pushButtonbreoff_clicked();

    void on_pushButtonbreon_clicked();

    void on_pushButtondissoff_clicked();

    void on_pushButtondisson_clicked();

    void on_pushButtonearoff_clicked();

    void on_pushButtonearon_clicked();

    void on_pushButtonzk2off_clicked();

    void on_pushButtonzk2on_clicked();

    void on_pushButtonzk3off_clicked();

    void on_pushButtonzk3on_clicked();

    void on_pushButton_37_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_pushButton_38_clicked();

    void on_pushButton2poff_2_clicked();

    void on_pushButton2pon_2_clicked();

    void on_pushButtontest_clicked();

    void on_pushButtontest_2_clicked();

    void on_pushButtontest_3_clicked();

private:
    Ui::DialogManual *ui;
    //room1
    myGauge2 *sels;
    myGauge2 *ears;
    myGauge2 *diss;
    myGauge2 *bres;
    QVBoxLayout *mainLayout1;
    QVBoxLayout *mainLayout2;
    QVBoxLayout *mainLayout3;
    QVBoxLayout *mainLayout4;
    //room2
    myGauge2 *sel21;
    QVBoxLayout *mainLayout21;
    myGauge2 *sel22;
    QVBoxLayout *mainLayout22;
    myGauge2 *sel23;
    QVBoxLayout *mainLayout23;
    myGauge2 *sel24;
    QVBoxLayout *mainLayout24;
    myGauge2 *sel25;
    QVBoxLayout *mainLayout25;
    myGauge2 *sel26;
    QVBoxLayout *mainLayout26;
    myGauge2 *sel27;
    QVBoxLayout *mainLayout27;
    myGauge2 *sel28;
    QVBoxLayout *mainLayout28;
    //测试
    myGauge2 *xn21;
    QVBoxLayout *mainLayout21t;
    myGauge2 *xn22;
    QVBoxLayout *mainLayout22t;
    myGauge2 *xn23;
    QVBoxLayout *mainLayout23t;
    myGauge2 *xn24;
    QVBoxLayout *mainLayout24t;
    myGauge2 *xn25;
    QVBoxLayout *mainLayout25t;
    myGauge2 *xn26;
    QVBoxLayout *mainLayout26t;
    myGauge2 *xn27;
    QVBoxLayout *mainLayout27t;
    myGauge2 *xn28;
    QVBoxLayout *mainLayout28t;


};

#endif // DIALOGMANUAL_H
