#ifndef DETECTOR_H
#define DETECTOR_H

//#define DEBUG_DETECTOR  //detector调试码

#include <iostream>
#include <opencv2/opencv.hpp>
#include<vector>
#include <fstream>
#include <opencv2/dnn/dnn.hpp>
#include <librealsense2/rs.hpp>
#ifndef DEBUG_DETECTOR
#include <ComData.h>

#endif

using namespace std;
using namespace cv;
using namespace cv::dnn;

#ifndef DEBUG_DETECTOR

float getPointDepth(Point point) {
    //相机设置
    rs2::colorizer color_map;
    rs2::align align_to(RS2_STREAM_COLOR);
    rs2::spatial_filter spat_filter;
    rs2::threshold_filter thr_filter;
    rs2::temporal_filter tem_filter;
    rs2::hole_filling_filter hole_filter;
    rs2::disparity_transform depth_to_disparity(true);
    rs2::disparity_transform disparity_to_depth(false);
    thr_filter.set_option(RS2_OPTION_MIN_DISTANCE, 0.0f);
    thr_filter.set_option(RS2_OPTION_MAX_DISTANCE, 4.f);
    spat_filter.set_option(RS2_OPTION_FILTER_SMOOTH_ALPHA, 0.55f);
    spat_filter.set_option(RS2_OPTION_FILTER_SMOOTH_DELTA, 20);
    spat_filter.set_option(RS2_OPTION_FILTER_MAGNITUDE, 2);
    spat_filter.set_option(RS2_OPTION_HOLES_FILL, 4);
    tem_filter.set_option(RS2_OPTION_FILTER_SMOOTH_DELTA, 4);
    tem_filter.set_option(RS2_OPTION_FILTER_SMOOTH_ALPHA, 0.4f);
    tem_filter.set_option(RS2_OPTION_FILTER_SMOOTH_DELTA, 20);
    //信息配置
    rs2::frameset data= pipe1.wait_for_frames();
    rs2::frameset aligned_set = align_to.process(data);
    rs2::depth_frame depth_frame = aligned_set.get_depth_frame();
    rs2::depth_frame filtered = depth_frame;
    filtered = thr_filter.process(filtered);
    filtered = depth_to_disparity.process(filtered);
    filtered = hole_filter.process(filtered);
    filtered = disparity_to_depth.process(filtered);
    filtered = spat_filter.process(filtered);
    filtered = tem_filter.process(filtered);
    //提取深度信息
    vector<float> distacnceVector(5);
    distacnceVector[0] = filtered.get_distance(cvRound(point.x), cvRound(point.y));
    distacnceVector[1] = filtered.get_distance(cvRound(point.x)-1, cvRound(point.y)-1);
    distacnceVector[2] = filtered.get_distance(cvRound(point.x)-1, cvRound(point.y)+1);
    distacnceVector[3] = filtered.get_distance(cvRound(point.x)+1, cvRound(point.y)-1);
    distacnceVector[4] = filtered.get_distance(cvRound(point.x)+1, cvRound(point.y)+1);

    float pointDistance = 0;
    for (auto i : distacnceVector)
        pointDistance += i;

    return pointDistance/5;
}

#endif
namespace detect {

//元件检测器
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
    void showBinaryImg() {
        imshow("binaryImg", binaryImg);
    }
    friend void getPrivateData(CellDetector& cellDetector); //该函数用于外部调用内部私有变量
//protected:
    //结果图
    Mat dstImg;
    Mat binaryImg;  //过程图
};
//圆检测器
class DetectCircle :public CellDetector {
public:
    vector<Vec3f> circleInfo;
    DetectCircle(Mat inputImg) : CellDetector(inputImg) {

    }
    //获取圆，使用HoughCircles
    void getCircles(double dp = 1, double minDist = 10) {
        HoughCircles(sourceImg, circleInfo, HOUGH_GRADIENT, dp, minDist, 80, 40, 0, 0);
        cout << (circleInfo.size() >= 1 ? "find circles" : "not find circles") << endl;
        //circles
        dstImg = sourceImg.clone();
        for (auto c : circleInfo) {
            circle(dstImg, Point(c[0], c[1]), 2, Scalar(255));
        }
    }
    //打印所有圆的信息
    void printCircleInfo() {
        for (auto i : circleInfo)
            cout << i << endl;
    }
};
//角点检测
class DetectRect : public CellDetector {
public:
    vector<Point> corners;
    vector<Vec3f> cornersWithDepth;

    DetectRect(Mat inputImg) : CellDetector(inputImg) {
    }
    //获取角点和深度(已经排序好顺序点)
    void getCornersWitdhDepth() {
        goodFeaturesToTrack(sourceImg, corners, 3, 0.1, 10);
        dstImg = sourceImg.clone();
        for (auto c : corners) {
            circle(dstImg, c, 2, Scalar(255));
        }
        //sort
        sortConrersUseCircle();
        for (auto point : corners) { //add corners' depth info
#ifndef DEBUG_DETECTOR
            cornersWithDepth.push_back( Vec3f(point.x, point.y, getPointDepth(point + Point(In_parameter(0,2)-100, In_parameter(1,2)-100))) );
#endif
        }
    }
    void printCorners() {
        for (auto i : corners)
            cout << i << endl;
    }
    void printCornersWithDistance() {
        cout << "find " << corners.size() << " points" << endl;
        for (auto i : cornersWithDepth)
            cout << i << endl;
    }
private:
    void sortConrersUseCircle() {
        vector<Point> sortedCorner(3, Point(0, 0)); //maxDistance:0
        DetectCircle detectCircle(sourceImg);
        detectCircle.getCircles();
        cout << "we have " << detectCircle.circleInfo.size() << " circles." << endl;
        detectCircle.printCircleInfo();

        //core
        Vec3f circlePoint;
        //if we have many circle, we get average
        for (auto circle : detectCircle.circleInfo) {
            circlePoint += circle;
        }
        circlePoint[0] /= detectCircle.circleInfo.size();
        circlePoint[1] /= detectCircle.circleInfo.size();
        circlePoint[2] /= detectCircle.circleInfo.size();
        //get max distance of corner to circle center
        float maxDistance = 0;
        int maxNo = 0;
        for (int i = 0; i < 3; i++) {
            double distance = pow(corners[i].x - circlePoint[0], 2) + pow(corners[i].y - circlePoint[1], 2);
            if (distance > maxDistance) {
                maxDistance = distance;
                maxNo = i;
            }
        }
        cout << "maxDistance: " << maxDistance << ". maxNo: " << maxNo << endl;
        sortedCorner[0] = corners[maxNo];   //maxDistance
        switch (maxNo) {
            case 0 : {
                if (corners[1].x > corners[2].x) {
                    sortedCorner[1] = corners[1];
                    sortedCorner[2] = corners[2];
                }
                else {
                    sortedCorner[1] = corners[2];
                    sortedCorner[2] = corners[1];
                }
                break;
            }
            case 1 : {
                if (corners[0].x > corners[2].x) {
                    sortedCorner[1] = corners[0];
                    sortedCorner[2] = corners[2];
                }
                else {
                    sortedCorner[1] = corners[2];
                    sortedCorner[2] = corners[0];
                }
                break;
            }
            case 2 : {
                if (corners[0].x > corners[1].x) {
                    sortedCorner[1] = corners[0];
                    sortedCorner[2] = corners[1];
                }
                else {
                    sortedCorner[1] = corners[1];
                    sortedCorner[2] = corners[0];
                }
                break;
            }
        }
        corners = sortedCorner; //sorted corners
    }
};
//空开检测
class AirSwitch : public CellDetector
{
public:

    AirSwitch(Mat inputImg) : CellDetector(inputImg){
    }
    bool isUp()
    {
        vector<Mat> bgrImg(3);
        split(sourceImg, bgrImg);
        binaryImg = bgrImg[1] - bgrImg[2];
        threshold(binaryImg, binaryImg, 10, 255, THRESH_BINARY);
        //闭运算去除噪点一次
        morphologyEx(binaryImg, binaryImg, MORPH_CLOSE, Mat(), Point(-1, -1));
        //识别轮廓
        vector<Mat> contours;
        findContours(binaryImg.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        vector<Rect> contourRects;
        dstImg = sourceImg; //目标图
        for (size_t i = 0; i < contours.size(); i++) {
            contourRects.push_back(boundingRect(contours[i]));  //最小矩形
            rectangle(dstImg, contourRects[i], Scalar(255));   //画出矩形
        }
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
};

#define BTN_RED     0
#define BTN_GREEN   1
#define BTN_BLUE    2
enum BtnColor {
    red = 0, green, blue
};
//按钮检测
class Btn : public CellDetector
{
public:
    BtnColor color;
    bool btnState;
    double mean=0, stdDev=0; //均值和标准差
    int THRESHOLD_STATE = 25; //50 detect by image size

    Btn(Mat inputImg) : CellDetector(inputImg) {
        color = BtnColor::blue;
    }
    Btn(Mat inputImg, BtnColor btnColor) : CellDetector(inputImg), color(btnColor){
    }
    //位置按钮颜色的情况下使用
    void getBtn() {
        vector<Mat> bgrImg(3);
        split(sourceImg, bgrImg);
        //先列举中每种颜色
        Mat redImg, greenImg, blueImg;
        redImg = bgrImg[2] - bgrImg[0];
        greenImg = bgrImg[1] - bgrImg[2];
        //判断按钮是什么颜色的
        Mat meanMatRed, meanMatGreen, stdDevMatRed, stdDevMatGreen;
        meanStdDev(redImg, meanMatRed, stdDevMatRed);
        meanStdDev(greenImg, meanMatGreen, stdDevMatGreen);
        double stdDevRed = meanMatRed.at<double>(0, 0);
        double stdDevGreen = meanMatGreen.at<double>(0, 0);

        if (stdDevGreen > stdDevRed) {
            color = BtnColor::green;
            dstImg = greenImg;
            btnState = stdDevGreen > THRESHOLD_STATE ? true : false;
        }
        else {
            color = BtnColor::red;
            dstImg = redImg;
            btnState = stdDevRed > THRESHOLD_STATE ? true : false;
        }
    }
    //已知按钮的颜色的情况下使用
    bool isLighted()
    {
        vector<Mat> bgrImg(3);
        split(sourceImg, bgrImg);
        switch (color)
        {
            case red: dstImg = bgrImg[2] - bgrImg[0]; break;
            case green: dstImg = bgrImg[1] - bgrImg[2];break;
            case blue: ;break;
        }
        //通过计算图片的均方差判断LED亮灭状态
        Mat meanMat, stdDevMat;
        meanStdDev(dstImg, meanMat, stdDevMat);

        mean = meanMat.at<double>(0, 0);
        stdDev = stdDevMat.at<double>(0, 0);

        return (stdDev > THRESHOLD_STATE) ? true : false;
    }
};
//旋钮检测
class KnobSwitch : public CellDetector
{
public:
    float angle = 0.0;
    Mat knobSwitchTestImg;
    KnobSwitch(Mat inputImg) : CellDetector(inputImg){
    }
    void getKnobSwitch() {
        //方案1：叠加法
        Mat overlayImg = sourceImg;
        for (int i = 2; i > 0; i--)
            overlayImg += overlayImg;
        cvtColor(overlayImg, binaryImg, COLOR_BGR2GRAY);
        //方案2：mean-shift

        //获得轮廓和最小矩形框
        vector<Mat> contours;
        Rect maxRect;
        int maxContour = 0;
        findContours(binaryImg.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        //获取最大矩形
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
        dstImg = sourceImg.clone();
        //获得最小矩形(可斜着)
        RotatedRect rotatedRect = minAreaRect(contours[maxContour]);
        Point2i center(rotatedRect.center);
        circle(dstImg, center, 5, Scalar(255), 2);
        knobSwitchTestImg = dstImg;
        site = center;  //位置
        angle = rotatedRect.angle;
    }

};


int numofText=0;

class TextRecongize{
public:
    //文本区域仿射变化预处理:将所有方框变成水平
    void fourPointsTransform(const Mat& src, const Point2f vertices[], Mat& result);

    //检测结果
    vector<string> recongize_text(Mat src);

    TextRecongize(){};
    ~TextRecongize(){};

private:
    //DB文本检测模型参数
    float binThresh = 0.3;
    float polyThresh  = 0.5;
    double unclipRatio = 2.0;
    uint maxCandidates = 200;
    int height = 736;
    int width = 736;
    cv::String detModelPath = "/home/xzh/JakaRobot/TEXT_RECONGIZE/recognition-text/DB_TD500_resnet50.onnx";
    double detScale = 1.0 / 255.0;

    //CRNN文本识别模型参数
    cv::String recModelPath = "/home/xzh/JakaRobot/TEXT_RECONGIZE/recognition-text/crnn_cs_CN.onnx";
    cv::String vocPath = "/home/xzh/JakaRobot/TEXT_RECONGIZE/recognition-text/alphabet_3944.txt";
    int imreadRGB = 1;         //0：以灰度图读取图像   1：以彩色图读取图像
    double recScale = 1.0 / 127.5;

};

void TextRecongize::fourPointsTransform(const Mat& src, const Point2f vertices[], Mat& result){
    const Size outputSize = Size(100, 32);
    Point2f targetVertices[4] = {
        Point(0, outputSize.height - 1),
        Point(0, 0),
        Point(outputSize.width - 1, 0),
        Point(outputSize.width - 1, outputSize.height - 1)
    };
    Mat rotationMatrix = getPerspectiveTransform(vertices, targetVertices);

    warpPerspective(src, result, rotationMatrix, outputSize);
//显示检测区域
/* #if 1
    imshow("roi", result);
    waitKey();
#endif */
}

vector<string> TextRecongize::recongize_text(Mat src){
    vector<string> text_result;

    TextDetectionModel_DB detector(detModelPath);
    detector.setBinaryThreshold(binThresh);
    detector.setPolygonThreshold(polyThresh);
    detector.setUnclipRatio(unclipRatio);
    detector.setMaxCandidates(maxCandidates);

    TextRecognitionModel recognizer(recModelPath);
    recognizer.setDecodeType("CTC-greedy");

    std::ifstream vocFile;
    vocFile.open(samples::findFile(vocPath));
    CV_Assert(vocFile.is_open());
    String vocLine;
    std::vector<String> vocabulary;
    while (std::getline(vocFile, vocLine)) {
        vocabulary.push_back(vocLine);
    }

    recognizer.setVocabulary(vocabulary);

    //设置检测参数
    Size detInputSize = Size(width, height);
    Scalar detMean = Scalar(122.67891434, 116.66876762, 104.00698793);
    detector.setInputParams(detScale, detInputSize, detMean);

    //设置识别参数
    Scalar recMean = Scalar(127.5);
    Size recInputSize = Size(100, 32);
    recognizer.setInputParams(recScale, recInputSize, recMean);

    // 创建窗口
    //static const std::string winName = "Text_Recongize";

    std::vector< std::vector<Point> > detResults;
    detector.detect(src, detResults);
    numofText=detResults.size();

    if (detResults.size() > 0) {
        //文本识别
        Mat recInput;
        if (!imreadRGB) {
            cvtColor(src, recInput, cv::COLOR_BGR2GRAY);
        } else {
            recInput = src;
        }
        std::vector< std::vector<Point> > contours;
        for (uint i = 0; i < detResults.size(); i++)
        {
            const auto& quadrangle = detResults[i];
            CV_CheckEQ(quadrangle.size(), (size_t)4, "");       //检测Mat是否为Vector，是否为4个点

            contours.emplace_back(quadrangle);                      //插入数据到向量

            std::vector<Point2f> quadrangle_2f;
            for (int j = 0; j < 4; j++)
                quadrangle_2f.emplace_back(quadrangle[j]);

            // 转换和裁剪图像
            Mat cropped;
            TextRecongize::fourPointsTransform(recInput, &quadrangle_2f[0], cropped);

            std::string recognitionResult = recognizer.recognize(cropped);
            //std::cout << i << ": '" << recognitionResult << "'" << std::endl;
            text_result.push_back(recognitionResult);

            putText(src, recognitionResult, quadrangle[3], FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2);
        }
        polylines(src, contours, true, Scalar(0, 255, 0), 2);
    } else {
        std::cout << "No Text Detected." << std::endl;
    }
    imwrite("text_result.png",src);

    return text_result ;

}
}


#endif // DETECTOR_H
