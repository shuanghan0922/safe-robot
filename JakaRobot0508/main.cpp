#include "mainwindow.h"
//#include "JAKAZuRobot.h"
#include <QApplication>
//#include "ComData.h"
#include  "connect.h"
#include "dialoglogin.h"
#include <opencv2/opencv.hpp>
/************************
//测试能否调用摄像头：
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;
******************/
#define PI 3.1415926

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    if(!createConnection()) return 1;

    Dialoglogin login;//登录
  //Dialogpara dlgpara;
    int flag=login.exec();
   // int flagpara;
    if(flag==!QDialog::Accepted) return 0;

    /*****************
    VideoCapture capture (0);
        Mat src;
        while(1)
        {

            capture >> src;
            imshow("capture",src);
            char(key)=(char)waitKey(1);
            if(key==27)
                break;
        }

    ****************/




    MainWindow w;
    w.show();
    cv::waitKey(0);
    cv::destroyAllWindows();
    return a.exec();
}
