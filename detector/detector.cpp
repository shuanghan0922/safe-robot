#include <../../detector/detector.h>


#ifndef DEBUG_DETECTOR
#include <ComData.h>
#endif

//类
namespace detect {

///////////////////////圆检测类///////////////////////
void DetectCircle::detect() {
    int dp = 1, minDist = 10;
    HoughCircles(sourceImg, circleInfo, HOUGH_GRADIENT, dp, minDist, 80, 40, 0, 0);
    cout << (circleInfo.size() >= 1 ? "find circles" : "not find circles") << endl;
}
void DetectCircle::printCircleInfo()  {
    for (auto i : circleInfo)
        cout << i << endl;
}

///////////////////////角点检测///////////////////////
void DetectRect::detect() {
    goodFeaturesToTrack(sourceImg, corners, 3, 0.1, 10);
    //sort
    sortConrersUseCircle();
    for (auto point : corners) { //add corners' depth info
#ifndef DEBUG_DETECTOR
        cornersWithDepth.push_back( Vec3f(point.x, point.y, getPointDepth(point)) );
#endif
    }
}
void DetectRect::printCorners() {
    for (auto i : corners)
        cout << i << endl;
}
void DetectRect::printCornersWithDistance() {
    cout << "find " << corners.size() << " points" << endl;
    for (auto i : cornersWithDepth)
        cout << i << endl;
}
void DetectRect::sortConrersUseCircle() {
    vector<Point> sortedCorner(3, Point(0, 0)); //maxDistance:0
    DetectCircle detectCircle(sourceImg);
    detectCircle.detect();
    cout << "we have " << detectCircle.circleInfo.size() << " circles." << endl;
    detectCircle.printCircleInfo();

    //core
    Vec3f circlePoint;
    //if we have many circle, we get average
    for (auto circle : detectCircle.circleInfo) {
       circlePoint += circle;
    }
    circlePoint[0] /= detectCircle.circleInfo.size();
    circlePoint[1] /= detectCircle.circleInfo.size();
    circlePoint[2] /= detectCircle.circleInfo.size();
    //get max distance of corner to circle center
    float maxDistance = 0;
    int maxNo = 0;
    for (int i = 0; i < 3; i++) {
       double distance = pow(corners[i].x - circlePoint[0], 2) + pow(corners[i].y - circlePoint[1], 2);
       if (distance > maxDistance) {
           maxDistance = distance;
           maxNo = i;
       }
    }
    cout << "maxDistance: " << maxDistance << ". maxNo: " << maxNo << endl;
    sortedCorner[0] = corners[maxNo];   //maxDistance
    switch (maxNo) {
       case 0 : {
           if (corners[1].x > corners[2].x) {
               sortedCorner[1] = corners[1];
               sortedCorner[2] = corners[2];
           }
           else {
               sortedCorner[1] = corners[2];
               sortedCorner[2] = corners[1];
           }
           break;
       }
       case 1 : {
           if (corners[0].x > corners[2].x) {
               sortedCorner[1] = corners[0];
               sortedCorner[2] = corners[2];
           }
           else {
               sortedCorner[1] = corners[2];
               sortedCorner[2] = corners[0];
           }
           break;
       }
       case 2 : {
           if (corners[0].x > corners[1].x) {
               sortedCorner[1] = corners[0];
               sortedCorner[2] = corners[1];
           }
           else {
               sortedCorner[1] = corners[1];
               sortedCorner[2] = corners[0];
           }
           break;
       }
    }
    corners = sortedCorner; //sorted corners
}

///////////////////////空开检测///////////////////////
void AirSwitch::detect() {
    vector<Mat> bgrImg(3);
    split(sourceImg, bgrImg);
    binaryImg = bgrImg[1] - bgrImg[2];
    threshold(binaryImg, binaryImg, 10, 255, THRESH_BINARY);
    //闭运算去除噪点一次
    morphologyEx(binaryImg, binaryImg, MORPH_CLOSE, Mat(), Point(-1, -1));
    //识别轮廓
    vector<Mat> contours;
    findContours(binaryImg.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    vector<Rect> contourRects;
    dstImg = sourceImg; //目标图
    for (size_t i = 0; i < contours.size(); i++) {
        contourRects.push_back(boundingRect(contours[i]));  //最小矩形
        rectangle(dstImg, contourRects[i], Scalar(255));   //画出矩形
    }
    int rectsNum = contourRects.size();
    cout << "have " << rectsNum << " rects." << endl;

    //利用矩形数据判断空气开关状态
    if (rectsNum < 1)
        cout << "无法识别空气开关状态，请退出后调试." << endl;
    else if (rectsNum == 1)
        state =  true;
    else
        state =  false;
}

bool AirSwitch::isUp() {
   return state;
}

///////////////////////按钮检测///////////////////////
void Btn::detect() {
    //颜色三通道分割
    vector<Mat> bgrImg(3);
    split(sourceImg, bgrImg);
    //按钮的颜色没有被传参, 先获得按钮颜色
    if (color == BtnColor::other) {
        //先列举每种颜色
        Mat redImg, greenImg, blueImg;
        redImg = bgrImg[2] - bgrImg[0];
        greenImg = bgrImg[1] - bgrImg[2];
        //判断按钮是什么颜色的
        Mat meanMatRed, meanMatGreen, stdDevMatRed, stdDevMatGreen;
        meanStdDev(redImg, meanMatRed, stdDevMatRed);
        meanStdDev(greenImg, meanMatGreen, stdDevMatGreen);
        double stdDevRed = meanMatRed.at<double>(0, 0);
        double stdDevGreen = meanMatGreen.at<double>(0, 0);

        if (stdDevGreen > stdDevRed) {
            color = BtnColor::green;
            dstImg = greenImg;
            state = stdDevGreen > THRESHOLD_STATE ? true : false;
        }
        else {
            color = BtnColor::red;
            dstImg = redImg;
            state = stdDevRed > THRESHOLD_STATE ? true : false;
        }
    }
    else {   //指定了按钮颜色
        switch (color)
        {
            case red: dstImg = bgrImg[2] - bgrImg[0]; break;
            case green: dstImg = bgrImg[1] - bgrImg[2];break;
            case blue: ;break;
        }
        //通过计算图片的均方差判断LED亮灭状态
        Mat meanMat, stdDevMat;
        meanStdDev(dstImg, meanMat, stdDevMat);

        mean = meanMat.at<double>(0, 0);
        stdDev = stdDevMat.at<double>(0, 0);
//        std::cout << stdDev << std::endl;

        state =  (stdDev > THRESHOLD_STATE) ? true : false;
    }
}

bool Btn::isLighted() {
    state ? cout << "light" << endl : cout << "Dark" << endl;
    return state;
}
int Btn::getColor() {
    switch (color) {
        case BtnColor::blue :
            std::cout << "blue" << std::endl; break;
        case BtnColor::green :
            std::cout << "green" << std::endl; break;
        case BtnColor::red :
            std::cout << "red" << std::endl; break;
        case BtnColor::other :
            std::cout << "other" << std::endl; break;
    }
    return color;
}
vector<Vec3f> Btn::getSite() {
    DetectCircle detectCircle(dstImg);
    detectCircle.detect();

    return detectCircle.circleInfo;
}
vector<int> Btn::getMeanAndStdDev() {
    std::cout << "均值: " << mean << "标准差: " << stdDev << std::endl;

    return vector<int>(mean, stdDev);
}

///////////////////////旋钮检测///////////////////////
void KnobSwitch::detect() {

}
void KnobSwitch::getKnobSwitch() {
    //方案1：叠加法
    Mat overlayImg = sourceImg;
    for (int i = 2; i > 0; i--)
        overlayImg += overlayImg;
    cvtColor(overlayImg, binaryImg, COLOR_BGR2GRAY);
    //方案2：mean-shift

    //获得轮廓和最小矩形框
    vector<Mat> contours;
    Rect maxRect;
    int maxContour = 0;
    findContours(binaryImg.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    //获取最大矩形
    for (size_t i = 0; i < contours.size(); i++) {
       Rect tempRect = boundingRect(contours[i]);
       if (i == 0) {
           maxRect = tempRect;
           maxContour = 0;
       }
       else {
           if (tempRect.area() > maxRect.area()) {
               maxRect = tempRect;
               maxContour = i;
           }
       }
    }
    dstImg = sourceImg.clone();
    //获得最小矩形(可斜着)
    RotatedRect rotatedRect = minAreaRect(contours[maxContour]);
    Point2i center(rotatedRect.center);
    circle(dstImg, center, 5, Scalar(255), 2);
    knobSwitchTestImg = dstImg;
    site = center;  //位置
    angle = rotatedRect.angle;
}

}


#ifndef DEBUG_DETECTOR
float getPointDepth(Point point) {
//相机设置
   rs2::colorizer color_map;
   rs2::align align_to(RS2_STREAM_COLOR);
   //rs2::pipeline pipe1;
   //pipe1.start();
   rs2::spatial_filter spat_filter;
   rs2::threshold_filter thr_filter;
   rs2::temporal_filter tem_filter;
   rs2::hole_filling_filter hole_filter;
   rs2::disparity_transform depth_to_disparity(true);
   rs2::disparity_transform disparity_to_depth(false);
   thr_filter.set_option(RS2_OPTION_MIN_DISTANCE, 0.0f);
   thr_filter.set_option(RS2_OPTION_MAX_DISTANCE, 4.f);
   spat_filter.set_option(RS2_OPTION_FILTER_SMOOTH_ALPHA, 0.55f);
   spat_filter.set_option(RS2_OPTION_FILTER_SMOOTH_DELTA, 20);
   spat_filter.set_option(RS2_OPTION_FILTER_MAGNITUDE, 2);
   spat_filter.set_option(RS2_OPTION_HOLES_FILL, 4);
   tem_filter.set_option(RS2_OPTION_FILTER_SMOOTH_DELTA, 4);
   tem_filter.set_option(RS2_OPTION_FILTER_SMOOTH_ALPHA, 0.4f);
   tem_filter.set_option(RS2_OPTION_FILTER_SMOOTH_DELTA, 20);
   //信息配置
   rs2::frameset data= pipe1.wait_for_frames();
   rs2::frameset aligned_set = align_to.process(data);
   rs2::depth_frame depth_frame = aligned_set.get_depth_frame();
   rs2::depth_frame filtered = depth_frame;
   filtered = thr_filter.process(filtered);
   filtered = depth_to_disparity.process(filtered);
   filtered = hole_filter.process(filtered);
   filtered = disparity_to_depth.process(filtered);
   filtered = spat_filter.process(filtered);
   filtered = tem_filter.process(filtered);
   //提取深度信息  //需要对距离检测进行像素上的修正
   vector<float> distacnceVector(5);
   distacnceVector[0] = filtered.get_distance(cvRound(point.x), cvRound(point.y));
   distacnceVector[1] = filtered.get_distance(cvRound(point.x)-1, cvRound(point.y)-1);
   distacnceVector[2] = filtered.get_distance(cvRound(point.x)-1, cvRound(point.y)+1);
   distacnceVector[3] = filtered.get_distance(cvRound(point.x)+1, cvRound(point.y)-1);
   distacnceVector[4] = filtered.get_distance(cvRound(point.x)+1, cvRound(point.y)+1);

   float pointDistance = 0;
   for (auto i : distacnceVector)
       pointDistance += i;

   return pointDistance/5;
}
#endif


