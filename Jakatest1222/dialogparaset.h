#ifndef DIALOGPARASET_H
#define DIALOGPARASET_H

#include <QDialog>
//****************
#include <QtGui>
#include <QLabel>
//****************/
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QRadioButton>

//#include "capturethread.h"
//#include "CameraApi.h"
//#include "paintwidget.h"

/************
typedef struct _WIDTH_HEIGHT{
    int     display_width;
    int     display_height;
    int     xOffsetFOV;
    int     yOffsetFOV;
    int     sensor_width;
    int     sensor_height;
    int     buffer_size;
}Width_Height;
*****************/
namespace Ui {
class Dialogparaset;
}

class Dialogparaset : public QDialog
{
    Q_OBJECT

public:
    explicit Dialogparaset(QWidget *parent = 0);
    ~Dialogparaset();
    void cpshow(int row, int col, bool status);
    //void closeEvent(QCloseEvent *);
    //int status;
    void paintEvent(QPaintEvent *);
    void disppf(); //read pfdata
    void drawcp(int x,int y,int width,int length,int row,int col,int krow,int kcol,int gap,int colrec);
private slots:
    void on_pushButton_clicked();

   // void on_comboBoxpf_currentTextChanged(const QString &arg1);

   // void on_comboBoxcp_currentTextChanged(const QString &arg1);

    void on_pushButton_2_clicked();
    /****************
    void on_radioButton_trigger_hardware_clicked(bool checked);
    void on_radioButton_software_trigger_clicked(bool checked);
    void on_radioButton_collect_clicked(bool checked);
    void on_flashlight_l_clicked(bool checked);
    void on_flashlight_h_clicked(bool checked);
    void on_flashlight_manual_clicked(bool checked);
    void on_flashlight_auto_clicked(bool checked);
    void on_lineEdit_exposure_time_returnPressed();
    void on_radioButton_raw_clicked();
    void on_radioButton_bmp_clicked();
    void on_pushButton_snap_catch_released();
    void on_pushButton_snap_path_released();
    void on_pushButton_para_load_released();
    void on_pushButton_para_save_released();
    void on_pushButton_para_acquiesce_released();
    void on_radioButton_D_clicked(bool checked);
    void on_radioButton_C_clicked(bool checked);
    void on_radioButton_B_clicked(bool checked);
   // void on_horizontalSlider_isp_acutance_valueChanged(int value);
   // void on_checkBox_isp_v_clicked(bool checked);
   // void on_checkBox_isp_h_clicked(bool checked);
    void on_contrast_horizontalSlider_valueChanged(int value);
    void on_gamma_horizontalSlider_valueChanged(int value);
   // void on_horizontalSlider_saturation_valueChanged(int value);
   // void on_horizontalSlider_gain_b_valueChanged(int value);
   // void on_horizontalSlider_gain_g_valueChanged(int value);
  //  void on_horizontalSlider_gain_r_valueChanged(int value);
    void on_horizontalSlider_exposure_time_valueChanged(int value);
    void on_horizontalSlider_gain_valueChanged(int value);
  //  void on_AWB_once_button_released();
    void on_res_combobox_activated(int index);

    void Image_process(QImage img);
    void camera_statues();

    void on_radioButton_A_clicked(bool checked);
    void on_exposure_mode_manual_clicked(bool checked);
    void on_exposure_mode_auto_clicked(bool checked);
    void on_radioButton_60HZ_clicked(bool checked);
    void on_radioButton_50HZ_clicked(bool checked);
    void on_software_trigger_once_button_clicked();
    void on_flicker_checkBox_clicked(bool checked);
    void on_AE_horizontalSlider_valueChanged(int value);

    void radioChange();

   // void on_radioButton_software_trigger_clicked();

    void on_tabWidget_currentChanged(int index);

  //  void on_radioButton_software_trigger_released();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:

    QTimer *m_timer;
    QLabel *m_camera_statuesFps;
    bool   exposure_time_lineedit_status;

    PaintWidget *m_scene;
    QGraphicsPixmapItem *m_image_item;
    QRadioButton *radioButton_speed[3];

    CaptureThread *m_thread;
********************/


    void on_tabWidget_currentChanged(int index);
/****************
    void on_checkBoxcy_clicked();

    void on_checkBoxpp_clicked();

    void on_checkBoxsl_clicked();

    void on_checkBoxur_clicked();

    void on_checkBoxsf_clicked();

    void on_checkBoxsj_clicked();
**********************/

    void on_lineEditpic_returnPressed();

    void on_lineEditalarm_returnPressed();

    void on_lineEditopt_returnPressed();

    void on_lineEditplcip_returnPressed();

    void on_lineEditplcport_returnPressed();

    void on_lineEditpcip_returnPressed();

    void on_lineEditsecsip_returnPressed();

    void on_lineEditsecsport_returnPressed();

    void on_lineEditdjstep_returnPressed();

    void on_lineEditupdjstep_returnPressed();

    void on_lineEditzfreshold_returnPressed();

    void on_lineEditffreshold_returnPressed();

    void on_lineEditkg_returnPressed();

    void on_lineEditkg_2_returnPressed();

    void on_lineEdit_returnPressed();

    void on_lineEdit_2_returnPressed();

    void on_pushButton_3_clicked();

    void on_lineEditcullfreshold_returnPressed();

    void on_lineEditboxfreshold_returnPressed();

    void on_lineEditfreshold1_returnPressed();

    void on_lineEditfreshold2_returnPressed();

    void on_lineEditfreshold3_returnPressed();

    void on_lineEditfreshold41_returnPressed();

    void on_lineEditfreshold42_returnPressed();

    void on_lineEditfreshold43_returnPressed();

    void on_lineEditfreshold44_returnPressed();

    //void on_checkBox_clicked();

    //void on_checkBox_2_clicked();

    void on_checkBox_3_clicked();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

  //  void on_lineEditpcip_cursorPositionChanged(int arg1, int arg2);

   // void on_lineEditplcip_2_returnPressed();

   // void on_lineEditplcip_cursorPositionChanged(int arg1, int arg2);

    void on_lineEditplcip2_returnPressed();

    void on_lineEditplcip3_returnPressed();

    void on_lineEditplcport2_returnPressed();

    void on_lineEditplcport3_returnPressed();

   // void on_checkBox_clicked(bool checked);

   // void on_checkBox_2_clicked(bool checked);

    void on_checkBox_3_clicked(bool checked);

    void on_checkBox_4_clicked(bool checked);

   // void on_lineEditplcip_cursorPositionChanged(int arg1, int arg2);

    void on_lineEditdx_returnPressed();

    void on_lineEditdy_returnPressed();

    void on_lineEditdz_returnPressed();

    void on_lineEditinpara00_returnPressed();

    void on_lineEditinpara01_returnPressed();

    void on_lineEditinpara02_returnPressed();

    void on_lineEditinpara10_returnPressed();

    void on_lineEditinpara11_returnPressed();

    void on_lineEditinpara12_returnPressed();

    void on_lineEditinpara20_returnPressed();

    void on_lineEditinpara21_returnPressed();

    void on_lineEditinpara22_returnPressed();

private:
    Ui::Dialogparaset *ui;
    bool bline;

    /*****************
    int init_SDK();
    int  GUI_init_parameter(int hCamera,tSdkCameraCapbility * pCameraInfo);
    int  GUI_init_Resolution(int hCamera,tSdkCameraCapbility * pCameraInfo);
    int  GUI_set_Resolution(int hCamera,tSdkCameraCapbility * pCameraInfo);
    int  GUI_init_exposure(int hCamera,tSdkCameraCapbility * pCameraInfo);
    int  GUI_init_WB(int hCamera,tSdkCameraCapbility * pCameraInfo);
    int  GUI_init_mmap(int hCamera,tSdkCameraCapbility * pCameraInfo);
    int  GUI_init_isp(int hCamera,tSdkCameraCapbility * pCameraInfo);
    int  GUI_init_speed(int hCamera,tSdkCameraCapbility * pCameraInfo);
    int  GUI_set_speed(int hCamera,tSdkCameraCapbility * pCameraInfo);
    int  GUI_init_Trigger(int hCamera,tSdkCameraCapbility * pCameraInfo);
    *****************/
};

#endif // DIALOGPARASET_H
