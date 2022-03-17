#include <iostream>
#include <extractGraph/extractGraph.h>
#include <multipleImage/multipleImage.h>

using namespace std;

#define RED     0
#define GREEN   1
#define BLUE    2

typedef struct Btn
{
    int color;
    Point site;
    Btn(){}
    Btn(int color) : color(color){}
}Btn;

bool btnIsLighted(Mat btnImg, Btn btn);

bool btnIsLighted(Mat btnImg, Btn btn)
{
    vector<Mat> bgrImg(3);
    split(btnImg, bgrImg);
    Mat testImg;
    switch (btn.color)
    {
        case RED: testImg = bgrImg[2] - bgrImg[0]; break;
        case GREEN: testImg = bgrImg[1] - bgrImg[2];break;
        case BLUE: ;break;
    }
    imshow("testImg", testImg);

    return true;
}

int main()
{
    Mat sourceImg = imread("../../resource/image/btnRedLight.png");
//    imshow("sourceImg", sourceImg);
//    Mat sourceHSVImg;
//    cvtColor(sourceImg, sourceHSVImg, COLOR_BGR2HSV);
//    imshow("sourceHSVImg", sourceHSVImg);
//    Scalar hsvDataLower(46, 70, 46), hsvDataUp(130, 255, 255);
//    Mat hsvImg;
//    inRange(sourceHSVImg, hsvDataLower, hsvDataUp, hsvImg);
//    imshow("hsvImg", hsvImg);

//    Btn greenBtn(GREEN);
//    btnIsLighted(sourceImg, greenBtn);

    vector<Mat> bgrImg(3);
    split(sourceImg, bgrImg);
    imshow("r", bgrImg[2]);
    imshow("g", bgrImg[1]);
    imshow("b", bgrImg[0]);
    imshow("testImg", bgrImg[1] - bgrImg[2]);


    waitKey(0);
    destroyAllWindows();
    return 0;
}
