#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


#define RED     0
#define GREEN   1
#define BLUE    2

class Btn
{
public:
    Mat sourceImg;
    int color;
    Point site;
    double mean=0, stdDev=0; //均值和标准差
    int THRESHOLD_STATE = 50;

    Btn(Mat inputImg, int btnColor) : sourceImg(inputImg), color(btnColor){

    }

    bool isLighted()
    {
        vector<Mat> bgrImg(3);
        split(sourceImg, bgrImg);
        switch (color)
        {
            case RED: diffImg = bgrImg[2] - bgrImg[0]; break;
            case GREEN: diffImg = bgrImg[1] - bgrImg[2];break;
            case BLUE: ;break;
        }
        //通过计算图片的均方差判断LED亮灭状态
        Mat meanMat, stdDevMat;
        meanStdDev(diffImg, meanMat, stdDevMat);

        mean = meanMat.at<double>(0, 0);
        stdDev = stdDevMat.at<double>(0, 0);

        return (stdDev > THRESHOLD_STATE) ? true : false;
    }
    void showDiffImg() {
        imshow("diffImg", diffImg);
    }
private:
    Mat diffImg;

};


int main()
{
    Mat btnImg = imread("../../resource/image/btnRedLight.png");

    Btn btn(btnImg, RED);
    cout << btn.isLighted() << endl;
    btn.showDiffImg();
    cout << btn.stdDev << endl;


    waitKey(0);
    destroyAllWindows();
    return 0;
}
