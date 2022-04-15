#include <iostream>
#include <../../detector/detector.h>

using namespace std;
//using namespace detect;

void detect::getPrivateData(CellDetector& cellDetector) {
    imshow("Test", cellDetector.binaryImg);
}


int main()
{
    Mat sourceImg = imread("../../resource/image/knobSwitch/knobSwitch3.png");
////    Mat sourceImg = imread("../../resource/image/knobSwitch/knobSwitch4.png",
////                           CAP_MODE_GRAY);
//    imshow("sourceImg", sourceImg);
//    Mat showImg;
//    //方案1:叠加法
//    sourceImg += sourceImg
//    sourceImg += sourceImg;
//    showImg = sourceImg.clone();
//    //方案2：水漫法
////    Rect* rect = new Rect;
////    floodFill(showImg, Point(sourceImg.rows/2, sourceImg.cols/2),
////              Scalar(0), rect, Scalar(20), Scalar(0), FLOODFILL_FIXED_RANGE);
//    //方案3：分水岭

////    //方案4：mean-shift
////    pyrMeanShiftFiltering(sourceImg, showImg, 3, 40);


//    imshow("sourceImg", sourceImg);
//    imshow("showImg", showImg);


//    //旋钮开关
//    KnobSwitch knobSwitch(sourceImg);
//    knobSwitch.getKnobSwitch();
//    cout << knobSwitch.site << endl;
//    cout << knobSwitch.angle << endl;
//    knobSwitch.showDstImg();
//    knobSwitch.showBinaryImg();
    detect::KnobSwitch knobSwitch(sourceImg);
    knobSwitch.getKnobSwitch();
    getPrivateData(knobSwitch);

    //空气开关
//    AirSwitch airSwitch(sourceImg);
//    std::cout << airSwitch.isUp() << endl;
//    airSwitch.showDstImg();

//    //按钮
//    Btn btn(sourceImg);
//    btn.getBtn();
//    cout << btn.btnState << endl;

    waitKey(0);
    destroyAllWindows();
    return 0;
}


