#include <iostream>
#include <../../detector/detector.h>

using namespace std;



int main()
{
    Mat sourceImg = imread("../../resource/image/btnRedDark.png");
    //旋钮开关
//    KnobSwitch knobSwitch(sourceImg);
//    knobSwitch.showSourceImg();
//    knobSwitch.getKnobSwitch();
//    cout << knobSwitch.site << endl;
//    cout << knobSwitch.angle << endl;
//    knobSwitch.showDstImg();

    //空气开关
//    AirSwitch airSwitch(sourceImg);
//    std::cout << airSwitch.isUp() << endl;
//    airSwitch.showDstImg();

    //按钮
    Btn btn(sourceImg);
    btn.getBtn();
    cout << btn.btnState << endl;

    waitKey(0);
    destroyAllWindows();
    return 0;
}
