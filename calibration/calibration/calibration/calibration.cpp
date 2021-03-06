#include "calibration.h"

namespace detect {

Calibration::Calibration(Mat sourceImg) :
    AbstractDetector(sourceImg),
    AbstractCalibration() {

    m_calibration = nullptr;
}

Calibration::Calibration(Mat sourceImg, CalibrationMethod method) :
    AbstractDetector(sourceImg),
    AbstractCalibration(),
    m_method(method) {

    switch (m_method) {
    case btn:
        m_calibration = new _CalibrationBtn(sourceImg);
        break;
    case line:
        m_calibration = new _CalibrationLine(sourceImg);
        break;
    case pannel:
        m_calibration = new _CalibrationPannel(sourceImg);
        break;
    default:
        std::cout << "输入正确的标定方法" << std::endl;
        break;
    }
}

Calibration::~Calibration()
{
    delete m_calibration;
}

void Calibration::detect()
{
    m_calibration->detect();
    this->m_dstImg = m_calibration->m_dstImg;
    this->m_midImg = m_calibration->m_midImg;
}

void _CalibrationBtn::detect()
{
    cv::Mat btnImg = m_sourceImg.clone();
    detect::Btn *btn = new detect::Btn(m_sourceImg, detect::BtnColor::green);

    btn->detect();
    m_points.push_back(btn->getSite());   //添加第一个点
    cv::Mat greenImg = btnImg.clone().setTo(0);
    btnImg(btn->getRect()).setTo(0).copyTo(btnImg(btn->getRect()));
    //重载图片，涂黑后再次检测
    btn->reloadSourceImg(btnImg);
    btn->detect();
    m_points.push_back(btn->getSite());    //添加第二个点
    btnImg(btn->getRect()).setTo(0).copyTo(btnImg(btn->getRect()));
    m_midImg = btnImg.clone();

    //红色
    delete btn;
    btn = new detect::Btn(btnImg, detect::BtnColor::red);
    btn->detect();
    m_points.push_back(btn->getSite());   //添加第一个点
    greenImg = btnImg.clone().setTo(0);
    btnImg(btn->getRect()).setTo(0).copyTo(btnImg(btn->getRect()));
    //重载图片，涂黑后再次检测
    btn->reloadSourceImg(btnImg);
    btn->detect();
    m_points.push_back(btn->getSite());    //添加第二个点
    btnImg(btn->getRect()).setTo(0).copyTo(btnImg(btn->getRect()));

    m_dstImg = btnImg.clone();
    for (auto p : m_points)
        std::cout << p << std::endl;
}

void _CalibrationLine::detect()
{
    //取出黄色line
    vector<cv::Mat> srcLineImg;
    cv::split(m_sourceImg, srcLineImg);
    cv::Mat greenBtn = srcLineImg[1] - srcLineImg[2];
    cv::Mat redBtn = srcLineImg[2] - srcLineImg[1];
    cv::Mat lineImg = srcLineImg[2]- srcLineImg[0] - redBtn - greenBtn;
    m_midImg = lineImg.clone();
    //查找角点
    vector<Point> corners;
    goodFeaturesToTrack(lineImg, corners, 12, 0.1, 10);
    cout << "查找到:" << corners.size() << " 个角点" << endl;
    for (auto p : corners) {
        circle(m_midImg, p, 5, Scalar(255), 2);
    }
    //排除掉最下面的4个角点
    m_dstImg = lineImg.clone();
    sort(corners.begin(), corners.end(), [](Point x, Point y){
        return (x.y < y.y);
    });
    for (int i = 4; i > 0; i--)
        corners.pop_back();     //删除末尾四个元素
    for (auto p : corners) {
        circle(m_dstImg, p, 5, Scalar(255), 2);
    }
    //可根据四个点的信息进行校正
    for (auto p : corners)
        cout << p << endl;
//    imshow("mid", midImg);
}

void _CalibrationPannel::detect()
{
    cv::Mat squareImg = m_sourceImg.clone();
    cv::cvtColor(m_sourceImg, squareImg, COLOR_BGR2GRAY);
    //预处理
    //反阈值
    threshold(squareImg, squareImg, 90, 255, THRESH_BINARY_INV);
    morphologyEx(squareImg, squareImg, CV_MOP_CLOSE, Mat());    //闭操作
    m_midImg = squareImg.clone();
    //轮廓识别 + 最大矩形排序
    vector<Mat> contours;
    findContours(squareImg.clone(), contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
    vector<Rect> contourRects;
    cv::Mat contoursImg = squareImg.clone().setTo(0);
    for (size_t i = 0; i < contours.size(); i++) {
        contourRects.push_back(boundingRect(contours[i]));  //最小矩形
        rectangle(contoursImg, contourRects[i], Scalar(255));   //画出矩形
    }
    sort(contourRects.begin(), contourRects.end(), [](cv::Rect x, cv::Rect y){
        return x.area() > y.area();
    }); //按照矩形面积从大到小进行排序
    //去掉除了最大轮廓外的所有杂图
    cv::Mat uselessImg = squareImg.clone().setTo(0);
    for (size_t i = 2; i < contourRects.size(); i++) {
        squareImg(contourRects[i]).copyTo(uselessImg(contourRects[i]));
    }
    squareImg -= uselessImg;
//    imshow("dstImg", squareImg);     //最大的矩形框
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

    cv::Mat cornerImg = squareImg.clone().setTo(0);
    for(auto r : fourCorners) {
        squareImg(r).copyTo(cornerImg(r));
    }

    goodFeaturesToTrack(squareImg, m_points, 4, 0.5, 10);
    cout << "查找到:" << m_points.size() << " 个角点" << endl;
    for (auto p : m_points) {
        circle(squareImg, p, 5, Scalar(255), 2);
        std::cout << p << std::endl;
    }
    m_dstImg = squareImg.clone();
}


}

