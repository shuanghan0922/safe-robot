#include <iostream>
#include "../../detector/detector.h"

using namespace std;
//using namespace detect;

void detect::getPrivateData(Detector& cellDetector) {
    imshow("Test", cellDetector.binaryImg);
}


int main()
{
    Mat sourceImg = imread("../../resource/image/btn/green1.png");


    detect::Btn detectBtn(sourceImg, detect::BtnColor::green);
    detectBtn.detect();
    detectBtn.isLighted();
    detectBtn.showDstImg();


    waitKey(0);
    destroyAllWindows();
    return 0;
}


