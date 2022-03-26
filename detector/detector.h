#ifndef DETECTOR_H
#define DETECTOR_H

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class CellDetector {
public:
    //输入的原图
    Mat sourceImg;
    //位置
    Point site;
    CellDetector(Mat inputImg) : sourceImg(inputImg) {

    }
    //显示输入图
    void showSourceImg() {
        imshow("sourceImg", sourceImg);
    }
    //展示处理后的最终图
    void showDstImg() {
        imshow("dstImg", dstImg);
    }
protected:
    //结果图
    Mat dstImg;
};

class AirSwitch : public CellDetector
{
public:

    AirSwitch(Mat inputImg) : CellDetector(inputImg){
    }

    bool isUp()
    {
        vector<Mat> bgrImg(3);
        split(sourceImg, bgrImg);
        diffImg = bgrImg[1] - bgrImg[2];
        threshold(diffImg, diffImg, 10, 255, THRESH_BINARY);
        //闭运算去除噪点一次
        morphologyEx(diffImg, diffImg, CV_MOP_CLOSE, Mat(), Point(-1, -1));
        //识别轮廓
        vector<Mat> contours;
        findContours(diffImg.clone(), contours, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        vector<Rect> contourRects;
        contoursRectImg = diffImg.clone().setTo(0);
        for (size_t i = 0; i < contours.size(); i++) {
            contourRects.push_back(boundingRect(contours[i]));  //最小矩形
            rectangle(contoursRectImg, contourRects[i], Scalar(255));   //画出矩形
        }
        dstImg = contoursRectImg;
        int rectsNum = contourRects.size();
        cout << "have " << rectsNum << " rects." << endl;

        //利用矩形数据判断空气开关状态
        if (rectsNum < 1)
            cout << "无法识别空气开关状态，请退出后调试." << endl;
        else if (rectsNum == 1)
            return true;
        else
            return false;

        return true;
    }
    void showDiffImg() {
        imshow("diffImg", diffImg);
    }
    void showContoursRectImg() {
        imshow("contoursRectImg", contoursRectImg);
    }
private:
    Mat diffImg;
    Mat contoursRectImg;
};

#define RED     0
#define GREEN   1
#define BLUE    2


class Btn : public CellDetector
{
public:
    int color;
    double mean=0, stdDev=0; //均值和标准差
    int THRESHOLD_STATE = 50;

    Btn(Mat inputImg, int btnColor) : CellDetector(inputImg), color(btnColor){

    }

    bool isLighted()
    {
        vector<Mat> bgrImg(3);
        split(sourceImg, bgrImg);
        switch (color)
        {
            case RED: dstImg = bgrImg[2] - bgrImg[0]; break;
            case GREEN: dstImg = bgrImg[1] - bgrImg[2];break;
            case BLUE: ;break;
        }
        //通过计算图片的均方差判断LED亮灭状态
        Mat meanMat, stdDevMat;
        meanStdDev(dstImg, meanMat, stdDevMat);

        mean = meanMat.at<double>(0, 0);
        stdDev = stdDevMat.at<double>(0, 0);

        return (stdDev > THRESHOLD_STATE) ? true : false;
    }
};


class KnobSwitch : public CellDetector
{
public:

    KnobSwitch(Mat inputImg) : CellDetector(inputImg){
    }

    float getAngle() {
        cvtColor(sourceImg, binaryImg, CV_BGR2GRAY);
        threshold(binaryImg, binaryImg, 30, 255, THRESH_BINARY_INV);
        morphologyEx(binaryImg, binaryImg, CV_MOP_CLOSE, Mat(), Point(-1, -1));
        //获得轮廓和最小矩形框
        vector<Mat> contours;
        Rect maxRect;
        int maxContour = 0;
        findContours(binaryImg.clone(), contours, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        for (size_t i = 0; i < contours.size(); i++) {
           Rect tempRect = boundingRect(contours[i]);
           if (i == 0) {
               maxRect = tempRect;
               maxContour = 0;
           }
           else {
               if (tempRect.area() > maxRect.area()) {
                   maxRect = tempRect;
                   maxContour = i;
               }
           }
        }
        dstImg = binaryImg(maxRect);
        //获得最小矩形(可斜着)
        RotatedRect rotatedRect = minAreaRect(contours[maxContour]);

        return rotatedRect.angle;
    }
    void showBinaryImg() {
        imshow("binaryImg", binaryImg);
    }

private:
    Mat binaryImg;
};



#endif // DETECTOR_H
