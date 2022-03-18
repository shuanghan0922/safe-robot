#ifndef DIALOGJAKAMOVETEST_H
#define DIALOGJAKAMOVETEST_H

#include <QDialog>

namespace Ui {
class DialogJakamovetest;
}

class DialogJakamovetest : public QDialog
{
    Q_OBJECT

public:
    explicit DialogJakamovetest(QWidget *parent = nullptr);
    ~DialogJakamovetest();

private slots:
    void on_pushButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::DialogJakamovetest *ui;
};

#endif // DIALOGJAKAMOVETEST_H
