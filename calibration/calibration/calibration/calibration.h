#ifndef CALIBRATION_H
#define CALIBRATION_H
#include <../../detector/detector.h>

#define HIDDEN __attribute__((visibility("hidden")))
//使用设计模式中的工厂设计模式

//定义一个标定的抽象类
class AbstractCalibration {

public:
    AbstractCalibration();

};


class Calibration : public detect::AbstractDetector {
public:
    Calibration(cv::Mat sourceImg) : AbstractDetector(sourceImg){};
    Calibration(cv::Mat sourceImg, const string &method);

    virtual void detect() override {
        std::cout << "Calibration" << std::endl;
    }
private:
    string m_method;
};

//按钮标定
class CalibrationBtn : public Calibration {

public:
    CalibrationBtn(cv::Mat img) : Calibration(img) {

    }
    virtual void detect() override;
    vector<cv::Point> getPoints() const {
        return point;
    }
private:
    vector<cv::Point> point;
};

//交叉线标定
class CalibrationLine : public Calibration {

public:
    CalibrationLine(cv::Mat img) : Calibration(img) {

    }
    virtual void detect() override;
    void showAllCornerImg() const {
        cv::imshow("line-allCornerImg", allCornerImg);
    }
private:
    cv::Mat allCornerImg;
};

//矩形框标定
class CalibrationSquare : public Calibration {

public:
    CalibrationSquare(cv::Mat img) : Calibration(img) {

    }
    virtual void detect() override;
    vector<cv::Point> getPoints() const {
        return point;
    }
private:
    vector<cv::Point> point;
};


////标定方法
//class CalibrationMethod {

//private:
//    CalibrationMethod(){};
//public:
//    static Calibration getMethodObject(const string &method);
//};


#endif // CALIBRATION_H
