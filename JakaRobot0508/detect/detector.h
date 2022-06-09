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
float getPointDepth(Point point);
#endif
namespace detect {

//检测器基类
class Detector {
public:
    //输入的原图
    Mat sourceImg;
    //位置
    Point site;
    Detector(Mat inputImg) : sourceImg(inputImg) {};

    //虚函数 detect()检测器
    //纯虚函数 该类无法被实例化，且继承自它的子类必须override该函数
    virtual void detect() = 0;
    /** @brief  重载源图
    */
    void loadSourceImg(cv::Mat& img) {
        sourceImg = img;
    }
    /** @brief  显示源图
    */
    void showSourceImg() {
        imshow("sourceImg", sourceImg);
    }
    /** @brief  显示结果图
    */
    void showDstImg() {
        imshow("dstImg", dstImg);
    }
    /** @brief  显示二值图
    */
    void showBinaryImg() {
        imshow("binaryImg", binaryImg);
    }
    friend void getPrivateData(Detector& cellDetector); //该函数用于外部调用内部私有变量
protected:
    //结果图
    Mat dstImg;
    Mat binaryImg;  //过程图
};
//圆检测器
class Circle :public Detector {
public:
    vector<Vec3f> circleInfo;
    Circle(Mat inputImg) : Detector(inputImg) {

    }
    /** @brief  重写来自基类的纯虚函数 霍夫检测圆
    */
    virtual void detect() override;
    //设置检测圆参数
    void setDetectParams(int dp = 1, int minDist = 10);
    //打印所有圆的信息
    void printCircleInfo();
private:
    int dp = 1, minDist = 10;
};
//角点检测
class Corner : public Detector {
public:
    vector<Point> corners;
    vector<Vec3f> cornersWithDepth;

    Corner(Mat inputImg) : Detector(inputImg) {
    }
    /** @brief  重写来自基类的纯虚函数 获取角点和深度(已经排序好顺序点)
    */
    virtual void detect() override;
    void printCorners();
    void printCornersWithDistance();
private:
    void sortConrersUseCircle();
};
//空开检测
class AirSwitch : public Detector
{
public:

    AirSwitch(Mat inputImg) : Detector(inputImg){};
    /** @brief  重写来自基类的纯虚函数
    */
    virtual void detect() override;
    bool isUp();
private:
    bool state;
};
//按钮颜色
enum BtnColor {
    red = 0, green, blue, other
};
//按钮检测
class Btn : public Detector
{
public:
    double mean=0, stdDev=0; //均值和标准差

    Btn(Mat inputImg) : Detector(inputImg) {
        color = BtnColor::other;
    }
    Btn(Mat inputImg, BtnColor btnColor) : Detector(inputImg), color(btnColor){
    }
    /** @brief  重写来自基类的纯虚函数
    */
    virtual void detect() override;
    /** @brief  判断btn状态
    */
    bool isLighted();
    /** @brief  获取颜色
    */
    int getColor();
    /** @brief  获取按钮位置
    */
    Point getSite();
    Vec3f getCircles();
    /** @brief  获取图像的均值和标准差
    */
    vector<int> getMeanAndStdDev();
private:
    BtnColor color;
    bool state;
    int THRESHOLD_STATE = 50;
    Vec3f circles{0, 0, 0};
    //获取btn中的圆
    bool detectCircles();
};
//旋钮检测
class KnobSwitch : public Detector
{
public:
    float angle = 0.0;
    Mat knobSwitchTestImg;
    KnobSwitch(Mat inputImg) : Detector(inputImg){
    }
    /** @brief  重写来自基类的纯虚函数
    */
    virtual void detect() override;
    void getKnobSwitch();

};
#ifndef DEBUG_DETECTOR
class TextRecongize{
public:
    int numofText=0;
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
    cv::String detModelPath = "/home/hfut/JakaRobot/TEXT_RECONGIZE/recognition-text/DB_TD500_resnet50.onnx";
    double detScale = 1.0 / 255.0;

    //CRNN文本识别模型参数
    cv::String recModelPath = "/home/hfut/JakaRobot/TEXT_RECONGIZE/recognition-text/crnn_cs_CN.onnx";
    cv::String vocPath = "/home/hfut/JakaRobot/TEXT_RECONGIZE/recognition-text/alphabet_3944.txt";
    int imreadRGB = 1;         //0：以灰度图读取图像   1：以彩色图读取图像
    double recScale = 1.0 / 127.5;

};
#endif
}


#endif // DETECTOR_H

