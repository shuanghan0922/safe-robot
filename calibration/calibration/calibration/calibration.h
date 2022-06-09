#ifndef CALIBRATION_H
#define CALIBRATION_H
#include <../../detector/detector.h>

//使用
//定位方法选择

class Calibration
{
public:
    Calibration();
};

//按钮法标定
class MethodBtn {
public:
    MethodBtn();

};

//线法标定
class MethodLine {
public:
    MethodLine();

};

//矩形法标定
class MethodSquare {
public:
    MethodSquare();
};

#endif // CALIBRATION_H
