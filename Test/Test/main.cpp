#include <iostream>
#include <../../detector/detector.h>

using namespace std;
//using namespace detect;

void detect::getPrivateData(Detector& cellDetector) {
    imshow("Test", cellDetector.binaryImg);
}


int main()
{
    Mat sourceImg = imread("../../resource/image/btn/green0_red1.png");

    imshow("sourceImg", sourceImg);

    detect::Btn detectBtn(sourceImg, detect::BtnColor::red);
    detectBtn.detect();
    detectBtn.isLighted();
    detectBtn.showDstImg();

//    vector<Vec3f> circles = detectBtn.getSite();


//    std::cout << "isLight: " << detectBtn.isLighted() << std::endl;
//    for (auto c : circles)
//        std::cout << c << endl;

    waitKey(0);
    destroyAllWindows();
    return 0;
}


