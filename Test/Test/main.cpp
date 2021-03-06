#include <iostream>
#include "../../detector/detector.h"

using namespace std;
//using namespace detect;

void detect::getPrivateData(AbstractDetector& cellDetector) {
    imshow("Test", cellDetector.m_dstImg);
}


int main()
{
    Mat sourceImg = imread("../../resource/image/cell/knobSwitch/knobSwitch1.png");

    detect::KnobSwitch knobSwitch(sourceImg, detect::KnobSwitchType::twoHand);
    knobSwitch.detect();
    knobSwitch.showMidImg();
    knobSwitch.showDstImg();
    cout << knobSwitch.getAngle() << endl;

//    detect::AirSwitch airSwitch(sourceImg, detect::AirSwitchType::type211);
//    airSwitch.detect();
//    airSwitch.showMidImg();
//    airSwitch.showDstImg();


    waitKey(0);
    destroyAllWindows();
    return 0;
}


