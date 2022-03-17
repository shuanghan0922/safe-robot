#include "extractGraph.h"


ExtractGraph::ExtractGraph()
{

}

//提取输入原始图片的蓝色层
Mat ExtractGraph::extractBlueLayer(Mat src)
{
    Mat blueImg;
    vector<Mat> BGRImg(3);
    //分割出元件的blue通道
    split(src, BGRImg);
    blueImg = BGRImg[0]-BGRImg[1];
    return blueImg;
}
//提取图片的所有元器件
Mat ExtractGraph::extractComponet(Mat src, double thresh, double maxValue)
{
    Mat blueImg = src.channels() == 1 ? src : extractBlueLayer(src);
    Mat binnaryImg;
    //阈值化提取出想要的图像
    threshold(blueImg, binnaryImg, thresh, maxValue, THRESH_BINARY);
    morphologyEx(binnaryImg, binnaryImg, MORPH_CLOSE, Mat());
//    dilate(binnaryImg, binnaryImg, Mat(), Point(-1, -1), 1);
    return binnaryImg;
}
//提取图片中的文字
Mat ExtractGraph::extractText(Mat sourceImg, vector<Rect>& rectVector,
                                  double thresh, double maxValue)
{
    Mat binnayImg;  //原图像的灰度图
    cvtColor(sourceImg, binnayImg, COLOR_BGR2GRAY);
    Mat componentImg = extractComponet(sourceImg);  //内容物图
    dilate(componentImg, componentImg, Mat(), Point(-1, -1), 2);

    Mat roughImg = binnayImg.clone();   //定义一个粗糙图
    threshold(roughImg, roughImg, thresh, maxValue, THRESH_BINARY_INV);
    Mat testImg = roughImg - componentImg;

    /******************通过识别轮廓，获取所有最小矩形，然后剔除文字矩形**********************/
    vector<vector<Point>> linesAndTextContours;
    findContours(testImg.clone(), linesAndTextContours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    int linesAndTextRectNum = linesAndTextContours.size();
    //检测元器件矩形轮廓
    vector<Rect> linesAndTextTectVector(linesAndTextRectNum);
    double linesAndTextRectAverageArea = 0.0;
    for (int i=0; i<linesAndTextRectNum; i++)
    {
        linesAndTextTectVector[i] = boundingRect(linesAndTextContours[i]);
        linesAndTextTectVector[i] += Size(10,10);
        linesAndTextTectVector[i] += Point(-5,-5);
        linesAndTextRectAverageArea += linesAndTextTectVector[i].area() / linesAndTextRectNum;
    }
    int squareSize = pow(linesAndTextRectAverageArea, 0.5);    //转化为正方形面积
    cout << squareSize << endl;

    //筛选矩形轮廓，剔除掉文字轮廓 通过对所有矩形的平均面积开平方形成正方形
    //剔除掉所有边都小于该长度的矩形
    rectVector.clear();
    Mat linesImg;
    Mat textImg = testImg.clone().setTo(0);
    for (int i=0; i<linesAndTextRectNum; i++)
    {
        if (linesAndTextTectVector[i].width > squareSize || linesAndTextTectVector[i].height > squareSize)
            rectVector.push_back(linesAndTextTectVector[i]);
        else
        {////在原图上剔除文字矩形的图像
            Mat charRectImg = testImg(linesAndTextTectVector[i]);
            charRectImg.copyTo(textImg(linesAndTextTectVector[i])); //文字图
        }
    }

    return textImg;
}
//提取图片的初始线路
Mat ExtractGraph::extractLines(Mat sourceImg, vector<Rect>& linesRect,
                 double thresh, double maxValue)
{
    Mat binnayImg;  //原图像的灰度图
    cvtColor(sourceImg, binnayImg, COLOR_BGR2GRAY);
    Mat componentImg = extractComponet(sourceImg);  //内容物图
    dilate(componentImg, componentImg, Mat(), Point(-1, -1), 2);

    Mat roughImg = binnayImg.clone();   //定义一个粗糙图
    threshold(roughImg, roughImg, thresh, maxValue, THRESH_BINARY_INV);
    Mat testImg = roughImg - componentImg;

    /******************通过识别轮廓，获取所有最小矩形，然后剔除文字矩形**********************/
    vector<vector<Point>> linesAndTextContours;
    findContours(testImg.clone(), linesAndTextContours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    int linesAndTextRectNum = linesAndTextContours.size();
    //检测元器件矩形轮廓
    vector<Rect> linesAndTextTectVector(linesAndTextRectNum);
    double linesAndTextRectAverageArea = 0.0;
    for (int i=0; i<linesAndTextRectNum; i++)
    {
        linesAndTextTectVector[i] = boundingRect(linesAndTextContours[i]);
        linesAndTextTectVector[i] += Size(10,10);
        linesAndTextTectVector[i] += Point(-5,-5);
        linesAndTextRectAverageArea += linesAndTextTectVector[i].area() / linesAndTextRectNum;
    }
    int squareSize = pow(linesAndTextRectAverageArea, 0.5);    //转化为正方形面积
//    cout << squareSize << endl;

    //筛选矩形轮廓，剔除掉文字轮廓 通过对所有矩形的平均面积开平方形成正方形
    //剔除掉所有边都小于该长度的矩形
    linesRect.clear();
    Mat textImg = testImg.clone().setTo(0);
    for (int i=0; i<linesAndTextRectNum; i++)
    {
        if (linesAndTextTectVector[i].width > squareSize || linesAndTextTectVector[i].height > squareSize)
            linesRect.push_back(linesAndTextTectVector[i]);
        else
        {////在原图上剔除文字矩形的图像
            Mat charRectImg = testImg(linesAndTextTectVector[i]);
            charRectImg.copyTo(textImg(linesAndTextTectVector[i])); //文字图
        }
    }
    Mat linesImg = testImg - textImg;  //线路图
    int realRectNum = linesRect.size();

//    //画出检测到的器件矩形轮廓
//    Mat linesAndTextRectImg = testImg.clone().setTo(0);
//    for (int i=0; i<realRectNum; i++)
//    {
//        rectangle(linesAndTextRectImg, linesRectVector[i], Scalar(255));
//    }
//    imshow("rectangleImg", linesAndTextRectImg);

    //同时画出矩形和线路图用于分析
    Mat linesAndRectImg = linesImg.clone();
    //画出检测到的器件矩形轮廓
    for (int i=0; i<realRectNum; i++)
    {
        rectangle(linesAndRectImg, linesRect[i], Scalar(255));
    }

    return linesAndRectImg;
}
//提取图片中的电子器件,并组合成图片返回
Mat ExtractGraph::extractCells(Mat componentImg, vector<Mat>& imgVector)
{
    //获取电子器件的轮廓
    vector<vector<Point>> contours;
    findContours(componentImg, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    int rectNum = contours.size();

    //检测元器件矩形轮廓
    vector<Rect> rectVector(rectNum);
    for (int i=0; i<rectNum; i++)
    {
        rectVector[i] = boundingRect(contours[i]);
        rectVector[i] += Size(10,10);
        rectVector[i] += Point(-5,-5);
    }
//    cout << "检测到的电子器件矩形数量:" << rectVector.size() << endl;

    //画出检测到的器件矩形轮廓
    Mat rectanglesImg = componentImg.clone().setTo(0);
    for (int i=0; i<rectNum; i++)
    {
        rectangle(rectanglesImg, rectVector[i], Scalar(255));
    }
//    imshow("rectangleImg", rectanglesImg);

    //根据矩形轮廓分割出器件图形
    imgVector.clear();
    imgVector.resize(rectNum);
    Mat devImgShow;
    for (int i=0; i<rectNum; i++)
    {
        imgVector[i] = Mat(componentImg, rectVector[i]);
    }
    multipleImage(imgVector, devImgShow, 4, NO_CHANGE);
//    imshow("devImgShow", devImgShow);

    return devImgShow;
}
//提取图片中的器件,并组成图片返回
Mat ExtractGraph::extractCells(Mat componentImg, vector<Rect>& rectVector)
{
    /***********************检测元件的矩形轮廓*****************************/
    //获取电子器件的轮廓
    vector<vector<Point>> contours;
    findContours(componentImg, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    int rectNum = contours.size();

    //检测元器件矩形轮廓
    rectVector.clear();
    rectVector.resize(rectNum);
    for (int i=0; i<rectNum; i++)
    {
        rectVector[i] = boundingRect(contours[i]);
        rectVector[i] += Size(10,10);
        rectVector[i] += Point(-5,-5);
    }
    cout << "检测到的电子器件矩形数量: " << rectVector.size() << endl;

//    //画出检测到的器件矩形轮廓
//    Mat componentRectImg = componentImg.clone().setTo(0);
//    for (int i=0; i<componentRectNum; i++)
//    {
//        rectangle(componentRectImg, componentRectVector[i], Scalar(255));
//    }
//    imshow("componentRectImg", componentRectImg);

    Mat cellsAndRectImg = componentImg.clone();
    for (int i=0; i<rectNum; i++)
    {
        rectangle(cellsAndRectImg, rectVector[i], Scalar(255));
    }
//    imshow("cellsAndRectImg", cellsAndRectImg);

    return cellsAndRectImg;
}
//提取图片中的线路
void ExtractGraph::extractCircultPoints(Mat src, vector<vector<Point>> &circultVector)
{
    //通过检测矩形轮廓绘制直线
    vector<vector<Point>> contours;
    findContours(src.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    Mat showImg = src.clone().setTo(0);
    drawContours(showImg, contours, -1, Scalar(255));
    imshow("srcContoursShowImg", showImg);
    cout << "轮廓数量:" << contours.size() << endl;
    int rectNum = contours.size();

    //检测线路矩形轮廓
    vector<Rect> rectVector(rectNum);
    for (int i=0; i<rectNum; i++)
        rectVector[i] = boundingRect(contours[i]);
    cout << "检测到的矩形数量:" << rectVector.size() << endl;

    //通过矩形轮廓融合直线
    circultVector.resize(rectNum);
    for (int i=0; i<rectNum; i++)
        circultVector[i].resize(2); //重定义大小
    for (int i=0; i<rectNum; i++)
    {
        if (rectVector[i].width > rectVector[i].height)
        {
            //横线
            circultVector[i][0] =
                    Point(rectVector[i].x, rectVector[i].y + rectVector[i].height/2);
            circultVector[i][1] =
                    Point(rectVector[i].x + rectVector[i].width, rectVector[i].y + rectVector[i].height/2);
        }
        else
        {
            //竖线
            circultVector[i][0] =
                    Point(rectVector[i].x + rectVector[i].width/2, rectVector[i].y);
            circultVector[i][1] =
                    Point(rectVector[i].x + rectVector[i].width/2, rectVector[i].y + rectVector[i].height);
        }
    }
    //描绘出直线
    Mat linesImg = src.clone().setTo(0);
    for (int i=0; i<rectNum; i++)
        line(linesImg, circultVector[i][0], circultVector[i][1], Scalar(255));
    imshow("srcLinesImg", linesImg);
}
//提取图片中的线路
Mat ExtractGraph::extractCircultPoints(Mat src)
{
    //通过检测矩形轮廓绘制直线
    vector<vector<Point>> contours;
    findContours(src.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
//    Mat showImg = src.clone().setTo(0);
//    drawContours(showImg, contours, -1, Scalar(255));
//    imshow("srcContoursShowImg", showImg);
//    cout << "轮廓数量:" << contours.size() << endl;
    int rectNum = contours.size();

    //检测线路矩形轮廓
    vector<Rect> rectVector(rectNum);
    for (int i=0; i<rectNum; i++)
        rectVector[i] = boundingRect(contours[i]);
//    cout << "检测到的矩形数量:" << rectVector.size() << endl;

    //通过矩形轮廓融合直线
    vector<vector<Point>> circultVector(rectNum, vector<Point>(2));
    for (int i=0; i<rectNum; i++)
    {
        if (rectVector[i].width > rectVector[i].height)
        {
            //横线
            circultVector[i][0] =
                    Point(rectVector[i].x, rectVector[i].y + rectVector[i].height/2);
            circultVector[i][1] =
                    Point(rectVector[i].x + rectVector[i].width, rectVector[i].y + rectVector[i].height/2);
        }
        else
        {
            //竖线
            circultVector[i][0] =
                    Point(rectVector[i].x + rectVector[i].width/2, rectVector[i].y);
            circultVector[i][1] =
                    Point(rectVector[i].x + rectVector[i].width/2, rectVector[i].y + rectVector[i].height);
        }
    }
    //描绘出直线
    Mat linesImg = src.clone().setTo(0);
    for (int i=0; i<rectNum; i++)
        line(linesImg, circultVector[i][0], circultVector[i][1], Scalar(255));

    return linesImg;
}


//提取图片的角点特征
Mat ExtractGraph::extractKeyPoints(const vector<Mat>& devImgVector,
                                   vector<vector<KeyPoint>>& devKeyPointsVector,
                                   int imgCols,
                                   int mode)
{
    int devNum = devImgVector.size();
    //对提取出的元器件进行SIFT特征提取
    Ptr<SIFT> detector = SIFT::create();
    devKeyPointsVector.clear(); devKeyPointsVector.resize(devNum);  //对输入进行清除
    //提取
    detector->detect(devImgVector, devKeyPointsVector);
    //绘制各图片的关键点图
    vector<Mat> devKeyPointsImgVector(devNum);
    for (int i=0; i<devNum; i++)
    {
        drawKeypoints(devImgVector[i], devKeyPointsVector[i], devKeyPointsImgVector[i],
                      Scalar::all(-1));
    }
    cout << "输入图片特征点提取完成." << endl;
    //绘制模板图的KeyPoints图片
    Mat devKeyPointsImgShow;
    multipleImage(devKeyPointsImgVector, devKeyPointsImgShow, imgCols, mode);

    return devKeyPointsImgShow;
}
//提取图片的角点特征
Mat ExtractGraph::extractKeyPoints(const Mat &templateImg,
                                   vector<KeyPoint> &templateKeyPoints)
{
    templateKeyPoints.clear();
    Ptr<SIFT> detector = SIFT::create();
    detector->detect(templateImg, templateKeyPoints);
    cout << "输入图片特征点提取完成." << endl;

    Mat templateKeyPointsImg;
    drawKeypoints(templateImg, templateKeyPoints, templateKeyPointsImg, Scalar::all(-1));

    return templateKeyPointsImg;
}

//提取特征点描述
void ExtractGraph::extractDescriptor( vector<Mat> &devImgVector,
                                    vector<vector<KeyPoint>> &devKeyPointsVector,
                                    vector<Mat> &devDescriptorVector)
{
    int devNum = devImgVector.size();

    Ptr<SIFT> detector = SIFT::create();
    devDescriptorVector.clear(); devDescriptorVector.resize(devNum);
    detector->compute(devImgVector, devKeyPointsVector, devDescriptorVector);
    cout << "输入图片特征描述提取完成." << endl;
}
//提取特征点描述
void ExtractGraph::extractDescriptor( Mat &templateImg,
                                     vector<KeyPoint> &templateKeyPoints,
                                     Mat &templateDescriptor)
{
    Ptr<SIFT> detector = SIFT::create();

    templateKeyPoints.clear();
    detector->compute(templateImg, templateKeyPoints, templateDescriptor);
}
/************************被测物的信息提取************************************/
UnderTestElectronics::UnderTestElectronics()
{

}
UnderTestElectronics::UnderTestElectronics(Mat srcImg) : sourceImg(srcImg)
{
    binnaryImg = extractBlueLayer(srcImg);  //提取蓝色底层图
    componentImg = extractComponet(binnaryImg); //提取内容图
    cellsShowImg = extractCells(componentImg, cellsImg);  //提取所有元件图片图片
    cellsAndRectShowImg = extractCells(componentImg, cellsRect);  //提取元件的所有矩形
    linesAndRectShowImg = extractLines(srcImg, linesRect); //提取所有线路的矩形
    textsShowImg = extractText(srcImg, textsRect);      //提取所有文字的矩形
    extractKeyPoints(cellsImg, keyPoints);        //提取所有关键点
    extractDescriptor(cellsImg, keyPoints, descriptor);   //提取所有描述
    count = cellsImg.size();
    cout << "待检测图片中找到 " << count << " 个元件." << endl;
}
/************************模板样本的信息提取************************************/
TemplateElectronics::TemplateElectronics()
{

}
TemplateElectronics::TemplateElectronics(FileStorage fKeyPoints,
                                         FileStorage fDescriptor,
                                         String dir)
{
    glob(dir, filesName);
    for (size_t i=0; i<filesName.size(); i++)
    {
        //存储样本库中的图片
        allSourceImg.push_back(imread(filesName[i]));
        allImg.push_back(extractComponet(allSourceImg[i]));
        //获取所有模板图片名称
        allName.push_back( filesName[i].substr(26, filesName[i].size()-26-4) );
        //获取KeyPoints
        vector<KeyPoint> tempKeyPoints;
        fKeyPoints[allName[i]] >> tempKeyPoints;
        keyPoints.push_back(tempKeyPoints);
        //获取Descriptor
        Mat tempDescriptor;
        fDescriptor[allName[i]] >> tempDescriptor;
        descriptor.push_back(tempDescriptor);
    }
    count = allImg.size();
}
TemplateElectronics::TemplateElectronics(String dir)
{
    glob(dir, filesName);
    for (size_t i=0; i<filesName.size(); i++)
    {
        //存储样本库中的图片
        allSourceImg.push_back(imread(filesName[i]));
        allImg.push_back(extractComponet(allSourceImg[i]));
        //获取所有模板图片名称
        allName.push_back( filesName[i].substr(26, filesName[i].size()-26-4) );
    }
    count = allImg.size();
    //扩容KeyPoints和descriptor
    keyPoints.resize(count);
    descriptor.resize(count);
}
/************************模板样本的读取************************************/
TemplateDataRead::TemplateDataRead()
{

}
TemplateDataRead::TemplateDataRead(String keyPointsDir, String descriptorDir)
{
    try
    {
        fKeyPoints.open(keyPointsDir, FileStorage::READ);
        fDescriptor.open(descriptorDir, FileStorage::READ);
    }  catch (Exception e) {
        cout << "未检测到模板库文件" << endl;
    }

    if (fKeyPoints.isOpened() && fDescriptor.isOpened())
    {
        cout << "检测到模板库文件." << endl;
    }
    else
    {
        cout << "未检测到模板库文件，试图创建中....." << endl;
        fKeyPoints.release();
        fDescriptor.release();
        fKeyPoints.open(keyPointsDir, FileStorage::WRITE);    //写库
        fDescriptor.open(descriptorDir, FileStorage::WRITE);
        cout << "创建成功，试图写入数据...." << endl;
        TemplateElectronics templateEle(TEMPLATE_IMG_DIR);   //获取模板库中的名字
        int imgNum = templateEle.count;
        Ptr<SIFT> detector = SIFT::create();
        for (int i=0; i<imgNum; i++)
        {
            detector->detect(templateEle.allImg[i], templateEle.keyPoints[i]);
            fKeyPoints << templateEle.allName[i] << templateEle.keyPoints[i];//写入
            detector->compute(templateEle.allImg[i],
                              templateEle.keyPoints[i],
                              templateEle.descriptor[i]);
            fDescriptor << templateEle.allName[i] << templateEle.descriptor[i];//写入
        }
        cout << "数据写入成功." << endl;
    }
}
