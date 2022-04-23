#include <iostream>
#include <../../detector/detector.h>

using namespace std;
//using namespace detect;

void detect::getPrivateData(Detector& cellDetector) {
    imshow("Test", cellDetector.binaryImg);
}


int main()
{
    Mat sourceImg = imread("../../resource/image/btn/green1_red0.png");

    imshow("sourceImg", sourceImg);

    detect::Btn detectBtn(sourceImg, detect::BtnColor::green);
    detectBtn.detect();
    detectBtn.isLighted();
    detectBtn.showDstImg();
    detectBtn.showBinaryImg();

//    Mat circleImg = imread("../binaryImg.png");
//    cvtColor(circleImg, circleImg, COLOR_BGR2GRAY);
//    imshow("circleImg", circleImg);
//    vector<Vec3f> outputCircles;
//    HoughCircles(circleImg, outputCircles, HOUGH_GRADIENT, 2, 10, 100, 30, 0, 0);
//    cout << outputCircles.size() << endl;
//    for (auto c : outputCircles)
//        cout << c << endl;


    waitKey(0);
    destroyAllWindows();
    return 0;
}


