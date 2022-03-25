#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;



class KnobSwitch
{
public:
    Mat sourceImg;
    Point site; //物体定位(待定)


    KnobSwitch(Mat inputImg) : sourceImg(inputImg){
    }

    float getAngle()
    {
        float angle = 0.0;
        cvtColor(sourceImg, binaryImg, CV_BGR2GRAY);
        threshold(binaryImg, binaryImg, 30, 255, THRESH_BINARY_INV);
        morphologyEx(binaryImg, binaryImg, CV_MOP_CLOSE, Mat(), Point(-1, -1));
        //获得轮廓和最小矩形框
        vector<Mat> contours;
        vector<RotatedRect> rects;
        findContours(binaryImg.clone(), contours, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        for (size_t i = 0; i < contours.size(); i++) {
            rects.push_back(minAreaRect(contours[i]));
        }
        cout << rects.size() << endl;

        return angle;
    }
    void showBinaryImg() {
        imshow("binaryImg", binaryImg);
    }
private:
    Mat binaryImg;
    Mat rectsImg;
};


int main()
{
    Mat sourceImg = imread("../../resource/image/knobSwitch.png");

    KnobSwitch knobSwitch(sourceImg);
    imshow("airSwitch", knobSwitch.sourceImg);
    cout << knobSwitch.getAngle() << endl;
    knobSwitch.showBinaryImg();

    waitKey(0);
    destroyAllWindows();
    return 0;
}
