#ifndef DETECTOR_H
#define DETECTOR_H

#define DEBUG_DETECTOR  //detector调试码

#include <iostream>
#include <opencv2/opencv.hpp>
#include <librealsense2/rs.hpp>
#ifndef DEBUG_DETECTOR
#include <ComData.h>
#endif

using namespace std;
using namespace cv;

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
class DetectCircle :public Detector {
public:
    vector<Vec3f> circleInfo;
    DetectCircle(Mat inputImg) : Detector(inputImg) {

    }
    /** @brief  重写来自基类的纯虚函数 霍夫检测圆
    */
    virtual void detect() override;
    //打印所有圆的信息
    void printCircleInfo();
};
//角点检测
class DetectRect : public Detector {
public:
    vector<Point> corners;
    vector<Vec3f> cornersWithDepth;

    DetectRect(Mat inputImg) : Detector(inputImg) {
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
    vector<Vec3f> getSite();
    /** @brief  获取图像的均值和标准差
    */
    vector<int> getMeanAndStdDev();
private:
    BtnColor color;
    bool state;
    int THRESHOLD_STATE = 25;
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

}


#endif // DETECTOR_H
