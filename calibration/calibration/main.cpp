#include <iostream>
#include "../../detector/detector.h"
#include "../calibration/calibration/calibration.h"

using namespace std;
using namespace cv;



int main()
{
    string dir = "../../resource/image/calibration/pannel/1.jpg";
    cv::Mat sourceImg = cv::imread(dir);

    detect::Calibration calib(sourceImg, detect::CalibrationMethod::pannel);
    calib.detect();
    calib.showMidImg();
    calib.showDstImg();


    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}

