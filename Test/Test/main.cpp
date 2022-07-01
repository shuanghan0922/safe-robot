#include <iostream>
#include "../../detector/detector.h"

using namespace std;
//using namespace detect;

void detect::getPrivateData(AbstractDetector& cellDetector) {
    imshow("Test", cellDetector.m_dstImg);
}


int main()
{
    Mat sourceImg = imread("../../resource/image/cell/knobSwitch/1.png");

    detect::KnobSwitch knobSwitch(sourceImg, detect::KnobSwitchType::oneHand);
    knobSwitch.detect();
    knobSwitch.showMidImg();
    knobSwitch.showDstImg();

//    detect::AirSwitch airSwitch(sourceImg, detect::AirSwitchType::type211);
//    airSwitch.detect();
//    airSwitch.showMidImg();
//    airSwitch.showDstImg();


    waitKey(0);
    destroyAllWindows();
    return 0;
}


