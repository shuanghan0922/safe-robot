#ifndef EXTRACTGRAPH_H
#define EXTRACTGRAPH_H

#include <iostream>
#include <vector>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "../multipleImage/multipleImage.h"
#include <opencv2/xfeatures2d/nonfree.hpp>

using namespace xfeatures2d;
using namespace cv;
using namespace std;

#define TEMPLATE_IMG_DIR "../../resource/image/base/*.jpg"
#define TEMPLATE_KEYPOINTS_DIR "../extract/data/keyPoints.yaml"
#define TEMPLATE_DESCRIPTOR_DIR "../extract/data/descriptor.yaml"

class ExtractGraph
{
public:

    vector<Mat> processImg;

    ExtractGraph();

    Mat extractBlueLayer(Mat src);
    Mat extractComponet(Mat  src, double thresh=200, double maxValue=255);
    //提取文字
    Mat extractText(Mat sourceImg, vector<Rect>& textsRect,
                    double thresh=80, double maxValue=255);
    //提取元件连接线路
    Mat extractLines(Mat sourceImg, vector<Rect>& linesRect,
                     double thresh=80, double maxValue=255);
    //提取元件
    Mat extractCells(Mat componentImg, vector<Mat>& imgVector);
    Mat extractCells(Mat componentImg, vector<Rect>& rectVector);

    //提取元件周围的线路
    void extractCircultPoints(Mat src, vector<vector<Point>>& circultVector);
    Mat extractCircultPoints(Mat src);

    //提取特征
    Mat extractKeyPoints(const vector<Mat>& devImgVector,
                         vector<vector<KeyPoint>>& devKeyPointsVector,
                         int imgCols = 4,
                         int mode = NO_CHANGE);
    Mat extractKeyPoints(const Mat& templateImg,
                         vector<KeyPoint>& templateKeyPoints);

    //特征点描述
    void extractDescriptor( vector<Mat>& devImgVector,
                          vector<vector<KeyPoint>>& devKeyPointsVector,
                          vector<Mat>& devDescriptorVector);
    void extractDescriptor( Mat& templateImg,
                          vector<KeyPoint>& templateKeyPoints,
                          Mat& templateDescriptor);
};

//单个电子元器件类
class SingleElectron
{
public:
    Mat img;
    Rect rect;
    vector<KeyPoint> keyPoints;
    Mat descriptor;
};

//多个电子元器件类型 继承ExtractGraph的方法
class UnderTestElectronics : ExtractGraph
{
private:
    Mat sourceImg;
    Mat componentImg;
    Mat binnaryImg;
    Mat cellsShowImg;
    Mat textsShowImg;
    Mat linesShowImg;
    Mat linesAndRectShowImg;
    Mat cellsAndRectShowImg;

public:
    vector<Mat> cellsImg;
    vector<vector<KeyPoint>> keyPoints;
    vector<Mat> descriptor;
    //内容物矩阵
    vector<Rect> linesRect; //线路
    vector<Rect> textsRect; //文字
    vector<Rect> cellsRect; //元件
    int count;

    UnderTestElectronics();
    UnderTestElectronics(Mat srcImg);

    void sourceImgShow()
    {
        imshow("sourceImg", sourceImg);
    }
    void componentImgShow()
    {
        imshow("componentImg", componentImg);
    }
    void showCellsImg()
    {
        imshow("allImg", cellsShowImg);
    }
    void showTextsImg()
    {
        imshow("textsImg", textsShowImg);
    }
    void showLinesImg()
    {
//        imshow("linesImg", linesShowImg);
    }
    void showLinesAndRectImg()
    {
        imshow("linesAndRectImg", linesAndRectShowImg);
    }
    void showCellAndRectImg()
    {
        imshow("cellsAndRectImg", cellsAndRectShowImg);
    }

};

//电子器件模板类 从库中读取数据
class TemplateElectronics : ExtractGraph
{
private:
    vector<String> filesName;
    vector<Mat> allSourceImg;

public:
    vector<String> allName;
    vector<Mat> allImg;
    vector<vector<KeyPoint>> keyPoints;
    vector<Mat>descriptor;
    int count;

    TemplateElectronics();

    //构造函数 读取库中的所有名字
    TemplateElectronics(FileStorage fKeyPoints, FileStorage fDescriptor, String dir);
    //构造函数 只读取模板库中的名字
    TemplateElectronics(String dir);

    void allNameShow()
    {
        cout << "样本库有 " << allName.size() << " 个样本." << endl;
        for (auto s : allName)
            cout << s << endl;
    }

    void allImgShow()
    {
        Mat showImg;
        multipleImage(allImg, showImg, 5, CHANGE, 300);
        imshow("allTemplateImg", showImg);
//        for (size_t i=0; i<allImg.size(); i++)
//        {
//            imshow("Test" + to_string(i), allImg[i]);
//        }
    }
};

class TemplateDataRead
{
public:
    FileStorage fKeyPoints;
    FileStorage fDescriptor;

    TemplateDataRead();
    TemplateDataRead(String keyPointsDir, String descriptorDir);

    ~TemplateDataRead()
    {
        fKeyPoints.release();
        fDescriptor.release();
    }
};


#endif // EXTRACTGRAPH_H
