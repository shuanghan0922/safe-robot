#include <iostream>
#include <opencv2/opencv.hpp>
#include <sys/time.h>
#include <unistd.h>

using namespace std;
using namespace cv;

class DetectCircle {
public:
    Mat sourceImg;
    vector<Vec3f> circleInfo;
    DetectCircle(Mat inputImg) : sourceImg(inputImg) {

    }
    void getCircles(double dp = 1, double minDist = 10) {
        HoughCircles(sourceImg, circleInfo, HOUGH_GRADIENT, dp, minDist, 80, 40, 0, 0);
        cout << (circleInfo.size() >= 1 ? "find circles" : "not find circles") << endl;
    }
    void printCircleInfo() {
        for (auto i : circleInfo)
            cout << i << endl;
    }
};

class DetectRect {
public:
    Mat sourceImg;
    vector<Point> corners;
    vector<Vec3f> cornersWithDepth;

    DetectRect(Mat inputImg) : sourceImg(inputImg) {

    }

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
    void getCornersWitdhDepth() {
        goodFeaturesToTrack(sourceImg, corners, 3, 0.1, 10);
        //sort
        sortConrersUseCircle();
//        for (auto point : corners) { //add corners' depth info
//            cornersWithDepth.push_back( Vec3f(point.x, point.y, getPointDepth(point)) );
//        }
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
};


int main()
{
    Mat sourceImg = imread("../detectDiffRect/image/3.png");
    cvtColor(sourceImg, sourceImg, COLOR_BGR2GRAY);
//    dilate(sourceImg, sourceImg, Mat(), Point(-1, -1), 5);
//    imshow("sourceImg", sourceImg);
//    DetectCircle detectCircle(sourceImg);
//    detectCircle.getCircles();
//    detectCircle.printCircleInfo();

    DetectRect detectRect(sourceImg);
    detectRect.getCornersWitdhDepth();
    detectRect.printCorners();

    for (auto i : detectRect.corners) {
        circle(sourceImg, i, 7, Scalar(255));
    }
imshow("sourceImg", sourceImg);
    waitKey(0);
    destroyAllWindows();
    return 0;
}
