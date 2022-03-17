#include <iostream>
#include <opencv2/opencv.hpp>
#include <sys/time.h>
#include <unistd.h>

using namespace std;
using namespace cv;

class DetectRect {
public:
    Mat sourceImg;
    vector<Point> corners;

    DetectRect(Mat inputImg) : sourceImg(inputImg) {

    }
    DetectRect(string fileName) {
        sourceImg = imread(fileName, CAP_MODE_GRAY);
    }
    void showImg() {
        imshow("rectImg", sourceImg);
    }
    void getCorners() {
        goodFeaturesToTrack(sourceImg, corners, 4, 0.1, 10);
        cout << (corners.size() == 0 ? "未检测到角点" : "检测到角点") << endl;
    }

    void printCorners() {
        for (auto i : corners)
            cout << i << endl;
    }
};

class DetectCircles {
public:
    Mat sourceImg;
    vector<Vec3f> circles;

    DetectCircles(Mat inputImg) : sourceImg(inputImg) {

    }
    DetectCircles(string fileName) {
        sourceImg = imread(fileName, CAP_MODE_GRAY);
    }
    void showImg() {
        imshow("circlesImg", sourceImg);
    }
    void getCorners(double dp=1, double minDist=10) {
        HoughCircles(sourceImg, circles, HOUGH_GRADIENT, dp, minDist);
        cout << (circles.size() == 0 ? "未检测到圆" : "检测到圆") << endl;
    }

    void printCorners() {
        for (auto c : circles)
            cout << c << endl;
    }
};

Mat getCircles(Point center, int radius) {

    Mat circleImg(300, 300, CV_8UC1, Scalar(255));
    circle(circleImg, center, radius, Scalar(0));

    return circleImg;
}

int main()
{
    DetectRect detectRect("../detectDiffRect/image/rects/110*110.png");
    detectRect.showImg();
//    detectRect.getCorners();
//    detectRect.printCorners();

    Mat circleImg = getCircles(Point(150, 150), 150);
    DetectCircles detectCircles(circleImg);
    detectCircles.showImg();
//    detectCircles.getCorners();
//    detectCircles.printCorners();


    timeval timeStart, timeEnd;
    gettimeofday(&timeStart, NULL);
    for (int i = 1000; i > 0; i--) {
        detectCircles.getCorners();
    }
    gettimeofday(&timeEnd, NULL);

    float mTime = 1000000 * (timeEnd.tv_sec - timeStart.tv_sec) + timeEnd.tv_usec - timeStart.tv_usec;
    mTime /= 1000000;
    cout << "程序运行时间" << mTime << "秒" << endl;

//    vector<Vec3f> circlesData = detectCircles.circles, result(1);

//    for (size_t i = 0; i < circlesData.size(); i++) {
//        result[0][0] += circlesData[i][0];
//        result[0][1] += circlesData[i][1];
//        result[0][2] += circlesData[i][2];
//    }
//    result[0][0] /= circlesData.size();
//    result[0][1] /= circlesData.size();
//    result[0][2] /= circlesData.size();
//    for (auto i : result) {
//        cout << i << endl;
//    }

    waitKey(0);
    destroyAllWindows();
    return 0;
}
