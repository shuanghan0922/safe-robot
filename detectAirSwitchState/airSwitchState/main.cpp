#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;



class AirSwitch
{
public:
    Mat sourceImg;
    Point site; //物体定位(待定)

    AirSwitch(Mat inputImg) : sourceImg(inputImg){
    }

    bool isUp()
    {
        vector<Mat> bgrImg(3);
        split(sourceImg, bgrImg);
        diffImg = bgrImg[1] - bgrImg[2];
        threshold(diffImg, diffImg, 10, 255, THRESH_BINARY);
        //闭运算去除噪点一次
        morphologyEx(diffImg, diffImg, CV_MOP_CLOSE, Mat(), Point(-1, -1));
        //识别轮廓
        vector<Mat> contours;
        findContours(diffImg.clone(), contours, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        vector<Rect> contourRects;
        contoursRectImg = diffImg.clone().setTo(0);
        for (size_t i = 0; i < contours.size(); i++) {
            contourRects.push_back(boundingRect(contours[i]));  //最小矩形
            rectangle(contoursRectImg, contourRects[i], Scalar(255));   //画出矩形
        }
        int rectsNum = contourRects.size();
        cout << "have " << rectsNum << " rects." << endl;

        //利用矩形数据判断空气开关状态
        if (rectsNum < 1)
            cout << "无法识别空气开关状态，请退出后调试." << endl;
        else if (rectsNum == 1)
            return true;
        else
            return false;

        return true;
    }
    void showDiffImg() {
        imshow("diffImg", diffImg);
    }
    void showContoursRectImg() {
        imshow("contoursRectImg", contoursRectImg);
    }
private:
    Mat diffImg;
    Mat contoursRectImg;
};


int main()
{
    Mat sourceImg = imread("../../resource/image/airSwitchDown.png");

    AirSwitch airSwitch(sourceImg);
    imshow("airSwitch", airSwitch.sourceImg);
    cout << airSwitch.isUp() << endl;
    airSwitch.showContoursRectImg();

    waitKey(0);
    destroyAllWindows();
    return 0;
}
