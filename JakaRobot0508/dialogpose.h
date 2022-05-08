#ifndef DIALOGPOSE_H
#define DIALOGPOSE_H

#include <QDialog>

namespace Ui {
class Dialogpose;
}

class Dialogpose : public QDialog
{
    Q_OBJECT

public:
    explicit Dialogpose(QWidget *parent = nullptr);
    ~Dialogpose();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Dialogpose *ui;
};

#endif // DIALOGPOSE_H
