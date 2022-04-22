#include <iostream>
#include <../../detector/detector.h>

using namespace std;
//using namespace detect;

void detect::getPrivateData(CellDetector& cellDetector) {
    imshow("Test", cellDetector.binaryImg);
}


int main()
{
    Mat sourceImg = imread("./image/img1.png");

    imshow("sourceImg", sourceImg);

    detect::Btn detectBtn(sourceImg, detect::BtnColor::green);
    detectBtn.isLighted();
    detectBtn.showDstImg();

    vector<Vec3f> circles = detectBtn.getBtnSite();

    std::cout << "stdDev" << detectBtn.stdDev << std::endl;
    std::cout << "btnColor: " << detectBtn.color << std::endl;
    std::cout << "isLight: " << detectBtn.isLighted() << std::endl;
    for (auto c : circles)
        std::cout << c << endl;

    waitKey(0);
    destroyAllWindows();
    return 0;
}


