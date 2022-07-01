#ifndef CALIBRATION_H
#define CALIBRATION_H
#include <../../detector/detector.h>

#define HIDDEN __attribute__((visibility("hidden")))
//使用设计模式中的工厂设计模式

namespace detect {

enum CalibrationMethod {
    btn,    //按钮标定法
    line,   //线标定法
    pannel  //仪表盘标定法
};

//定义一个标定的抽象类
class AbstractCalibration {

public:
    AbstractCalibration() {}
    virtual ~AbstractCalibration() {}

protected:
    vector<cv::Point> m_points;

};


class Calibration : public detect::AbstractDetector, public AbstractCalibration {

public:
    Calibration(cv::Mat sourceImg);
    Calibration(cv::Mat sourceImg, CalibrationMethod method);
    ~Calibration();

    virtual void detect() override;

private:
    CalibrationMethod m_method;
    Calibration *m_calibration;
};

//按钮标定
class _CalibrationBtn final : public Calibration {

public:
    _CalibrationBtn(cv::Mat intputImg) : Calibration(intputImg) {

    }
    virtual void detect() override;
    vector<cv::Point> getPoints() const {
        return m_points;
    }
};

//交叉线标定
class _CalibrationLine final : public Calibration {

public:
    _CalibrationLine(cv::Mat img) : Calibration(img) {

    }
    virtual void detect() override;
};

//矩形框标定
class _CalibrationPannel final : public Calibration {

public:
    _CalibrationPannel(cv::Mat img) : Calibration(img) {

    }
    virtual void detect() override;
    vector<cv::Point> getPoints() const {
        return m_points;
    }
};


////标定方法
//class CalibrationMethod {

//private:
//    CalibrationMethod(){};
//public:
//    static Calibration getMethodObject(const string &method);
//};
}



#endif // CALIBRATION_H
