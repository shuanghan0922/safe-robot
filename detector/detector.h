#ifndef DETECTOR_H
#define DETECTOR_H

#define DEBUG_DETECTOR  //detector调试

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

//按钮颜色
enum BtnColor {
    red = 0, green, blue, other
};
//空开种类
enum AirSwitchType {
    type2,
    type3,
    type211
};
//旋钮识别种类
enum KnobSwitchType {
    twoHand,    //双头
    oneHand      //单头
};

/***********************检测抽象基类***************************/
class AbstractDetector {
public:
    AbstractDetector(Mat inputImg) : m_sourceImg(inputImg) {};
    virtual ~AbstractDetector();    //作为基类要有虚析构函数
    //虚函数 detect()检测器
    //纯虚函数 该类无法被实例化，且继承自它的子类必须override该函数
    virtual void detect() = 0;
    /** @brief  重载源图
    */
    virtual void reloadSourceImg(cv::Mat& img) {
        m_sourceImg = img;
    }
    /** @brief  显示源图
    */
    virtual Mat showSourceImg(bool show = true) const {
        if (show) imshow("sourceImg", m_sourceImg);
        return m_sourceImg;
    }
    /** @brief  显示结果图
    */
    virtual Mat showDstImg(bool show = true) const {
        if (show) imshow("dstImg", m_dstImg);
        return m_dstImg;
    }
    /** @brief  显示过程图
    */
    virtual Mat showMidImg(bool show = true) const {
        if (show) imshow("midImg", m_midImg);
        return m_midImg;
    }
    friend void getPrivateData(AbstractDetector& cellDetector); //该函数用于外部调用内部私有变量
protected:
    Mat m_sourceImg;  //输入的原图
    Mat m_dstImg; //结果图
    Mat m_midImg;  //过程图
};
/***********************圆检测***************************/
class Circle :public AbstractDetector {
public:
    vector<Vec3f> circleInfo;
    Circle(Mat inputImg) : AbstractDetector(inputImg) {

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
/***********************角点检测**************************/
class Corner : public AbstractDetector {
public:
    vector<Point> corners;
    vector<Vec3f> cornersWithDepth;

    Corner(Mat inputImg) : AbstractDetector(inputImg) {
    }
    /** @brief  重写来自基类的纯虚函数 获取角点和深度(已经排序好顺序点)
    */
    virtual void detect() override;
    void printCorners();
    void printCornersWithDistance();
private:
    void sortConrersUseCircle();
};
/***********************空开检测**************************/
class AirSwitch : public AbstractDetector
{
public:
    AirSwitch(Mat inputImg);
    AirSwitch(Mat inputImg, AirSwitchType type);
    ~AirSwitch();
    /** @brief  重写来自基类的纯虚函数
    */
    virtual void detect() override;
    bool isUp();
protected:
    bool m_state;
private:
    AirSwitchType m_type;
    AirSwitch *m_airSwitch;
};

//空开类型1
class _AirSwitch2 : public AirSwitch {   //2个连接在一起
public:
    _AirSwitch2(cv::Mat inputImg) : AirSwitch(inputImg) {};
    virtual void detect() override;
};

//空开类型2
class _AirSwitch3 : public AirSwitch {   //3个连接在一起
public:
    _AirSwitch3(cv::Mat inputImg) : AirSwitch(inputImg) {};
    virtual void detect() override;
};

//空开类型3
class _AirSwitch211 : public AirSwitch {   //2，1，1 连接在一起
public:
    _AirSwitch211(cv::Mat inputImg) : AirSwitch(inputImg) {};
    virtual void detect() override;
};

/***********************按钮检测**************************/
class Btn : public AbstractDetector
{
public:
    //位置
    Point site;
    double mean=0, stdDev=0; //均值和标准差

    Btn(Mat inputImg) : AbstractDetector(inputImg) {
        color = BtnColor::other;
    }
    Btn(Mat inputImg, BtnColor btnColor) : AbstractDetector(inputImg), color(btnColor){
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
    /** @brief  获取矩形位置
    */
    Rect getRect();
    Vec3f getCircles();
    /** @brief  获取图像的均值和标准差
    */
    vector<int> getMeanAndStdDev();
private:
    BtnColor color;
    bool state;
    int THRESHOLD_STATE = 50;
    Vec3f circles{0, 0, 0};
    Rect rect;
    //获取btn中的圆
    bool detectCircles();
};
/***********************旋钮检测**************************/
class KnobSwitch : public AbstractDetector
{
public:
    KnobSwitch(Mat inputImg) : AbstractDetector(inputImg){};
    KnobSwitch(Mat inputImg, KnobSwitchType method);
    /** @brief  重写来自基类的纯虚函数
    */
    virtual void detect() override;
    float getAngle() const {return m_knobSwitch->getAngle();}
protected:
    //位置
    Point m_site;
    float m_angle = 0.0;
private:
    KnobSwitchType m_method;
    KnobSwitch *m_knobSwitch;
};

//双头旋钮
class _KnobSwitchTowHand : public KnobSwitch {
public:
    _KnobSwitchTowHand(cv::Mat inputImg) : KnobSwitch(inputImg){}
    virtual void detect() override;
};
//单头旋钮
class _KnobSwitchOneHand : public KnobSwitch {
public:
    _KnobSwitchOneHand(cv::Mat inputImg) : KnobSwitch(inputImg){}
    virtual void detect() override;
};

/***********************字符识别**************************/
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

