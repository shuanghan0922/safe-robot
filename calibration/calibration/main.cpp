#include <iostream>
#include "../../detector/detector.h"
#include "../calibration/calibration/calibration.h"

using namespace std;
using namespace cv;

<<<<<<< Updated upstream


int main()
{
    string dir = "../../resource/image/calibration/pannel/1.jpg";
=======
int main()
{
    string dir = "../../resource/image/calibration/square/2.jpg";
>>>>>>> Stashed changes
    cv::Mat sourceImg = cv::imread(dir);
//    cv::resize(sourceImg, sourceImg, cv::Size(), 0.5, 0.5);

<<<<<<< Updated upstream
    detect::Calibration calib(sourceImg, detect::CalibrationMethod::pannel);
    calib.detect();
    calib.showMidImg();
    calib.showDstImg();
=======
//    CalibrationLine *line = new CalibrationLine(sourceImg);
//    line->detect();
//    line->showSourceImg();
//    line->showDstImg();

    CalibrationSquare *square = new  CalibrationSquare(sourceImg);
    square->detect();
    square->showBinaryImg();
    square->showDstImg();
    square->showDstImg();

//    CalibrationBtn *btn = new CalibrationBtn(sourceImg);
//    btn->detect();
//    btn->showSourceImg();
//    btn->showDstImg();
>>>>>>> Stashed changes


    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}
<<<<<<< Updated upstream

=======
>>>>>>> Stashed changes
