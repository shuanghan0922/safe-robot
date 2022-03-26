#include <iostream>
#include <../../detector/detector.h>

using namespace std;

int main()
{
    Mat sourceImg = imread("../../resource/image/btnGreenLight.png");
    Btn greenBtn(sourceImg, GREEN);
    cout << greenBtn.isLighted() << endl;
    greenBtn.showDstImg();

    waitKey(0);
    destroyAllWindows();
    return 0;
}
