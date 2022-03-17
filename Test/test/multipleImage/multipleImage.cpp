#include "multipleImage.h"

using namespace cv;
using namespace std;

#define NO_CHANGE 1
#define CHANGE 0

void multipleImage(vector<Mat> imgVector,
                   Mat& dst,
                   int imgCols,
                   int mode,
                   int pixel)  //参数为列
{
    if (mode == CHANGE)
    {   //如果参数选择改变，则预先分配每个图片300*300的空间
        //对每个图片进行比较,如果最大边大于300,则进行缩放, 否则保持原状
        Size imgStdSize(pixel, pixel);  //标准300*300通道
        int imgNum = imgVector.size();
        Mat imgStd; //标准图片
        Point2i location(0, 0); //坐标点(从0,0开始)
        //构建窗口大小 通道与imageVector[0]的通道一样
        Mat imgWindow(imgStdSize.height*((imgNum-1)/imgCols + 1), imgStdSize.width*imgCols, imgVector[0].type());
        for (int i=0; i<imgNum; i++)
        {
            location.x = (i%imgCols)*imgStdSize.width;
            location.y = (i/imgCols)*imgStdSize.height;

            int MAX_PIXEL=pixel;
            //选择图片最大的一边 将最大的边按比例变为300像素
            Size imgOriSize = imgVector[i].size();
            int imgMaxPixel = max(imgOriSize.height, imgOriSize.width);
            //获取最大像素变为MAX_PIXEL的比例因子
            double prop = imgMaxPixel < MAX_PIXEL ?  1 : MAX_PIXEL/(double)imgMaxPixel;
            cout << prop << endl;

            resize(imgVector[i], imgStd, Size(0, 0), prop, prop, INTER_LINEAR); //设置为标准大小
//            imgStd.copyTo( imgWindow( Rect(location, imgStdSize) ) );
            imgStd.copyTo( imgWindow( Rect(location, Size(imgStd.cols, imgStd.rows)) ));
        }
        dst = imgWindow;
    }
    else if(mode == NO_CHANGE)
    {
        int imgNum = imgVector.size();
        Size imgStdSize(pixel, pixel); //窗口显示的标准图像的Size
        Point2i location(0, 0); //坐标点(从0,0开始)
        //构建窗口大小 通道与imageVector[0]的通道一样
        Mat imgWindow(imgStdSize.height*((imgNum-1)/imgCols + 1), imgStdSize.width*imgCols, imgVector[0].type(), Scalar(100));
        for (int i=0; i<imgNum; i++)
        {
            location.x = (i%imgCols)*imgStdSize.width;
            location.y = (i/imgCols)*imgStdSize.height;
            //resize(imgVector[i], imgVector[i], imgStdSize); //设置为标准大小
            imgVector[i].copyTo( imgWindow( Rect(location, Size(imgVector[i].cols, imgVector[i].rows)) ));
        }
        dst = imgWindow;
    }
}

void multipleImage(vector<Mat> imgVector, Mat& dst, int imgCols, double ratio)  //参数为列
{
    int imgNum = imgVector.size();  //图片数
    //将图片按照比例因子缩放
    Size imgOriSize = imgVector[0].size();
    Size imgStdSize(imgOriSize.width * ratio, imgOriSize.height * ratio); //窗口显示的标准图像的Size

    Mat imgStd; //标准图片
    Point2i location(0, 0); //坐标点(从0,0开始)
    //构建窗口大小 通道与imageVector[0]的通道一样
    Mat imgWindow(imgStdSize.height*((imgNum-1)/imgCols + 1), imgStdSize.width*imgCols, imgVector[0].type());
    for (int i=0; i<imgNum; i++)
    {
        location.x = (i%imgCols)*imgStdSize.width;
        location.y = (i/imgCols)*imgStdSize.height;
        resize(imgVector[i], imgStd, imgStdSize, ratio, ratio, INTER_LINEAR); //设置为标准大小
        imgStd.copyTo( imgWindow( Rect(location, imgStdSize) ) );
    }
    dst = imgWindow;
}

void ManyImages(vector<Mat> Images, Mat& dst, int imgRows)  //行
{
    int Num = Images.size();//得到Vector容器中图片个数
    //设定包含这些图片的窗口大小，这里都是BGR3通道，如果做灰度单通道，稍微改一下下面这行代码就可以
    Mat Window(300 * ((Num - 1) / imgRows + 1), 300 * imgRows, CV_8UC3, Scalar(0, 0, 0));

    Mat Std_Image;//存放标准大小的图片
    Mat imageROI;//图片放置区域
    Size Std_Size = Size(300, 300);//每个图片显示大小300*300
    int x_Begin = 0;
    int y_Begin = 0;
    for (int i = 0; i < Num; i++)
    {
        x_Begin = (i % imgRows)*Std_Size.width;//每张图片起始坐标
        y_Begin = (i / imgRows)*Std_Size.height;
        resize(Images[i], Std_Image, Std_Size, 0, 0, INTER_LINEAR);//将图像设为标准大小
        //将其贴在Window上
        imageROI = Window(Rect(x_Begin, y_Begin, Std_Size.width, Std_Size.height));
        Std_Image.copyTo(imageROI);
    }
    dst = Window;
}

