#include <iostream>
#include "../../detector/detector.h"
#include "../calibration/calibration/calibration.h"

using namespace std;
using namespace cv;

void line();    //线标定
void square();  //矩形标定
void btn();     //按钮标定

int main()
{
    string dir = "../../resource/image/calibration/btn/1.jpg";
    cv::Mat sourceImg = cv::imread(dir);

//    CalibrationLine *line = new CalibrationLine(sourceImg);
//    line->detect();
//    line->showDstImg();

//    CalibrationSquare *square = new  CalibrationSquare(sourceImg);
//    square->detect();

    CalibrationBtn *btn = new CalibrationBtn(sourceImg);
    btn->detect();
    btn->showSourceImg();
    btn->showDstImg();




    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}

void line() {
    string dir = "../../resource/image/calibration/line/1.jpg";
    cv::Mat sourceImg = cv::imread(dir);
//    imshow("sourceImg", sourceImg); //显示原图
    //取出黄色line
    vector<cv::Mat> srcLineImg;
    cv::split(sourceImg, srcLineImg);
    cv::Mat greenBtn = srcLineImg[1] - srcLineImg[2];
    cv::Mat redBtn = srcLineImg[2] - srcLineImg[1];
    cv::Mat lineImg = srcLineImg[2]- srcLineImg[0] - redBtn - greenBtn;
//    imshow("yelloLineImg", lineImg);    //显示黄线图
    //查找角点
    cv::Mat allCornerLineImg = lineImg.clone();
    vector<Point> corners;
    goodFeaturesToTrack(lineImg, corners, 12, 0.1, 10);
    cout << "查找到:" << corners.size() << " 个角点" << endl;
    for (auto p : corners) {
        circle(allCornerLineImg, p, 5, Scalar(255), 2);
    }
//    cv::imshow("allCornerImg", allCornerLineImg);    //显示全部角点
    //排除掉最下面的4个角点
    cv::Mat  curCornerLineImg = lineImg.clone();
    sort(corners.begin(), corners.end(), [](Point x, Point y){
        return (x.y < y.y);
    });
    for (int i = 4; i > 0; i--)
        corners.pop_back();     //删除末尾四个元素
    for (auto p : corners) {
        circle(curCornerLineImg, p, 5, Scalar(255), 2);
    }
    cv::imshow("cornerImg", curCornerLineImg);      //显示有用的角点
//    cv::imshow("lineImg", lineImg);
    //可根据四个点的信息进行校正
    for (auto p : corners)
        cout << p << endl;
}
void square() {
    string dir = "../../resource/image/calibration/square/3.jpg";
    cv::Mat sourceImg = cv::imread(dir, CAP_MODE_GRAY);
//    cv::imshow("sourceImg", sourceImg);     //显示原图
    //预处理
    //反阈值
    threshold(sourceImg, sourceImg, 90, 255, THRESH_BINARY_INV);
    morphologyEx(sourceImg, sourceImg, CV_MOP_CLOSE, Mat());    //闭操作
    cv::imshow("thresholdImg", sourceImg);
    //轮廓识别 + 最大矩形排序
    vector<Mat> contours;
    findContours(sourceImg.clone(), contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
    vector<Rect> contourRects;
    cv::Mat contoursImg = sourceImg.clone().setTo(0);
    for (size_t i = 0; i < contours.size(); i++) {
        contourRects.push_back(boundingRect(contours[i]));  //最小矩形
        rectangle(contoursImg, contourRects[i], Scalar(255));   //画出矩形
    }
//    imshow("contoursImg", contoursImg); //显示全部轮廓图
    sort(contourRects.begin(), contourRects.end(), [](cv::Rect x, cv::Rect y){
        return x.area() > y.area();
    }); //按照矩形面积从大到小进行排序
    //去掉除了最大轮廓外的所有杂图
    cv::Mat uselessImg = sourceImg.clone().setTo(0);
    for (size_t i = 2; i < contourRects.size(); i++) {
        sourceImg(contourRects[i]).copyTo(uselessImg(contourRects[i]));
    }
    sourceImg -= uselessImg;
    imshow("dstImg", sourceImg);     //最大的矩形框
    //从识别到的最大轮廓周围识别角点
    vector<cv::Rect> fourCorners(4);
    Rect maxContour = contourRects[0];
    int rectSize = 30;
    fourCorners[0] = Rect(maxContour.tl() + Point(-rectSize, -rectSize),
            contourRects[0].tl() + Point(rectSize, rectSize));
    fourCorners[1] = Rect(maxContour.tl() + Point(maxContour.width, 0) + Point(rectSize, -rectSize),
            maxContour.tl() + Point(maxContour.width, 0) + Point(-rectSize, rectSize));
    fourCorners[2] = Rect(maxContour.tl() + Point(0, maxContour.height) + Point(-rectSize, -rectSize),
            maxContour.tl() + Point(0, maxContour.height) + Point(rectSize, rectSize));
    fourCorners[3] = Rect(maxContour.br() + Point(-rectSize, -rectSize),
            maxContour.br() + Point(rectSize, rectSize));

    cv::Mat cornerImg = sourceImg.clone().setTo(0);
    for(auto r : fourCorners) {
        sourceImg(r).copyTo(cornerImg(r));
    }

    vector<Point> corners;
    goodFeaturesToTrack(sourceImg, corners, 4, 0.5, 10);
    cout << "查找到:" << corners.size() << " 个角点" << endl;
    for (auto p : corners) {
        circle(sourceImg, p, 5, Scalar(255), 2);
    }
    cv::imshow("cornerImg", sourceImg);
}
void btn() {
    string dir = "../../resource/image/calibration/btn/1.jpg";
    cv::Mat sourceImg = cv::imread(dir);
    detect::Btn btn(sourceImg, detect::BtnColor::red);



//    greenBtn.detect();
//    cv::Mat binaryImg =  greenBtn.showBinaryImg(false);
//    cv::Mat greenImg = sourceImg.clone().setTo(0);

//    sourceImg(greenBtn.getRect()).setTo(0).copyTo(sourceImg(greenBtn.getRect()));

//    greenBtn.reloadSourceImg(sourceImg);
//    greenBtn.detect();
//    sourceImg(greenBtn.getRect()).setTo(0).copyTo(sourceImg(greenBtn.getRect()));
//    imshow("sourceImg", sourceImg);

    btn.detect();
    cv::Mat binaryImg =  btn.showBinaryImg(false);
    cv::Mat greenImg = sourceImg.clone().setTo(0);
    sourceImg(btn.getRect()).setTo(0).copyTo(sourceImg(btn.getRect()));
    //重载图片，涂黑后再次检测
    btn.reloadSourceImg(sourceImg);
    btn.detect();
    sourceImg(btn.getRect()).setTo(0).copyTo(sourceImg(btn.getRect()));

    imshow("sourceImg", sourceImg);
}
