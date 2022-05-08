 #include "jkrobot.h"
#include <QApplication>
#include <QCoreApplication>
#include "inc/JAKAZuRobot.h"
#include <iostream>
#include <QTimer>
#include <QDebug>
#include <librealsense2/rs.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <Eigen/Geometry>
#include <Eigen/Dense>
#include "../detect/detector.h"
//#include<algorithm>
#include "ComData.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define PI 3.1415926

using namespace cv;
using namespace std;
using namespace Eigen;
using Eigen::MatrixXd;

static BOOL is_pos;
static BOOL in_pos;
float temp, s, temp1, s1;
static double angle4,angle5,angle6 ;
float dist_to_centerold;
float dist_to_centernew;
static int kkidxnew;//开关编号
static int statenew;//将要去的状态 0-分 1-合  -1-左（135） 0-中（90） 1-右（45）
static int valuenew;//当前状态(两档不自锁value始终为0)
float alpha1;

static double iv_y,iv_z,iv_y1=0,iv_z1=0;//intermediate variable 每次操作结束后记录当前开关相对于标定点的差值



void metadata_to_csv(const rs2::frame& frm, const std::string& filename);

CartesianPose cart;

CartesianPose tcp_pos;
CartesianPose tcp_pos1;
JointValue refP;
Mat src, src1, src2, hsv, hsv1, hsv2, result, result1, result2, ys, ys1, ys2, ys3, ys11, ys21, ys31, ys12, ys22, ys32;
Rect CRect, CRect1;

/*******************
     *****ms延时程序******
     *******************/
void Delay_MSec(unsigned int msec)
{
    QEventLoop loop;//定义一个新的事件循环
    QTimer::singleShot(msec, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
    loop.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
}

/**判断机械臂是否运动*/
void jkstate(BOOL &in_pos,BOOL &is_pos)
{

//    in_pos=1;
//    while(in_pos==1)
//    {
//        JKrobot.is_in_pos(&in_pos);

//    }
//    while(in_pos==0)
//    {
//        JKrobot.is_in_pos(&in_pos);

//    }
//    return;
//    JKrobot.is_in_pos(&in_pos);
//    if(in_pos==1){
//        return;
//    }
//    else{
//    is_pos=1;
    JKrobot.is_in_pos(&is_pos);
    JKrobot.is_in_pos(&in_pos);
    while (!(is_pos==0&&in_pos==1))
    {
        is_pos=in_pos;  //此时is_pos != in_pos==0的时候
        JKrobot.is_in_pos(&in_pos);

    }
    Delay_MSec(1000);
    return;
}

/*******************
     *****空气开关位置检测******
     *******************/
int Aspd()
{
    s=0,s1=0,temp=0,temp1=0;
    //列宽
    //cout << "clos:" << result.cols<<endl;
    //行高
    //cout << "rows:" << result.rows << endl;
    vector< vector<Point> > contours1;
    vector<Vec4i> hierarchy;
    Rect rect(0, 0, result.cols, CRect.y + CRect.height/2);
    Mat img = result(rect);
    //imshow("1", img);
    findContours(img, contours1, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

    for (int i = 0; i < contours1.size(); ++i) {
        temp = fabs(contourArea(contours1[i]));
        //cout << "i=" << i << "  area=" << temp << endl;
        s = s + temp;
    }
    qDebug("S = %f", s);

    vector< vector<Point> > contours2;
    vector<Vec4i> hierarchy1;

    Rect rect1(0, CRect.y + CRect.height/2, result.cols, CRect.height/2);

    Mat img1 = result(rect1);
    //imshow("2", img1);
    findContours(img1, contours2, hierarchy1, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);


    for (int i = 0; i < contours2.size(); ++i)
    {
        temp1 = fabs(contourArea(contours2[i]));
        //cout << "i=" << i << "  area=" << temp1 << endl;
        s1 = s1 + temp1;
    }
    qDebug("S1 = %f", s1);

}

/*******************
     ***机器人连接程序*****
     *******************/
int RobotConnect()
{
    int i=0;
    i=JKrobot.login_in("10.5.5.100");          //连接机器人
    std::cout<<i<<std::endl;
    Delay_MSec(1000);
    i=JKrobot.power_on();
    Delay_MSec(2000);                       //延时2s
    if(i==0)    i=JKrobot.enable_robot();      //机器人使能
    else        return (2);
    if(i==0)    return (0);
    else        return (3);
}

/*******************
     **机器人断开连接程序***
     *******************/
int RobotDisconnect()
{
    int i=0;
    i=JKrobot.disable_robot();                     //机器人下使能
    std::cout<<i<<std::endl;
    if(i!=0)
    {
        std::cout<<i<<std::endl;
        return (1);
    }

    Delay_MSec(4000);                           //延时2s

    i = JKrobot.power_off();                          //机器人下电
    if (i!=0)
    {
        std::cout<<i<<std::endl;
        return (2);
    }

    return (0);

}

/***************************
     ***获取机器人工具末端位姿程序***
     ***************************/
double GetRobotPosition(double* add_x,double* add_y,double* add_z,double* add_rx,double* add_ry,double* add_rz)
{
    CartesianPose tcp_pos;
    JKrobot.get_tcp_position(&tcp_pos);
    std::cout << "1# tcp_pos is :\n x: " << tcp_pos.tran.x << " y: " << tcp_pos.tran.y << " z: " << tcp_pos.tran.z << std::endl;
    std::cout << "rx: " << tcp_pos.rpy.rx << " ry: " << tcp_pos.rpy.ry << " rz: " << tcp_pos.rpy.rz << std::endl;
    *add_x = tcp_pos.tran.x;
    *add_y = tcp_pos.tran.y;
    *add_z = tcp_pos.tran.z;
    *add_rx = tcp_pos.rpy.rx;
    *add_ry = tcp_pos.rpy.ry;
    *add_rz = tcp_pos.rpy.rz;
    return (0);
}

/***************************
     ***设置机器人工具末端位姿程序***
     **************int*************/
double SetRobotPosition(double add_x,double add_y,double add_z,double add_rx,double add_ry,double add_rz)
{
    int i;
    cart.tran.x = add_x;  cart.tran.y = add_y;  cart.tran.z = add_z;
    cart.rpy.rx = add_rx; cart.rpy.ry = add_ry; cart.rpy.rz = add_rz;

    i = JKrobot.linear_move(&cart,  MoveMode::ABS, TRUE, sysparaset.linearspeed);
    std::cout<<i<<std::endl;
    return (0);
}

/***************************
     ***机器人初始位置程序***
     ***************************/
int RobotInitialPosition()
{
    int i;

    GetRobotPosition(&tcp_pos.tran.x, &tcp_pos.tran.y, &tcp_pos.tran.z, &tcp_pos.rpy.rx, &tcp_pos.rpy.ry, &tcp_pos.rpy.rz);

    cart.tran.x = -350; cart.tran.y = 0; cart.tran.z = tcp_pos.tran.z;
    cart.rpy.rx = tcp_pos.rpy.rx; cart.rpy.ry = tcp_pos.rpy.ry; cart.rpy.rz = tcp_pos.rpy.rz;
    i = JKrobot.linear_move(&cart, MoveMode::ABS, TRUE, sysparaset.linearspeed*1.5);

    cart.tran.x = -350; cart.tran.y = 0; cart.tran.z = 600;
    cart.rpy.rx = tcp_pos.rpy.rx; cart.rpy.ry = tcp_pos.rpy.ry; cart.rpy.rz = tcp_pos.rpy.rz;
    i = JKrobot.linear_move(&cart, MoveMode::ABS, TRUE, sysparaset.linearspeed*1.5);

    cart.tran.x = tcp_posbd.tran.x; cart.tran.y = tcp_posbd.tran.y; cart.tran.z = tcp_posbd.tran.z;
    cart.rpy.rx = tcp_posbd.rpy.rx; cart.rpy.ry = tcp_posbd.rpy.ry; cart.rpy.rz = tcp_posbd.rpy.rz;
    i = JKrobot.linear_move(&cart, MoveMode::ABS, TRUE, sysparaset.linearspeed*1.5);

    refP.jVal[0] =-270 * PI / 180; refP.jVal[1] = 0 * PI / 180; refP.jVal[2] = 152 * PI / 180;
    refP.jVal[3] = 120 * PI / 180; refP.jVal[4] = 0 * PI / 180; refP.jVal[5] = 0 * PI / 180;
    i = JKrobot.joint_move(&refP, MoveMode::ABS, TRUE, sysparaset.angularspeed*1.5);
    return (0);
}

/***************************
     ******相机平面矫正************
     ***************************/
int camerafix()
{

    //相机内参矩阵

    Matrix3f m;
    MatrixXf k(3,1),n(3,1);
    m = In_parameter.inverse();

    //相机通道与配置

    rs2::colorizer color_map;
    rs2::align align_to(RS2_STREAM_COLOR);
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

    //设置限定参数
    int differ_flag = true;
    int differ_x, differ_y ;
    float differ_z1 , differ_z2 , differ_z3 ;

    //设置相关参数
    int j;
    for (j = 1; j < 11 && differ_flag; j++)
    {
        //拍照
        for (auto i = 0; i < 30; ++i) pipe1.wait_for_frames();
        for (auto&& frame : pipe1.wait_for_frames())
        {
            if (auto vf = frame.as<rs2::video_frame>())
            {
                auto stream = frame.get_profile().stream_type();
                if (vf.is<rs2::depth_frame>()) vf = color_map.process(frame);
                std::stringstream png_file;
                png_file << "/dev/shm/rs-save-to-disk-output-" << vf.get_profile().stream_name() << ".png";
                stbi_write_png(png_file.str().c_str(), vf.get_width(), vf.get_height(),
                               vf.get_bytes_per_pixel(), vf.get_data(), vf.get_stride_in_bytes());
                std::cout << "Saved " << png_file.str() <<std::endl;
            }
        }

        //读取src
        src = imread("/dev/shm/rs-save-to-disk-output-Color.png");
        //灰度化
        cvtColor(src, hsv, COLOR_BGR2GRAY);
        GaussianBlur(hsv, hsv, Size(9, 9), 2, 2);

        vector<Vec3f> circles;
        //进行霍夫圆变换
        HoughCircles(hsv, circles, CV_HOUGH_GRADIENT, 1, 10, 80, 40, 0, 0);

        size_t i = 0;
        float dist_to_center;
        float dist_to_center1;
        float dist_to_center2;
        float dist_to_center3;
        float dist_to_center4;

        for ( i = 0; i < circles.size(); i++)
        {
            int radius_r = cvRound(circles[i][2]);

            //深度配置
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

            dist_to_center = filtered.get_distance(cvRound(circles[i][0]), cvRound(circles[i][1]));
            dist_to_center1 = filtered.get_distance(cvRound(circles[i][0])-1, cvRound(circles[i][1])-1);
            dist_to_center2 = filtered.get_distance(cvRound(circles[i][0])-1, cvRound(circles[i][1])+1);
            dist_to_center3 = filtered.get_distance(cvRound(circles[i][0])+1, cvRound(circles[i][1])-1);
            dist_to_center4 = filtered.get_distance(cvRound(circles[i][0])+1, cvRound(circles[i][1])+1);

            dist_to_center = (dist_to_center+dist_to_center1+dist_to_center2+dist_to_center3+dist_to_center4)/5;
            std::cout << "The camera i1s facing an object " << dist_to_center << " meters away \r"<<std::endl;

            double rd = dist_to_center*1000 * ( 2 * radius_r ) / In_parameter(0,0);

            qDebug("x = %d, y = %d，r = %d, i=%zu, rd=%f", cvRound(circles[i][0]), cvRound(circles[i][1]),radius_r,i,rd);

            if( rd>=13&&rd<=17)

                break;

        }

        qDebug("x = %d,y = %d, i=%zu", cvRound(circles[i][0]), cvRound(circles[i][1]),i);

        int cameracenterx = In_parameter(0,2);
        int cameracentery = In_parameter(1,2);

        n(0, 0) = cvRound(circles[i][0]);
        n(1, 0) = cvRound(circles[i][1]);
        n(2, 0) = 1.0;

        k = m * (dist_to_center*1000 * n);

        double k1, k2, k3;
        k1 = k(0, 0);
        k2 = k(1, 0);
        k3 = k(2, 0);

        GetRobotPosition(&tcp_pos.tran.x, &tcp_pos.tran.y, &tcp_pos.tran.z, &tcp_pos.rpy.rx, &tcp_pos.rpy.ry, &tcp_pos.rpy.rz);

        cart.tran.x = tcp_pos.tran.x + 480 - dist_to_center*1000; cart.tran.y = tcp_pos.tran.y + k1; cart.tran.z = tcp_pos.tran.z - k2;
        cart.rpy.rx = tcp_pos.rpy.rx; cart.rpy.ry = tcp_pos.rpy.ry; cart.rpy.rz = tcp_pos.rpy.rz;
        JKrobot.linear_move(&cart, MoveMode::ABS, TRUE, sysparaset.linearspeed);


        //调整光心后拍照
        double circlex1=0,circley1=0,circled1=0,circlex2=0,circley2=0,circled2=0,circlex3=0,circley3=0,circled3=0;

        for (auto i = 0; i < 30; ++i) pipe1.wait_for_frames();
        for (auto&& frame : pipe1.wait_for_frames())
        {
            if (auto vf = frame.as<rs2::video_frame>())
            {
                auto stream = frame.get_profile().stream_type();
                if (vf.is<rs2::depth_frame>()) vf = color_map.process(frame);
                std::stringstream png_file;
                png_file << "/dev/shm/rs-save-to-disk-output-" << vf.get_profile().stream_name() << ".png";
                stbi_write_png(png_file.str().c_str(), vf.get_width(), vf.get_height(),
                               vf.get_bytes_per_pixel(), vf.get_data(), vf.get_stride_in_bytes());
                std::cout << "Saved " << png_file.str() <<std::endl;
            }
        }

        //opencv
        src1 = imread("/dev/shm/rs-save-to-disk-output-Color.png");


        Rect rect1(cameracenterx-100, cameracentery-100, 300, 300);

        Mat img1 = src1(rect1);

        cvtColor(img1, hsv2, COLOR_BGR2GRAY);

        GaussianBlur(hsv2, hsv2, Size(9, 9), 2, 2);

        detect::Corner detectRect(hsv2);
        detectRect.detect();
        detectRect.printCornersWithDistance();
        vector<Vec3f> cornersDepth = detectRect.cornersWithDepth;

        circlex1=cornersDepth[0][0],circley1=cornersDepth[0][1],circled1=cornersDepth[0][2];
        circlex2=cornersDepth[1][0],circley2=cornersDepth[1][1],circled2=cornersDepth[1][2];
        circlex3=cornersDepth[2][0],circley3=cornersDepth[2][1],circled3=cornersDepth[2][2];

        double  u1=circlex1 + cameracenterx - 100, v1=circley1 + cameracentery - 100,
                u2=circlex2 + cameracenterx - 100, v2=circley2 + cameracentery - 100,
                u3=circlex3 + cameracenterx - 100, v3=circley3 + cameracentery - 100;

        qDebug("x = %f,y = %f,d = %f", u1, v1, circled1);
        qDebug("x = %f,y = %f,d = %f", u2, v2, circled2);
        qDebug("x = %f,y = %f,d = %f", u3, v3, circled3);

        differ_x = fabs (circlex1-circlex3);
        differ_y = fabs (circley1-circley2);

        differ_z1 = fabs (circled1-circled2);
        differ_z2 = fabs (circled1-circled3);
        differ_z3 = fabs (circled2-circled3);

        differ_flag = (differ_x>2)||(differ_y>2)||(differ_z1>sysparaset.errthreshold)||(differ_z2>sysparaset.errthreshold)||(differ_z3>sysparaset.errthreshold);
        qDebug("x=%d,y=%d,z1=%f,z2=%f,z3=%f",differ_x,differ_y,differ_z1,differ_z2,differ_z3);
        qDebug("d=%d",differ_flag) ;
        qDebug("j=%d",j);


        //旋转角度计算与机械臂执行
        double X1, Y1, X2, Y2, X3, Y3;

        X1 = circled1 * (u1 - In_parameter(0,2)) / In_parameter(0,0); Y1 = circled1 * (v1 - In_parameter(1,2)) / In_parameter(1,1);
        X2 = circled2 * (u2 - In_parameter(0,2)) / In_parameter(0,0); Y2 = circled2 * (v2 - In_parameter(1,2)) / In_parameter(1,1);
        X3 = circled3 * (u3 - In_parameter(0,2)) / In_parameter(0,0); Y3 = circled3 * (v3 - In_parameter(1,2)) / In_parameter(1,1);


        double d12 = circled1 - circled2;   //1-2
        double d13 = circled1 - circled3;   //1-3
        double x12 = X1 - X2;               //1-2
        double y12 = Y1 - Y2;               //1-2
        double y13 = Y1 - Y3;               //1-3

        double v12=v1-v2,u12=u1-u2;
        angle4 = atan(d13 / y13);
        angle5 = atan(d12 / x12);
        angle6 = atan(v12 / u12);

        qDebug("angle6=%g,angle5=%g,angle4=%g",angle6,angle5,angle4);

        if(differ_flag==0)
            break;

        float alf=-angle6,beta=angle4,gam=angle5;
        Matrix3f rotx,roty,rotz,rot_start,rot_mid,rot_end;

        //定义rz，ry，rx三个旋转矩阵
        rotz(0,0)=cos(alf);
        rotz(0,1)=-sin(alf);
        rotz(0,2)=0;
        rotz(1,0)=sin(alf);
        rotz(1,1)=cos(alf);
        rotz(1,2)=0;
        rotz(2,0)=0;
        rotz(2,1)=0;
        rotz(2,2)=1;

        roty(0,0)=cos(beta);
        roty(0,1)=0;
        roty(0,2)=sin(beta);
        roty(1,0)=0;
        roty(1,1)=1;
        roty(1,2)=0;
        roty(2,0)=-sin(beta);
        roty(2,1)=0;
        roty(2,2)=cos(beta);

        rotx(0,0)=1;
        rotx(0,1)=0;
        rotx(0,2)=0;
        rotx(1,0)=0;
        rotx(1,1)=cos(gam);
        rotx(1,2)=-sin(gam);
        rotx(2,0)=0;
        rotx(2,1)=sin(gam);
        rotx(2,2)=cos(gam);

        CartesianPose tcp_pos_start,tcp_pos_end;
        //获取当前机械臂的位姿
        JKrobot.get_tcp_position(&tcp_pos_start);
        std::cout << "tcp_pos is :\n x: " << tcp_pos_start.tran.x << " y: " << tcp_pos_start.tran.y << "z: " << tcp_pos_start.tran.z << std::endl;
        std::cout << "rx: " << tcp_pos_start.rpy.rx << " ry: " << tcp_pos_start.rpy.ry << " rz: " << tcp_pos_start.rpy.rz << std::endl;
        RotMatrix rot_matrix1,rot_matrix2;
        //欧拉角转换成旋转矩阵
        JKrobot.rpy_to_rot_matrix(&tcp_pos_start.rpy,&rot_matrix1);
        rot_start(0,0)=rot_matrix1.x.x;
        rot_start(0,1)=rot_matrix1.y.x;
        rot_start(0,2)=rot_matrix1.z.x;
        rot_start(1,0)=rot_matrix1.x.y;
        rot_start(1,1)=rot_matrix1.y.y;
        rot_start(1,2)=rot_matrix1.z.y;
        rot_start(2,0)=rot_matrix1.x.z;
        rot_start(2,1)=rot_matrix1.y.z;
        rot_start(2,2)=rot_matrix1.z.z;

        rot_mid=rotz*roty*rotx*rot_start;

        rot_matrix2.x.x=rot_mid(0,0);
        rot_matrix2.y.x=rot_mid(0,1);
        rot_matrix2.z.x=rot_mid(0,2);
        rot_matrix2.x.y=rot_mid(1,0);
        rot_matrix2.y.y=rot_mid(1,1);
        rot_matrix2.z.y=rot_mid(1,2);
        rot_matrix2.x.z=rot_mid(2,0);
        rot_matrix2.y.z=rot_mid(2,1);
        rot_matrix2.z.z=rot_mid(2,2);

        //旋转矩阵转换成欧拉角
        Rpy rpy;
        JKrobot.rot_matrix_to_rpy(&rot_matrix2,&rpy);
        std::cout<<"rx:"<<rpy.rx<<"ry:"<<rpy.ry<<"rz:"<<rpy.rz<<std::endl;
        tcp_pos_end.tran.x=tcp_pos_start.tran.x;
        tcp_pos_end.tran.y=tcp_pos_start.tran.y;
        tcp_pos_end.tran.z=tcp_pos_start.tran.z;
        tcp_pos_end.rpy.rx=rpy.rx;
        tcp_pos_end.rpy.ry=rpy.ry;
        tcp_pos_end.rpy.rz=rpy.rz;
        //机械臂运动到旋转后的姿态
        JKrobot.set_rapidrate(0.05);
        JKrobot.linear_move(&tcp_pos_end,MoveMode::ABS,TRUE,20);
        JKrobot.set_rapidrate(1);
        Delay_MSec(1000);
    }

    for (auto i = 0; i < 30; ++i) pipe1.wait_for_frames();
    for (auto&& frame : pipe1.wait_for_frames())
    {
        if (auto vf = frame.as<rs2::video_frame>())
        {
            auto stream = frame.get_profile().stream_type();
            if (vf.is<rs2::depth_frame>()) vf = color_map.process(frame);
            std::stringstream png_file;
            png_file << "/dev/shm/rs-save-to-disk-output-" << vf.get_profile().stream_name() << ".png";
            stbi_write_png(png_file.str().c_str(), vf.get_width(), vf.get_height(),
                           vf.get_bytes_per_pixel(), vf.get_data(), vf.get_stride_in_bytes());
            std::cout << "Saved " << png_file.str() <<std::endl;
        }
    }

    //读取src
    src = imread("/dev/shm/rs-save-to-disk-output-Color.png");

    //灰度化
    cvtColor(src, hsv, COLOR_BGR2GRAY);
    GaussianBlur(hsv, hsv, Size(9, 9), 2, 2);

    vector<Vec3f> circles;
    //进行霍夫圆变换
    HoughCircles(hsv, circles, CV_HOUGH_GRADIENT, 1, 10, 80, 40, 0, 0);

    size_t i = 0;
    float dist_to_center;
    float dist_to_center1;
    float dist_to_center2;
    float dist_to_center3;
    float dist_to_center4;

    for ( i = 0; i < circles.size(); i++)
    {
//        //参数定义
//        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius_r = cvRound(circles[i][2]);
//        circle(hsv, center, 1, Scalar(255, 0, 0), -1, 8, 0);

//        //绘制圆轮廓
//        circle(hsv, center, radius_r, Scalar(255, 0, 0), 2, 8, 0);

        qDebug("x = %d,y = %d，r = %d, i=%zu\n", cvRound(circles[i][0]), cvRound(circles[i][1]),radius_r,i);

        //移动光心

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

        dist_to_center = filtered.get_distance(cvRound(circles[i][0]), cvRound(circles[i][1]));
        dist_to_center1 = filtered.get_distance(cvRound(circles[i][0])-1, cvRound(circles[i][1])-1);
        dist_to_center2 = filtered.get_distance(cvRound(circles[i][0])-1, cvRound(circles[i][1])+1);
        dist_to_center3 = filtered.get_distance(cvRound(circles[i][0])+1, cvRound(circles[i][1])-1);
        dist_to_center4 = filtered.get_distance(cvRound(circles[i][0])+1, cvRound(circles[i][1])+1);

        dist_to_center = (dist_to_center+dist_to_center1+dist_to_center2+dist_to_center3+dist_to_center4)/5;
        std::cout << "The camera i1s facing an object " << dist_to_center << " meters away \r"<<std::endl;

        double rd = dist_to_center*1000 * ( 2 * radius_r ) / In_parameter(0,0);

        qDebug("rd=%f",rd);

        if( rd>=13&&rd<=17)

            break;

    }

    qDebug("x = %d,y = %d, i=%zu", cvRound(circles[i][0]), cvRound(circles[i][1]),i);

    n(0, 0) = cvRound(circles[i][0]);
    n(1, 0) = cvRound(circles[i][1]);
    n(2, 0) = 1.0;

    k = m * (dist_to_center*1000 * n);

    double k1, k2, k3;
    k1 = k(0, 0);
    k2 = k(1, 0);
    k3 = k(2, 0);


    GetRobotPosition(&tcp_pos.tran.x, &tcp_pos.tran.y, &tcp_pos.tran.z, &tcp_pos.rpy.rx, &tcp_pos.rpy.ry, &tcp_pos.rpy.rz);

//    cart.tran.x = tcp_pos.tran.x + 440 - dist_to_center*1000; cart.tran.y = tcp_pos.tran.y + k1; cart.tran.z = tcp_pos.tran.z - k2;
//    cart.rpy.rx = tcp_pos.rpy.rx; cart.rpy.ry = tcp_pos.rpy.ry; cart.rpy.rz = tcp_pos.rpy.rz;
//    JKrobot.linear_move(&cart, MoveMode::ABS, TRUE, sysparaset.linearspeed);
    JKrobot.jog(1,INCR,COORD_TOOL,40,-k1);
    jkstate(in_pos,is_pos);
    JKrobot.jog(0,INCR,COORD_TOOL,40,k2);
    jkstate(in_pos,is_pos);
    JKrobot.jog(2,INCR,COORD_TOOL,40,-(480 - dist_to_center*1000));
    jkstate(in_pos,is_pos);


    GetRobotPosition (&tcp_posbd.tran.x,&tcp_posbd.tran.y,&tcp_posbd.tran.z,&tcp_posbd.rpy.rx,&tcp_posbd.rpy.ry,&tcp_posbd.rpy.rz);

    if (j<11&&differ_flag==0)
    return (0);//成功
    else
    return (2);//失败

}

int move2calibrationpos(int equidx)
{
    //int i=10;

    iv_y1=0;
    iv_z1=0;

    JKrobot.set_rapidrate(1);
    //static int inpos=1;
    refP.jVal[0] = -90 * PI / 180; refP.jVal[1] = 0 * PI / 180; refP.jVal[2] = 152 * PI / 180;
    refP.jVal[3] = 120 * PI / 180; refP.jVal[4] = 0 * PI / 180; refP.jVal[5] = 0 * PI / 180;
    JKrobot.joint_move(&refP, MoveMode::ABS, TRUE, sysparaset.angularspeed*1.5);

    refP.jVal[0] = -90 * PI / 180; refP.jVal[1] = 0 * PI / 180; refP.jVal[2] = -11 * PI / 180;
    refP.jVal[3] = 120 * PI / 180; refP.jVal[4] = 0 * PI / 180; refP.jVal[5] = 0 * PI / 180;
    JKrobot.joint_move(&refP, MoveMode::ABS, TRUE, sysparaset.angularspeed*1.5);

    refP.jVal[0] = -90 * PI / 180; refP.jVal[1] = 21.508 * PI / 180; refP.jVal[2] = -136.859 * PI / 180;
    refP.jVal[3] = 115.722 * PI / 180; refP.jVal[4] = -90 * PI / 180; refP.jVal[5] = -90 * PI / 180;
    JKrobot.joint_move(&refP, MoveMode::ABS, TRUE, sysparaset.angularspeed*1.5);

    refP.jVal[0] = -180 * PI / 180; refP.jVal[1] = 21.508 * PI / 180; refP.jVal[2] = -136.859 * PI / 180;
    refP.jVal[3] = 115.722 * PI / 180; refP.jVal[4] = -90 * PI / 180; refP.jVal[5] = -90 * PI / 180;
    JKrobot.joint_move(&refP, MoveMode::ABS, TRUE, sysparaset.angularspeed*1.5);

    cart.tran.x = -330; cart.tran.y = 0; cart.tran.z = equparaset[equidx]. calibration_dz - 166.219;
    cart.rpy.rx = 0 * PI / 180; cart.rpy.ry = 90 * PI / 180; cart.rpy.rz = -180 * PI / 180;
    JKrobot.linear_move(&cart, MoveMode::ABS, TRUE, sysparaset.linearspeed*1.5);

    tight();

    int b = camerafix();                                                                  // 相机平面校准
    if (b==0)
        return (0);//成功
    else
        return (2);//失败

}


int confirm()
{

    cout<<"编号："<<kkidxnew<<"  "<<"将要去的状态："<<statenew<<"  "<<"当前状态："<<valuenew<<" "<<kkparaset[kkidxnew].value<<endl;

    if(kkidxnew>=2&&kkidxnew<=5)//button
    {

        Delay_MSec(1000);
        JKrobot.jog(2,INCR,COORD_TOOL,40,-(-dist_to_centerold*1000 + sysparaset.dx+ kkparaset[kkidxnew].locate_dx));
        jkstate(in_pos,is_pos);
        JKrobot.jog(2,INCR,COORD_TOOL,40,kkparaset[kkidxnew].opt_x);
        jkstate(in_pos,is_pos);
        JKrobot.jog(2,INCR,COORD_TOOL,40,-kkparaset[kkidxnew].opt_x);
        jkstate(in_pos,is_pos);
        JKrobot.jog_stop(0);

    }

    else if(kkidxnew==0)//airswich

    {
        if(valuenew==0&&statenew==1)//up

        {


            JKrobot.jog(0,INCR,COORD_TOOL,40,-(sysparaset.dz - kkparaset[kkidxnew].locate_dz));
            jkstate(in_pos,is_pos);

            JKrobot.jog(1,INCR,COORD_TOOL,40,-(sysparaset.dy + kkparaset[kkidxnew].locate_dy));
            jkstate(in_pos,is_pos);


            JKrobot.jog(2,INCR,COORD_TOOL,40,-(-dist_to_centernew*1000 + sysparaset.dx + kkparaset[kkidxnew].locate_dx));
            jkstate(in_pos,is_pos);

            CartesianPose tcpMid,end_p,mid_p;
            GetRobotPosition(&tcpMid.tran.x,&tcpMid.tran.y,&tcpMid.tran.z,&tcpMid.rpy.rx,&tcpMid.rpy.ry,&tcpMid.rpy.rz);
            end_p.tran.x=tcpMid.tran.x;end_p.tran.y=tcpMid.tran.y;end_p.tran.z=tcpMid.tran.z+30;
            end_p.rpy.rx=tcpMid.rpy.rx;end_p.rpy.ry=tcpMid.rpy.ry;end_p.rpy.rz=tcpMid.rpy.rz;
            mid_p.tran.x=tcpMid.tran.x+2;mid_p.tran.y=tcpMid.tran.y;mid_p.tran.z=tcpMid.tran.z+20;
            mid_p.rpy.rx=tcpMid.rpy.rx;mid_p.rpy.ry=tcpMid.rpy.ry;mid_p.rpy.rz=tcpMid.rpy.rz;
            JKrobot.circular_move(&end_p,&mid_p,ABS,TRUE,sysparaset.linearspeed,1,0,NULL);

            Delay_MSec(1000);
            cart.tran.x = 0 ; cart.tran.y = 0 ; cart.tran.z = -kkparaset[kkidxnew].opt_z ;
            cart.rpy.rx = 0 ; cart.rpy.ry = 0  ; cart.rpy.rz = 0 ;
            JKrobot.linear_move(&cart, MoveMode::INCR, TRUE, sysparaset.linearspeed);
            Delay_MSec(1000);
        }

        if(valuenew==1&&statenew==0)//down
        {
            JKrobot.jog(0,INCR,COORD_TOOL,40,-(sysparaset.dz + kkparaset[kkidxnew].locate_dz));
            jkstate(in_pos,is_pos);

            JKrobot.jog(1,INCR,COORD_TOOL,40,-(sysparaset.dy + kkparaset[kkidxnew].locate_dy));
            jkstate(in_pos,is_pos);


            JKrobot.jog(2,INCR,COORD_TOOL,40,-(-dist_to_centernew*1000 + sysparaset.dx+ kkparaset[kkidxnew].locate_dx));
            jkstate(in_pos,is_pos);

            cart.tran.x = 0 ; cart.tran.y = 0 ; cart.tran.z = -kkparaset[kkidxnew].opt_z ;
            cart.rpy.rx = 0 ; cart.rpy.ry = 0  ; cart.rpy.rz = 0 ;
            JKrobot.linear_move(&cart, MoveMode::INCR, TRUE, sysparaset.linearspeed);
            Delay_MSec(1000);
            cart.tran.x = 0 ; cart.tran.y = 0 ; cart.tran.z = kkparaset[kkidxnew].opt_z ;
            cart.rpy.rx = 0 ; cart.rpy.ry = 0  ; cart.rpy.rz = 0 ;
            JKrobot.linear_move(&cart, MoveMode::INCR, TRUE, sysparaset.linearspeed);
            Delay_MSec(1000);
        }


    }

    else
    {

        release();

        //先调整好角度以便可以插进去
        if(alpha1>=125&&valuenew==-1)//当前为左
        {
            refP.jVal[0] = 0 * PI / 180; refP.jVal[1] = 0 * PI / 180; refP.jVal[2] = 0 * PI / 180;
            refP.jVal[3] = 0 * PI / 180; refP.jVal[4] = 0 * PI / 180; refP.jVal[5] = -kkparaset[kkidxnew].locate_angle6 * PI / 180;
            JKrobot.joint_move(&refP, MoveMode::INCR, TRUE, sysparaset.angularspeed);
        }
        if(alpha1>=80&&alpha1<=100&&valuenew==0)//当前为中
        {
            refP.jVal[0] = 0 * PI / 180; refP.jVal[1] = 0 * PI / 180; refP.jVal[2] = 0 * PI / 180;
            refP.jVal[3] = 0 * PI / 180; refP.jVal[4] = 0 * PI / 180; refP.jVal[5] = 0 * PI / 180;
            JKrobot.joint_move(&refP, MoveMode::INCR, TRUE, sysparaset.angularspeed);
        }
        if(alpha1<=55&&valuenew==1)//当前为右
        {
            refP.jVal[0] = 0 * PI / 180; refP.jVal[1] = 0 * PI / 180; refP.jVal[2] = 0 * PI / 180;
            refP.jVal[3] = 0 * PI / 180; refP.jVal[4] = 0 * PI / 180; refP.jVal[5] = kkparaset[kkidxnew].locate_angle6 * PI / 180;
            JKrobot.joint_move(&refP, MoveMode::INCR, TRUE, sysparaset.angularspeed);
        }

        JKrobot.jog(2,INCR,COORD_TOOL,40,-(- dist_to_centernew*1000 + sysparaset.dx + kkparaset[kkidxnew].locate_dx));
        jkstate(in_pos,is_pos);

        tight();

        if((alpha1>=80&&alpha1<=100&&valuenew==0&&statenew==-1&&kkparaset[kkidxnew].type==21)||(alpha1<=55&&valuenew==1&&statenew==0&&kkparaset[kkidxnew].type==21))//左旋45 三档自锁
        {
            refP.jVal[0] = 0 * PI / 180; refP.jVal[1] = 0 * PI / 180; refP.jVal[2] = 0 * PI / 180;
            refP.jVal[3] = 0 * PI / 180; refP.jVal[4] = 0 * PI / 180; refP.jVal[5] = -kkparaset[kkidxnew].opt_angle6 * PI / 180;
            JKrobot.joint_move(&refP, MoveMode::INCR, TRUE, sysparaset.angularspeed);
        }

        if((alpha1>=125&&valuenew==-1&&statenew==0&&kkparaset[kkidxnew].type==21)||(alpha1>=80&&alpha1<=100&&valuenew==0&&statenew==1&&kkparaset[kkidxnew].type==21))//右旋45 三档自锁
        {
            refP.jVal[0] = 0 * PI / 180; refP.jVal[1] = 0 * PI / 180; refP.jVal[2] = 0 * PI / 180;
            refP.jVal[3] = 0 * PI / 180; refP.jVal[4] = 0 * PI / 180; refP.jVal[5] = kkparaset[kkidxnew].opt_angle6 * PI / 180;
            JKrobot.joint_move(&refP, MoveMode::INCR, TRUE, sysparaset.angularspeed);
        }

        if(alpha1<=55&&valuenew==1&&statenew==-1&&kkparaset[kkidxnew].type==21)//左旋90 两档自锁（-1，1）
        {
            refP.jVal[0] = 0 * PI / 180; refP.jVal[1] = 0 * PI / 180; refP.jVal[2] = 0 * PI / 180;
            refP.jVal[3] = 0 * PI / 180; refP.jVal[4] = 0 * PI / 180; refP.jVal[5] = -kkparaset[kkidxnew].opt_angle6 * PI / 90;
            JKrobot.joint_move(&refP, MoveMode::INCR, TRUE, sysparaset.angularspeed);
        }

        if(alpha1>=125&&valuenew==-1&&statenew==1&&kkparaset[kkidxnew].type==21)//右旋90 两档自锁（-1，1）
        {
            refP.jVal[0] = 0 * PI / 180; refP.jVal[1] = 0 * PI / 180; refP.jVal[2] = 0 * PI / 180;
            refP.jVal[3] = 0 * PI / 180; refP.jVal[4] = 0 * PI / 180; refP.jVal[5] = kkparaset[kkidxnew].opt_angle6 * PI / 90;
            JKrobot.joint_move(&refP, MoveMode::INCR, TRUE, sysparaset.angularspeed);
        }

        if(alpha1>=80&&alpha1<=100&&valuenew==0&&statenew==-1&&kkparaset[kkidxnew].type==20)//左旋  不自锁
        {
            refP.jVal[0] = 0 * PI / 180; refP.jVal[1] = 0 * PI / 180; refP.jVal[2] = 0 * PI / 180;
            refP.jVal[3] = 0 * PI / 180; refP.jVal[4] = 0 * PI / 180; refP.jVal[5] = -kkparaset[kkidxnew].opt_angle6 * PI / 180;
            JKrobot.joint_move(&refP, MoveMode::INCR, TRUE, sysparaset.angularspeed);
            refP.jVal[0] = 0 * PI / 180; refP.jVal[1] = 0 * PI / 180; refP.jVal[2] = 0 * PI / 180;
            refP.jVal[3] = 0 * PI / 180; refP.jVal[4] = 0 * PI / 180; refP.jVal[5] = +kkparaset[kkidxnew].opt_angle6 * PI / 180;
            JKrobot.joint_move(&refP, MoveMode::INCR, TRUE, sysparaset.angularspeed);
        }

        if(alpha1>=80&&alpha1<=100&&valuenew==0&&statenew==1&&kkparaset[kkidxnew].type==20)//右旋  不自锁
        {
            refP.jVal[0] = 0 * PI / 180; refP.jVal[1] = 0 * PI / 180; refP.jVal[2] = 0 * PI / 180;
            refP.jVal[3] = 0 * PI / 180; refP.jVal[4] = 0 * PI / 180; refP.jVal[5] = +kkparaset[kkidxnew].opt_angle6 * PI / 180;
            JKrobot.joint_move(&refP, MoveMode::INCR, TRUE, sysparaset.angularspeed);
            refP.jVal[0] = 0 * PI / 180; refP.jVal[1] = 0 * PI / 180; refP.jVal[2] = 0 * PI / 180;
            refP.jVal[3] = 0 * PI / 180; refP.jVal[4] = 0 * PI / 180; refP.jVal[5] = -kkparaset[kkidxnew].opt_angle6 * PI / 180;
            JKrobot.joint_move(&refP, MoveMode::INCR, TRUE, sysparaset.angularspeed);
        }

        release();

        Delay_MSec(1000);
        JKrobot.jog(2,INCR,COORD_TOOL,40,-200);
        jkstate(in_pos,is_pos);
        tight();

    }

    Delay_MSec(1000);
    cart.tran.x = tcp_pos.tran.x; cart.tran.y = tcp_pos.tran.y; cart.tran.z = tcp_pos.tran.z;
    cart.rpy.rx = tcp_pos.rpy.rx; cart.rpy.ry = tcp_pos.rpy.ry; cart.rpy.rz = tcp_pos.rpy.rz;
    JKrobot.linear_move(&cart, MoveMode::ABS, TRUE, sysparaset.linearspeed);

    return (0);

}



int button(int kkidx, int state, bool num)
{

    Matrix3f m;
    MatrixXf k(3,1),n(3,1);

    m = In_parameter.inverse();

    cart.tran.x = tcp_posbd.tran.x ; cart.tran.y = tcp_posbd.tran.y +kkparaset[kkidx].dy; cart.tran.z = tcp_posbd.tran.z+kkparaset[kkidx].dz;
    cart.rpy.rx = tcp_posbd.rpy.rx ; cart.rpy.ry = tcp_posbd.rpy.ry; cart.rpy.rz = tcp_posbd.rpy.rz;
    JKrobot.linear_move(&cart, MoveMode::ABS, TRUE, sysparaset.linearspeed);


try{
    rs2::colorizer color_map;
    rs2::align align_to(RS2_STREAM_COLOR);
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

    //拍照移光心
    float arr[1][3];
    double circlex=0,ciecley=0,circled=0;
    for (auto i = 0; i < 30; ++i) pipe1.wait_for_frames();
    for (auto&& frame : pipe1.wait_for_frames())
    {
        if (auto vf = frame.as<rs2::video_frame>())
        {
            auto stream = frame.get_profile().stream_type();
            if (vf.is<rs2::depth_frame>()) vf = color_map.process(frame);
            std::stringstream png_file;
            png_file << "/dev/shm/rs-save-to-disk-output-" << vf.get_profile().stream_name() << ".png";
            stbi_write_png(png_file.str().c_str(), vf.get_width(), vf.get_height(),
                           vf.get_bytes_per_pixel(), vf.get_data(), vf.get_stride_in_bytes());
            std::cout << "Saved " << png_file.str() <<std::endl;
        }
    }

    //opencv
    src = imread("/dev/shm/rs-save-to-disk-output-Color.png");


    Rect rect(In_parameter(0,2)-100, In_parameter(1,2)-100, kkparaset[kkidx].pix_w, kkparaset[kkidx].pix_h);
    Mat img = src(rect);
//    GaussianBlur(img, img, Size(9, 9), 2, 2);
    imwrite("../img.png", img);
//    cvtColor(img, hsv, COLOR_BGR2HSV);
//    inRange(hsv, Scalar(kkparaset[kkidx].color_value1, kkparaset[kkidx].color_value2, kkparaset[kkidx].color_value3), Scalar(kkparaset[kkidx].color_value4, kkparaset[kkidx].color_value5, kkparaset[kkidx].color_value6), ys1);
//    inRange(hsv, Scalar(kkparaset[kkidx].color_value7, kkparaset[kkidx].color_value8, kkparaset[kkidx].color_value9), Scalar(kkparaset[kkidx].color_value10, kkparaset[kkidx].color_value11, kkparaset[kkidx].color_value12), ys2);
//    addWeighted(ys1, 1, ys2, 1, 0, ys3);
//    Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
//    morphologyEx(ys3, result, MORPH_OPEN, element);//形态学开运算降噪
//    GaussianBlur(result, result, Size(9, 9), 2, 2);
//    vector<Vec3f> circles;//进行霍夫圆变换
//    HoughCircles(result, circles, CV_HOUGH_GRADIENT, 1, 10, 80, 30, 0, 0);

//    if(circles.empty()){
//        throw -1;
//    }

    int btnColor=detect::BtnColor::other;
    detect::Btn* myBtn;
    if(kkparaset[kkidx].color_value1==35)
    {
        //green
        myBtn = new detect::Btn(img, detect::BtnColor::green);
    }
    else if(kkparaset[kkidx].color_value1==0)
    {
        myBtn = new detect::Btn(img, detect::BtnColor::red);
    }
    myBtn->detect();

    vector<Vec3f> circles;
    circles.push_back(myBtn->getCircles());
    if (circles[0][2] == 0 ) //半径为0
        throw -1;

    bool myBtnState = myBtn->isLighted();
    for (auto c : circles)
        cout << c << endl;
    if(myBtnState) {
        valuenew=1;
    }
    else {
        valuenew = 0;
    }

    float dist_to_center;
    float dist_to_center1;
    float dist_to_center2;
    float dist_to_center3;
    float dist_to_center4;

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

    dist_to_center = filtered.get_distance(cvRound(circles[0][0])+ In_parameter(0,2)-100 , cvRound(circles[0][1])+ In_parameter(1,2)-100 );
    dist_to_center1 = filtered.get_distance(cvRound(circles[0][0])+ In_parameter(0,2)-100-1, cvRound(circles[0][1])+ In_parameter(1,2)-100-1);
    dist_to_center2 = filtered.get_distance(cvRound(circles[0][0])+ In_parameter(0,2)-100-1, cvRound(circles[0][1])+ In_parameter(1,2)-100+1);
    dist_to_center3 = filtered.get_distance(cvRound(circles[0][0])+ In_parameter(0,2)-100+1, cvRound(circles[0][1])+ In_parameter(1,2)-100-1);
    dist_to_center4 = filtered.get_distance(cvRound(circles[0][0])+ In_parameter(0,2)-100+1, cvRound(circles[0][1])+ In_parameter(1,2)-100+1);

    dist_to_centerold = (dist_to_center+dist_to_center1+dist_to_center2+dist_to_center3+dist_to_center4)/5;

    std::cout << "The camera i1s facing an object " << dist_to_centerold << " meters away \r"<<std::endl;

    arr[0][0]=cvRound(circles[0][0]),arr[0][1]=cvRound(circles[0][1]),arr[0][2]=dist_to_centerold;

    circlex=arr[0][0],ciecley=arr[0][1],circled=arr[0][2];

    n(0,0)= circlex + In_parameter(0,2)-100 ;
    n(1,0)= ciecley + In_parameter(1,2)-100 ;
    n(2,0)= 1.0;

    qDebug("x = %f , y = %f , d = %f" , n(0,0), n(1,0) , circled);

    k = m * ((circled*1000) * n);

    double k1,k2,k3;
    k1=k(0,0);
    k2=k(1,0);
    k3=k(2,0);
    JKrobot.jog(1,INCR,COORD_TOOL,40,-k1);
    jkstate(in_pos,is_pos);
    JKrobot.jog(0,INCR,COORD_TOOL,40,k2);
    jkstate(in_pos,is_pos);
    //Delay_MSec(3000);





    //向前走250拍照识别字符
    JKrobot.jog(2,INCR,COORD_TOOL,40,-(230 - dist_to_centerold*1000));
    jkstate(in_pos,is_pos);
    Delay_MSec(3000);
    for (auto&& frame : pipe1.wait_for_frames())
    {
        if (auto vf = frame.as<rs2::video_frame>())
        {
            auto stream = frame.get_profile().stream_type();
            if (vf.is<rs2::depth_frame>()) vf = color_map.process(frame);
            std::stringstream png_file;
            png_file << "/dev/shm/rs-save-to-disk-output-" << vf.get_profile().stream_name() << ".png";
            stbi_write_png(png_file.str().c_str(), vf.get_width(), vf.get_height(),
                           vf.get_bytes_per_pixel(), vf.get_data(), vf.get_stride_in_bytes());
            std::cout << "Saved " << png_file.str() <<std::endl;
        }
    }
    src2 = imread("/dev/shm/rs-save-to-disk-output-Color.png");

    Rect rect2(In_parameter(0,2)-100, In_parameter(1,2), 200, 200);
    Mat img2 = src2(rect2);
    detect::TextRecongize text_recongize;
    vector<string> text_result=text_recongize.recongize_text(img2);
    for(int i=0;i<text_recongize.numofText;i++){
        cout<<text_result[i]<<endl;
    }

    JKrobot.jog(2,INCR,COORD_TOOL,40,(230 - dist_to_centerold*1000));
    jkstate(in_pos,is_pos);

    for (auto&& frame : pipe1.wait_for_frames())
    {
        if (auto vf = frame.as<rs2::video_frame>())
        {
            auto stream = frame.get_profile().stream_type();
            if (vf.is<rs2::depth_frame>()) vf = color_map.process(frame);
            std::stringstream png_file;
            png_file << "/dev/shm/rs-save-to-disk-output-" << vf.get_profile().stream_name() << ".png";
            stbi_write_png(png_file.str().c_str(), vf.get_width(), vf.get_height(),
                           vf.get_bytes_per_pixel(), vf.get_data(), vf.get_stride_in_bytes());
            std::cout << "Saved " << png_file.str() <<std::endl;
        }
    }

    //opencv
    src1 = imread("/dev/shm/rs-save-to-disk-output-Color.png");    
    Rect rect1(In_parameter(0,2)-100, In_parameter(1,2)-100, kkparaset[kkidx].pix_w, kkparaset[kkidx].pix_h);
    Mat img1 = src1(rect1);

//    GaussianBlur(img1, img1, Size(9, 9), 2, 2);
    imwrite("../img1.png", img1);
    detect::Btn* myBtn2;
    if(kkparaset[kkidx].color_value1==35)
    {
        //green
        myBtn2 = new detect::Btn(img1, detect::BtnColor::green);
    }
    else if(kkparaset[kkidx].color_value1==0)
    {
        myBtn2 = new detect::Btn(img1, detect::BtnColor::red);
        cout << "run here" << "red" << endl;
    }

    myBtn2->detect();
    vector<Vec3f> circles1;
    circles1.push_back(myBtn2->getCircles());
    if (circles1[0][2] == 0)
        throw -1;
//    cvtColor(img1, hsv1, COLOR_BGR2HSV);//RGB转HSV
//    inRange(hsv1, Scalar(kkparaset[kkidx].color_value1, kkparaset[kkidx].color_value2, kkparaset[kkidx].color_value3), Scalar(kkparaset[kkidx].color_value4, kkparaset[kkidx].color_value5, kkparaset[kkidx].color_value6), ys11);
//    inRange(hsv1, Scalar(kkparaset[kkidx].color_value7, kkparaset[kkidx].color_value8, kkparaset[kkidx].color_value9), Scalar(kkparaset[kkidx].color_value10, kkparaset[kkidx].color_value11, kkparaset[kkidx].color_value12), ys21);
//    addWeighted(ys11, 1, ys21, 1, 0, ys31);
//    morphologyEx(ys31, result1, MORPH_OPEN, element);//形态学开运算降噪
//    GaussianBlur(result1, result1, Size(9, 9), 2, 2);

//    //进行霍夫圆变换
//    vector<Vec3f> circles1;
//    HoughCircles(result1, circles1, CV_HOUGH_GRADIENT, 1, 10, 80, 30, 0, 0);
//    if(circles1.empty()){
//        throw -1;
//    }

    dist_to_center = filtered.get_distance(cvRound(circles1[0][0])+ In_parameter(0,2)-100 , cvRound(circles1[0][1])+ In_parameter(1,2)-100 );
    dist_to_center1 = filtered.get_distance(cvRound(circles1[0][0])+ In_parameter(0,2)-100-1, cvRound(circles1[0][1])+ In_parameter(1,2)-100-1);
    dist_to_center2 = filtered.get_distance(cvRound(circles1[0][0])+ In_parameter(0,2)-100-1, cvRound(circles1[0][1])+ In_parameter(1,2)-100+1);
    dist_to_center3 = filtered.get_distance(cvRound(circles1[0][0])+ In_parameter(0,2)-100+1, cvRound(circles1[0][1])+ In_parameter(1,2)-100-1);
    dist_to_center4 = filtered.get_distance(cvRound(circles1[0][0])+ In_parameter(0,2)-100+1, cvRound(circles1[0][1])+ In_parameter(1,2)-100+1);

    dist_to_centernew = (dist_to_center+dist_to_center1+dist_to_center2+dist_to_center3+dist_to_center4)/5;

    std::cout << "The camera i1s facing an object " << dist_to_centernew << " meters away \r"<<std::endl;

    arr[0][0]=cvRound(circles1[0][0]),arr[0][1]=cvRound(circles1[0][1]),arr[0][2]=dist_to_centernew;

    circlex=arr[0][0],ciecley=arr[0][1],circled=arr[0][2];

    n(0,0)= circlex + In_parameter(0,2)-100 ;
    n(1,0)= ciecley + In_parameter(1,2)-100 ;
    n(2,0)= 1.0;

    qDebug("x = %f , y = %f , d = %f" , n(0,0), n(1,0) , circled);

    JKrobot.jog(1,INCR,COORD_TOOL,50,-(sysparaset.dy + kkparaset[kkidx].locate_dy));
    jkstate(in_pos,is_pos);
    JKrobot.jog(0,INCR,COORD_TOOL,50,-(sysparaset.dz+kkparaset[kkidx].locate_dz));
    jkstate(in_pos,is_pos);
    //JKrobot.jog_stop(0);

//    for (auto&& frame : pipe1.wait_for_frames())
//    {
//        if (auto vf = frame.as<rs2::video_frame>())
//        {
//            auto stream = frame.get_profile().stream_type();
//            if (vf.is<rs2::depth_frame>()) vf = color_map.process(frame);
//            std::stringstream png_file;
//            png_file << "/dev/shm/rs-save-to-disk-output-" << vf.get_profile().stream_name() << ".png";
//            stbi_write_png(png_file.str().c_str(), vf.get_width(), vf.get_height(),
//                           vf.get_bytes_per_pixel(), vf.get_data(), vf.get_stride_in_bytes());
//            std::cout << "Saved " << png_file.str() <<std::endl;
//        }
//    }
//    src2 = imread("/dev/shm/rs-save-to-disk-output-Color.png");


    GetRobotPosition (&tcp_pos.tran.x,&tcp_pos.tran.y,&tcp_pos.tran.z,&tcp_pos.rpy.rx,&tcp_pos.rpy.ry,&tcp_pos.rpy.rz);

    kkidxnew=kkidx;
    statenew=state;
}catch(int e){
    cout<<e<<endl;

    }

    return 0;
}




int airswitch(int kkidx, int state, bool num)
{
    //相机内参矩阵
    Matrix3f m;
    MatrixXf k(3,1),n(3,1);
    m = In_parameter.inverse();

    cart.tran.x = tcp_posbd.tran.x ; cart.tran.y = tcp_posbd.tran.y +kkparaset[kkidx].dy; cart.tran.z = tcp_posbd.tran.z+kkparaset[kkidx].dz;
    cart.rpy.rx = tcp_posbd.rpy.rx ; cart.rpy.ry = tcp_posbd.rpy.ry; cart.rpy.rz = tcp_posbd.rpy.rz;
    JKrobot.linear_move(&cart, MoveMode::ABS, TRUE, sysparaset.linearspeed);

try{
    rs2::colorizer color_map;
    rs2::align align_to(RS2_STREAM_COLOR);
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

    for (auto i = 0; i < 30; ++i) pipe1.wait_for_frames();
    for (auto&& frame : pipe1.wait_for_frames())
    {
        if (auto vf = frame.as<rs2::video_frame>())
        {
            auto stream = frame.get_profile().stream_type();
            if (vf.is<rs2::depth_frame>()) vf = color_map.process(frame);
            std::stringstream png_file;
            png_file << "/dev/shm/rs-save-to-disk-output-" << vf.get_profile().stream_name() << ".png";
            stbi_write_png(png_file.str().c_str(), vf.get_width(), vf.get_height(),
                           vf.get_bytes_per_pixel(), vf.get_data(), vf.get_stride_in_bytes());
            std::cout << "Saved " << png_file.str() <<std::endl;
        }
    }

    //opencv
    src = imread("/dev/shm/rs-save-to-disk-output-Color.png");
    Mat kernel = (Mat_<char>(3,3)<<0,-1,0,-1,5,-1,0,-1,0);
    filter2D(src,src,src.depth(),kernel);
    Rect rect(In_parameter(0,2)-50, In_parameter(1,2)-100, kkparaset[kkidx].pix_w, kkparaset[kkidx].pix_h);
    Mat img = src(rect);
    cvtColor(img, hsv, COLOR_BGR2HSV);
    inRange(hsv, Scalar(kkparaset[kkidx].color_value1, kkparaset[kkidx].color_value2, kkparaset[kkidx].color_value3), Scalar(kkparaset[kkidx].color_value4, kkparaset[kkidx].color_value5, kkparaset[kkidx].color_value6), ys);
    Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
    morphologyEx(ys, result, MORPH_OPEN, element);//形态学开运算降噪

    vector<vector<Point>> contours1;//找轮廓
    vector<vector<Point>> contours2;
    cv::findContours(result, contours1, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    if(contours1.empty()){
        throw -1;
    }

    double maxLength = 0;
    int maxLengthNo = 0;
    for (size_t t = 0; t < contours1.size(); t++)
    {
        double length = arcLength(contours1[t], true);
        //cout << "第" << t << "个轮廓长度=" << length << endl;
        if (length > maxLength) {
            maxLength = length;
            maxLengthNo = t;
        }
    }
    contours2.push_back(contours1[maxLengthNo]);

    Mat hole(result.size(), CV_8U, Scalar(0)); //遮罩图层
    cv::drawContours(hole, contours2, -1, Scalar(255), CV_FILLED); //在遮罩图层上，用白色像素填充轮廓
    vector<vector<Point>> contours3; //放轮廓点
    vector<Vec4i> hierachy;          //图像的拓扑结构
    findContours(hole, contours3, hierachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(-1, -1));//根据轮廓点找最小矩形


    vector<vector<Point>> contours_ploy(contours3.size()); //存放绘制矩形的点
    vector<Rect> ploy_rects(contours3.size());
    vector<RotatedRect> minRects(contours3.size());
    Mat imageContours = Mat::zeros(hole.size(), CV_8UC1);	//最小外接矩形画布

    for (size_t i = 0; i < contours3.size(); i++)
    {
        drawContours(imageContours, contours3, i, Scalar(255), 1, 8, hierachy);
        RotatedRect rect = minAreaRect(contours3[i]);
        Point2f P[4];
        rect.points(P);
        for (int j = 0; j <= 3; j++)
        {
            line(imageContours, P[j], P[(j + 1) % 4], Scalar(255), 2);
        }

        CRect = boundingRect(contours3[i]);
        cout << "左上角坐标：" << CRect.tl() << endl;
        cout << CRect.x << endl;
        cout << CRect.y << endl;
        cout << CRect.width << endl;
        cout << CRect.height << endl;

        float dist_to_center;
        float dist_to_center1;
        float dist_to_center2;
        float dist_to_center3;
        float dist_to_center4;

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

        dist_to_center = filtered.get_distance(In_parameter(0,2)-50 + CRect.x + CRect.width/2 , In_parameter(1,2)-100 + CRect.y + CRect.height/2 );
        dist_to_center1 = filtered.get_distance(In_parameter(0,2)-50 + CRect.x + CRect.width/2-1, In_parameter(1,2)-100 + CRect.y + CRect.height/2-1);
        dist_to_center2 = filtered.get_distance(In_parameter(0,2)-50 + CRect.x + CRect.width/2-1, In_parameter(1,2)-100 + CRect.y + CRect.height/2+1);
        dist_to_center3 = filtered.get_distance(In_parameter(0,2)-50 + CRect.x + CRect.width/2+1, In_parameter(1,2)-100 + CRect.y + CRect.height/2-1);
        dist_to_center4 = filtered.get_distance(In_parameter(0,2)-50 + CRect.x + CRect.width/2+1, In_parameter(1,2)-100 + CRect.y + CRect.height/2+1);

        dist_to_centerold = (dist_to_center+dist_to_center1+dist_to_center2+dist_to_center3+dist_to_center4)/5;

        std::cout << "The camera i1s facing an object " << dist_to_centerold << " meters away \r"<<std::endl;

        n(0,0)= In_parameter(0,2)-50 + CRect.x + CRect.width/2 ;
        n(1,0)= In_parameter(1,2)-100 + CRect.y + CRect.height/2 ;
        n(2,0)= 1.0;



        //判断空开分合确定当前状态
        if(n(1,0)>In_parameter(1,2))
        {
            valuenew=0;
        }
        else
        {
            valuenew=1;
        }

        qDebug("x = %f,y = %f,d = %f",  n(0,0), n(1,0), dist_to_centerold) ;

        k = m * ((dist_to_centerold*1000) * n);
        double k1,k2,k3;
        k1=k(0,0);
        k2=k(1,0);
        k3=k(2,0);

        JKrobot.jog(1,INCR,COORD_TOOL,40,-k1);
        jkstate(in_pos,is_pos);
        JKrobot.jog(0,INCR,COORD_TOOL,40,k2);
        jkstate(in_pos,is_pos);

    }
    Delay_MSec(1000);




    //向前走250拍照识别字符
    JKrobot.jog(2,INCR,COORD_TOOL,40,-(230 - dist_to_centerold*1000));
    jkstate(in_pos,is_pos);
    Delay_MSec(3000);
    for (auto&& frame : pipe1.wait_for_frames())
    {
        if (auto vf = frame.as<rs2::video_frame>())
        {
            auto stream = frame.get_profile().stream_type();
            if (vf.is<rs2::depth_frame>()) vf = color_map.process(frame);
            std::stringstream png_file;
            png_file << "/dev/shm/rs-save-to-disk-output-" << vf.get_profile().stream_name() << ".png";
            stbi_write_png(png_file.str().c_str(), vf.get_width(), vf.get_height(),
                           vf.get_bytes_per_pixel(), vf.get_data(), vf.get_stride_in_bytes());
            std::cout << "Saved " << png_file.str() <<std::endl;
        }
    }
    src2 = imread("/dev/shm/rs-save-to-disk-output-Color.png");
    float ip12=In_parameter(1,2)+10;
    if(valuenew==1) ip12=ip12+30;
    Rect rect2(In_parameter(0,2)-100, ip12, 200, 200);
    Mat img2 = src2(rect2);

    detect::TextRecongize text_recongize;
    vector<string> text_result=text_recongize.recongize_text(img2);
    for(int i=0;i<text_recongize.numofText;i++){
        cout<<text_result[i]<<endl;
    }

    JKrobot.jog(2,INCR,COORD_TOOL,40,(230 - dist_to_centerold*1000));
    jkstate(in_pos,is_pos);


    for (auto i = 0; i < 30; ++i) pipe1.wait_for_frames();
    for (auto&& frame : pipe1.wait_for_frames())
    {
        if (auto vf = frame.as<rs2::video_frame>())
        {
            auto stream = frame.get_profile().stream_type();
            if (vf.is<rs2::depth_frame>()) vf = color_map.process(frame);
            std::stringstream png_file;
            png_file << "/dev/shm/rs-save-to-disk-output-" << vf.get_profile().stream_name() << ".png";
            stbi_write_png(png_file.str().c_str(), vf.get_width(), vf.get_height(),
                           vf.get_bytes_per_pixel(), vf.get_data(), vf.get_stride_in_bytes());
            std::cout << "Saved " << png_file.str() <<std::endl;
        }
    }
    //opencv
    src1 = imread("/dev/shm/rs-save-to-disk-output-Color.png");
    filter2D(src1,src1,src1.depth(),kernel);
    Rect rect1(In_parameter(0,2)-50, In_parameter(1,2)-100, kkparaset[kkidx].pix_w, kkparaset[kkidx]. pix_h);
    Mat img1 = src1(rect1);
    cvtColor(img1, hsv1, COLOR_BGR2HSV);
    inRange(hsv1, Scalar(kkparaset[kkidx].color_value1, kkparaset[kkidx].color_value2, kkparaset[kkidx].color_value3), Scalar(kkparaset[kkidx].color_value4, kkparaset[kkidx].color_value5, kkparaset[kkidx].color_value6), ys1);
    morphologyEx(ys1, result1, MORPH_OPEN, element);//形态学开运算降噪

    vector<vector<Point>> contours11;//找轮廓
    vector<vector<Point>> contours21;
    cv::findContours(result1, contours11, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    if(contours11.empty()){
        throw -1;
    }
    double maxLength1 = 0;
    int maxLengthNo1 = 0;
    for (size_t t = 0; t < contours11.size(); t++)
    {
        double length = arcLength(contours11[t], true);
        //cout << "第" << t << "个轮廓长度=" << length << endl;
        if (length > maxLength1) {
            maxLength1 = length;
            maxLengthNo1 = t;
        }
    }
    contours21.push_back(contours11[maxLengthNo1]);

    Mat hole1(result1.size(), CV_8U, Scalar(0)); //遮罩图层
    cv::drawContours(hole1, contours21, -1, Scalar(255), CV_FILLED); //在遮罩图层上，用白色像素填充轮廓
    vector<vector<Point>> contours31; //放轮廓点
    vector<Vec4i> hierachy1;          //图像的拓扑结构
    findContours(hole1, contours31, hierachy1, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(-1, -1));//根据轮廓点找最小矩形

    vector<vector<Point>> contours_ploy1(contours31.size()); //存放绘制矩形的点
    vector<Rect> ploy_rects1(contours31.size());
    vector<RotatedRect> minRects1(contours31.size());
    Mat imageContours1 = Mat::zeros(hole1.size(), CV_8UC1);	//最小外接矩形画布

    for (size_t i = 0; i < contours31.size(); i++)
    {
        drawContours(imageContours1, contours31, i, Scalar(255), 1, 8, hierachy1);
        RotatedRect rect1 = minAreaRect(contours31[i]);
        Point2f P1[4];
        rect1.points(P1);
        for (int j = 0; j <= 3; j++)
        {
            line(imageContours1, P1[j], P1[(j + 1) % 4], Scalar(255), 2);
        }

        Rect CRect1;
        CRect1 = boundingRect(contours31[i]);

        cout << "左上角坐标：" << CRect1.tl() << endl;
        cout << CRect1.x << endl;
        cout << CRect1.y << endl;
        cout << CRect1.width << endl;
        cout << CRect1.height << endl;

        float dist_to_center11;
        float dist_to_center12;
        float dist_to_center13;
        float dist_to_center14;
        float dist_to_center15;

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

        dist_to_center11 = filtered.get_distance(In_parameter(0,2)-50 + CRect1.x + CRect1.width/2 , In_parameter(1,2)-100  + CRect1.y + CRect1.height/2 );
        dist_to_center12 = filtered.get_distance(In_parameter(0,2)-50 + CRect1.x + CRect1.width/2-1, In_parameter(1,2)-100  + CRect1.y + CRect1.height/2-1);
        dist_to_center13 = filtered.get_distance(In_parameter(0,2)-50 + CRect1.x + CRect1.width/2-1, In_parameter(1,2)-100  + CRect1.y + CRect1.height/2+1);
        dist_to_center14 = filtered.get_distance(In_parameter(0,2)-50 + CRect1.x + CRect1.width/2+1, In_parameter(1,2)-100  + CRect1.y + CRect1.height/2-1);
        dist_to_center15 = filtered.get_distance(In_parameter(0,2)-50 + CRect1.x + CRect1.width/2+1, In_parameter(1,2)-100  + CRect1.y + CRect1.height/2+1);

        dist_to_centernew = (dist_to_center11+dist_to_center12+dist_to_center13+dist_to_center14+dist_to_center15)/5;

        std::cout << "The camera i1s facing an object " << dist_to_centernew << " meters away \r"<<std::endl;

        n(0,0)= In_parameter(0,2)-50 + CRect1.x + CRect1.width/2  ;
        n(1,0)= In_parameter(1,2)-100 + CRect1.y + CRect1.height/2  ;
        n(2,0)= 1.0;


        qDebug("x = %f,y = %f,d = %f",  n(0,0), n(1,0), dist_to_centernew) ;


//        if(kkparaset[kkidx].value==0)//上拨
//        {

//            JKrobot.jog(0,INCR,COORD_TOOL,40,-(sysparaset.dz - kkparaset[kkidx].locate_dz));
//            jkstate(in_pos,is_pos);

//            JKrobot.jog(1,INCR,COORD_TOOL,40,-(sysparaset.dy + kkparaset[kkidx].locate_dy));
//            jkstate(in_pos,is_pos);

//        }

//        if(kkparaset[kkidx].value==1)//下拨
//        {

//            JKrobot.jog(0,INCR,COORD_TOOL,40,-(sysparaset.dz + kkparaset[kkidx].locate_dz));
//            jkstate(in_pos,is_pos);

//            JKrobot.jog(1,INCR,COORD_TOOL,40,-(sysparaset.dy + kkparaset[kkidx].locate_dy));
//            jkstate(in_pos,is_pos);

//        }

    }

//    for (auto&& frame : pipe1.wait_for_frames())
//    {
//        if (auto vf = frame.as<rs2::video_frame>())
//        {
//            auto stream = frame.get_profile().stream_type();
//            if (vf.is<rs2::depth_frame>()) vf = color_map.process(frame);
//            std::stringstream png_file;
//            png_file << "/dev/shm/rs-save-to-disk-output-" << vf.get_profile().stream_name() << ".png";
//            stbi_write_png(png_file.str().c_str(), vf.get_width(), vf.get_height(),
//                           vf.get_bytes_per_pixel(), vf.get_data(), vf.get_stride_in_bytes());
//            std::cout << "Saved " << png_file.str() <<std::endl;
//        }
//    }
//    src2 = imread("/dev/shm/rs-save-to-disk-output-Color.png");


    GetRobotPosition (&tcp_pos.tran.x,&tcp_pos.tran.y,&tcp_pos.tran.z,&tcp_pos.rpy.rx,&tcp_pos.rpy.ry,&tcp_pos.rpy.rz);

    kkidxnew=kkidx;
    statenew=state;
    }catch(int e){
        cout<<e<<endl;
    }


    return (0);

}

int knob(int kkidx, int state, bool num)
{
    //相机内参矩阵
    Matrix3f m;
    MatrixXf k(3,1),n(3,1);
    m = In_parameter.inverse();
    float x1, y1, w1, h1;

    cart.tran.x = tcp_posbd.tran.x ; cart.tran.y = tcp_posbd.tran.y + kkparaset[kkidx].dy; cart.tran.z = tcp_posbd.tran.z+kkparaset[kkidx].dz;
    cart.rpy.rx = tcp_posbd.rpy.rx ; cart.rpy.ry = tcp_posbd.rpy.ry; cart.rpy.rz = tcp_posbd.rpy.rz;
    JKrobot.linear_move(&cart, MoveMode::ABS, TRUE, sysparaset.linearspeed);



try{
    rs2::colorizer color_map;
    rs2::align align_to(RS2_STREAM_COLOR);
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

    for (auto i = 0; i < 30; ++i) pipe1.wait_for_frames();
    for (auto&& frame : pipe1.wait_for_frames())
    {
        if (auto vf = frame.as<rs2::video_frame>())
        {
            auto stream = frame.get_profile().stream_type();
            if (vf.is<rs2::depth_frame>()) vf = color_map.process(frame);
            std::stringstream png_file;
            png_file << "/dev/shm/rs-save-to-disk-output-" << vf.get_profile().stream_name() << ".png";
            stbi_write_png(png_file.str().c_str(), vf.get_width(), vf.get_height(),
                           vf.get_bytes_per_pixel(), vf.get_data(), vf.get_stride_in_bytes());
            std::cout << "Saved " << png_file.str() <<std::endl;
        }
    }
    src = imread("/dev/shm/rs-save-to-disk-output-Color.png");
    Mat kernel = (Mat_<char>(3,3)<<0,-1,0,-1,5,-1,0,-1,0);
    filter2D(src,src,src.depth(),kernel);
    Rect rect(In_parameter(0,2)-100, In_parameter(1,2)-100, kkparaset[kkidx]. pix_w, kkparaset[kkidx]. pix_h);
    Mat img = src(rect);

    //    KnobSwitch knobSwitch(img);
    //    knobSwitch.getKnobSwitch();
    //    cout << "angle: " << knobSwitch.angle << endl;
    //    cout << "site: " << knobSwitch.site << endl;

    cvtColor(img, hsv, COLOR_BGR2HSV);
    inRange(hsv, Scalar(kkparaset[kkidx].color_value1, kkparaset[kkidx].color_value2, kkparaset[kkidx].color_value3), Scalar(kkparaset[kkidx].color_value4, kkparaset[kkidx].color_value5, kkparaset[kkidx].color_value6), ys);
    Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
    morphologyEx(ys, result, MORPH_OPEN, element);//形态学开运算降噪

    vector<vector<Point>> contours1;//找轮廓
    vector<vector<Point>> contours2;
    cv::findContours(result, contours1, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    if(contours1.empty()){
        throw -1;
    }
    double maxLength = 0;
    int maxLengthNo = 0;
    for (size_t t = 0; t < contours1.size(); t++)
    {
        double length = arcLength(contours1[t], true);
        //cout << "第" << t << "个轮廓长度=" << length << endl;
        if (length > maxLength) {
            maxLength = length;
            maxLengthNo = t;
        }
    }
    contours2.push_back(contours1[maxLengthNo]);
    Mat hole(result.size(), CV_8U, Scalar(0)); //遮罩图层
    cv::drawContours(hole, contours2, -1, Scalar(255), CV_FILLED); //在遮罩图层上，用白色像素填充轮廓
    vector<vector<Point>> contours3; //放轮廓点
    vector<Vec4i> hierachy;          //图像的拓扑结构
    findContours(hole, contours3, hierachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(-1, -1));//根据轮廓点找最小矩形
    vector<vector<Point>> contours_ploy(contours3.size()); //存放绘制矩形的点
    vector<Rect> ploy_rects(contours3.size());
    vector<RotatedRect> minRects(contours3.size());
    Mat imageContours = Mat::zeros(hole.size(), CV_8UC1);	//最小外接矩形画布
    for (size_t i = 0; i < contours3.size(); i++)
    {
        drawContours(imageContours, contours3, i, Scalar(255), 1, 8, hierachy);
        RotatedRect rect = minAreaRect(contours3[i]);
        Point2f P[4];
        rect.points(P);

        for (int j = 0; j <= 3; j++)
        {
            line(imageContours, P[j], P[(j + 1) % 4], Scalar(255), 2);
        }

        float x0, y0, alpha0, w0 , h0;
        x0 = rect.center.x;
        y0 = rect.center.y;
        alpha0 = rect.angle;
        w0 = rect.size.width;
        h0 = rect.size.height;

        if (rect.angle <= 80)
        {
            if (w0 > h0)
            {
                alpha0 = rect.angle + 90;
            }
            else
            {
                alpha0 = rect.angle;
            }
        }
        qDebug("x0 = %f,y0 = %f,w0 = %f,h0 = %f,alpha0 = %f", x0,y0,w0,h0,alpha0);





        //判断旋扭角度确定当前状态(仅仅是操作前的状态，两档无自锁value是始终为0)

        if(alpha0<=55)
        {
            valuenew=1;
        }
        else if (alpha0<=100&&alpha0>=80)
        {
            valuenew=0;
        }
        else
        {
            valuenew=-1;
        }







        float dist_to_center;
        float dist_to_center1;
        float dist_to_center2;
        float dist_to_center3;
        float dist_to_center4;

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

        dist_to_center = filtered.get_distance(In_parameter(0,2)-100+x0 , In_parameter(1,2)-100+y0 );
        dist_to_center1 = filtered.get_distance(In_parameter(0,2)-100+x0-1, In_parameter(1,2)-100+y0-1);
        dist_to_center2 = filtered.get_distance(In_parameter(0,2)-100+x0-1, In_parameter(1,2)-100+y0+1);
        dist_to_center3 = filtered.get_distance(In_parameter(0,2)-100+x0+1, In_parameter(1,2)-100+y0-1);
        dist_to_center4 = filtered.get_distance(In_parameter(0,2)-100+x0+1, In_parameter(1,2)-100+y0+1);

        dist_to_centerold = (dist_to_center+dist_to_center1+dist_to_center2+dist_to_center3+dist_to_center4)/5;

        std::cout << "The camera i1s facing an object " << dist_to_centerold << " meters away \r"<<std::endl;

        n(0,0)= In_parameter(0,2)-100 + x0 ;
        n(1,0)= In_parameter(1,2)-100 + y0 ;
        n(2,0)= 1.0;

        qDebug("x = %f,y = %f,d = %f", n(0,0), n(1,0), dist_to_centerold) ;

        k = m * ((dist_to_centerold*1000) * n);
        double k1,k2,k3;
        k1=k(0,0);
        k2=k(1,0);
        k3=k(2,0);

        JKrobot.jog(1,INCR,COORD_TOOL,40,-k1);
        jkstate(in_pos,is_pos);
        JKrobot.jog(0,INCR,COORD_TOOL,40,k2);
        jkstate(in_pos,is_pos);
        Delay_MSec(1000);

    }





        //向前走250拍照识别字符
        JKrobot.jog(2,INCR,COORD_TOOL,40,-(230 - dist_to_centerold*1000));
        jkstate(in_pos,is_pos);
        Delay_MSec(3000);
        for (auto&& frame : pipe1.wait_for_frames())
        {
            if (auto vf = frame.as<rs2::video_frame>())
            {
                auto stream = frame.get_profile().stream_type();
                if (vf.is<rs2::depth_frame>()) vf = color_map.process(frame);
                std::stringstream png_file;
                png_file << "/dev/shm/rs-save-to-disk-output-" << vf.get_profile().stream_name() << ".png";
                stbi_write_png(png_file.str().c_str(), vf.get_width(), vf.get_height(),
                               vf.get_bytes_per_pixel(), vf.get_data(), vf.get_stride_in_bytes());
                std::cout << "Saved " << png_file.str() <<std::endl;
            }
        }
        src2 = imread("/dev/shm/rs-save-to-disk-output-Color.png");

        Rect rect2(In_parameter(0,2)-100, In_parameter(1,2)+50, 200, 200);
        Mat img2 = src2(rect2);

        detect::TextRecongize text_recongize;
        vector<string> text_result=text_recongize.recongize_text(img2);
        for(int i=0;i<text_recongize.numofText;i++){
            cout<<text_result[i]<<endl;
        }

        JKrobot.jog(2,INCR,COORD_TOOL,40,(230 - dist_to_centerold*1000));
        jkstate(in_pos,is_pos);





    for (auto i = 0; i < 30; ++i) pipe1.wait_for_frames();
    for (auto&& frame : pipe1.wait_for_frames())
    {
        if (auto vf = frame.as<rs2::video_frame>())
        {
            auto stream = frame.get_profile().stream_type();
            if (vf.is<rs2::depth_frame>()) vf = color_map.process(frame);
            std::stringstream png_file;
            png_file << "/dev/shm/rs-save-to-disk-output-" << vf.get_profile().stream_name() << ".png";
            stbi_write_png(png_file.str().c_str(), vf.get_width(), vf.get_height(),
                           vf.get_bytes_per_pixel(), vf.get_data(), vf.get_stride_in_bytes());
            std::cout << "Saved " << png_file.str() <<std::endl;
        }
    }


    //opencv
    src1 = imread("/dev/shm/rs-save-to-disk-output-Color.png");
    filter2D(src1,src1,src1.depth(),kernel);
    Rect rect1(In_parameter(0,2)-100, In_parameter(1,2)-100, kkparaset[kkidx]. pix_w, kkparaset[kkidx]. pix_h);
    Mat img1 = src1(rect1);
    cvtColor(img1, hsv1, COLOR_BGR2HSV);
    inRange(hsv1, Scalar(kkparaset[kkidx].color_value1, kkparaset[kkidx].color_value2, kkparaset[kkidx].color_value3), Scalar(kkparaset[kkidx].color_value4, kkparaset[kkidx].color_value5, kkparaset[kkidx].color_value6), ys1);
    morphologyEx(ys1, result1, MORPH_OPEN, element);//形态学开运算降噪

    vector<vector<Point>> contours11;//找轮廓
    vector<vector<Point>> contours21;
    cv::findContours(result1, contours11, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    if(contours11.empty()){
        throw -1;
    }

    double maxLength1 = 0;
    int maxLengthNo1 = 0;
    for (size_t t = 0; t < contours11.size(); t++)
    {
        double length = arcLength(contours11[t], true);
        //cout << "第" << t << "个轮廓长度=" << length << endl;
        if (length > maxLength1) {
            maxLength1 = length;
            maxLengthNo1 = t;
        }
    }
    contours21.push_back(contours11[maxLengthNo1]);



    Mat hole1(result1.size(), CV_8U, Scalar(0)); //遮罩图层
    cv::drawContours(hole1, contours21, -1, Scalar(255), CV_FILLED); //在遮罩图层上，用白色像素填充轮廓
    vector<vector<Point>> contours31; //放轮廓点
    vector<Vec4i> hierachy1;          //图像的拓扑结构
    findContours(hole1, contours31, hierachy1, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(-1, -1));//根据轮廓点找最小矩形

    vector<vector<Point>> contours_ploy1(contours31.size()); //存放绘制矩形的点
    vector<Rect> ploy_rects1(contours31.size());
    vector<RotatedRect> minRects1(contours31.size());
    Mat imageContours1 = Mat::zeros(hole1.size(), CV_8UC1);	//最小外接矩形画布

    for (size_t i = 0; i < contours31.size(); i++)
    {
        drawContours(imageContours1, contours31, i, Scalar(255), 1, 8, hierachy1);
        RotatedRect rect1 = minAreaRect(contours31[i]);
        Point2f P1[4];
        rect1.points(P1);
        for (int j = 0; j <= 3; j++)
        {
            line(imageContours1, P1[j], P1[(j + 1) % 4], Scalar(255), 2);
        }

        x1 = rect1.center.x;                    //  中心位置
        y1 = rect1.center.y;
        alpha1 = rect1.angle;
        w1 = rect1.size.width;
        h1 = rect1.size.height;

        if (rect1.angle <= 80)
        {
            if (w1 > h1)
            {
                alpha1 = rect1.angle + 90;
            }
            else
            {
                alpha1 = rect1.angle;
            }
        }

        qDebug("x1 = %f,y1 = %f,w1 = %f,h1 = %f,alpha1 = %f", x1,y1,w1,h1,alpha1);

        float dist_to_center11;
        float dist_to_center12;
        float dist_to_center13;
        float dist_to_center14;
        float dist_to_center15;

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

        dist_to_center11 = filtered.get_distance(In_parameter(0,2)-100 + x1  , In_parameter(1,2)-100  + y1 );
        dist_to_center12 = filtered.get_distance(In_parameter(0,2)-100 + x1-1, In_parameter(1,2)-100  + y1-1);
        dist_to_center13 = filtered.get_distance(In_parameter(0,2)-100 + x1-1, In_parameter(1,2)-100  + y1+1);
        dist_to_center14 = filtered.get_distance(In_parameter(0,2)-100 + x1+1, In_parameter(1,2)-100  + y1-1);
        dist_to_center15 = filtered.get_distance(In_parameter(0,2)-100 + x1+1, In_parameter(1,2)-100  + y1+1);

        dist_to_centernew = (dist_to_center11+dist_to_center12+dist_to_center13+dist_to_center14+dist_to_center15)/5;

        std::cout << "The camera i1s facing an object " << dist_to_centernew << " meters away \r"<<std::endl;

        n(0,0)= In_parameter(0,2)-100 + x1 ;
        n(1,0)= In_parameter(1,2)-100 + y1 ;
        n(2,0)= 1.0;

        qDebug("x = %f,y = %f,d = %f", n(0,0), n(1,0), dist_to_centernew);

        k = m * ((dist_to_centernew*1000) * n);
        double k1,k2,k3;
        k1=k(0,0);
        k2=k(1,0);
        k3=k(2,0);

        JKrobot.jog(1,INCR,COORD_TOOL,50,-(sysparaset.dy + kkparaset[kkidx].locate_dy));
        jkstate(in_pos,is_pos);
        JKrobot.jog(0,INCR,COORD_TOOL,50,-(sysparaset.dz + kkparaset[kkidx].locate_dz));
        jkstate(in_pos,is_pos);
//        for (auto&& frame : pipe1.wait_for_frames())
//        {
//            if (auto vf = frame.as<rs2::video_frame>())
//            {
//                auto stream = frame.get_profile().stream_type();
//                if (vf.is<rs2::depth_frame>()) vf = color_map.process(frame);
//                std::stringstream png_file;
//                png_file << "/dev/shm/rs-save-to-disk-output-" << vf.get_profile().stream_name() << ".png";
//                stbi_write_png(png_file.str().c_str(), vf.get_width(), vf.get_height(),
//                               vf.get_bytes_per_pixel(), vf.get_data(), vf.get_stride_in_bytes());
//                std::cout << "Saved " << png_file.str() <<std::endl;
//            }
//        }
//        src2 = imread("/dev/shm/rs-save-to-disk-output-Color.png");


    }

    GetRobotPosition (&tcp_pos.tran.x,&tcp_pos.tran.y,&tcp_pos.tran.z,&tcp_pos.rpy.rx,&tcp_pos.rpy.ry,&tcp_pos.rpy.rz);

    kkidxnew=kkidx;
    statenew=state;
    }catch(int e){
        cout<<e<<endl;
    }

    return (0);

}

int operate (int kkidx, int state, bool num)
{
    int res,res1;
    switch(kkparaset[kkidx].type)
    {
    case 0:  res1=button(kkidx,state,num);
        break;
    case 1:  res1=button(kkidx,state,num);
        break;
    case 2:  res1=button(kkidx,state,num);
        break;
    case 3:  res1=button(kkidx,state,num);
        break;
    case 4:  res1=button(kkidx,state,num);
        break;
    case 5:  res1=button(kkidx,state,num);
        break;
    case 6:  res1=button(kkidx,state,num);
        break;
    case 7:  res1=button(kkidx,state,num);
        break;
    case 8:  res1=button(kkidx,state,num);
        break;
    case 9:  res1=button(kkidx,state,num);
        break;
    case 10: res1=button(kkidx,state,num);
        break;
    case 11: res1=airswitch(kkidx,state,num);
        break;
    case 12: res1=airswitch(kkidx,state,num);
        break;
    case 13: res1=airswitch(kkidx,state,num);
        break;
    case 14: res1=airswitch(kkidx,state,num);
        break;
    case 15: res1=airswitch(kkidx,state,num);
        break;
    case 16: res1=airswitch(kkidx,state,num);
        break;
    case 17: res1=airswitch(kkidx,state,num);
        break;
    case 18: res1=airswitch(kkidx,state,num);
        break;
    case 19: res1=airswitch(kkidx,state,num);
        break;
    case 20: res1=knob(kkidx,state,num);
        break;
    case 21: res1=knob(kkidx,state,num);
        break;
    case 22: res1=knob(kkidx,state,num);
        break;
    case 23: res1=knob(kkidx,state,num);
        break;
    case 24: res1=knob(kkidx,state,num);
        break;
    case 25: res1=knob(kkidx,state,num);
        break;
    case 31: res1=knob(kkidx,state,num);
        break;
    case 32: res1=knob(kkidx,state,num);
        break;
    case 33: res1=knob(kkidx,state,num);
        break;


    }
    if(res1==0)res=0;
    else res=2;
    return res;


}

void tight()
{
    if(Jkstatus.is_socket_connect!=1)
    {
        // QMessageBox::information(this,"提示","JAKA控制器连接失败，请检查！");
        return;
    }

    JKrobot.set_digital_output(IO_TOOL,0,1);//???
    JKrobot.set_digital_output(IO_TOOL,1,0);//???
    usleep(100000);
    JKrobot.set_digital_output(IO_TOOL,1,1);//???
}

void release()
{
    if(Jkstatus.is_socket_connect!=1)
    {
        //QMessageBox::information(this,"提示","JAKA控制器连接失败，请检查！");
        return;
    }
    JKrobot.set_digital_output(IO_TOOL,0,0);
    JKrobot.set_digital_output(IO_TOOL,1,0);//???
    usleep(100000);
    JKrobot.set_digital_output(IO_TOOL,1,1);//???
}

int test()
{
    rs2::colorizer color_map;
    rs2::align align_to(RS2_STREAM_COLOR);
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

    //拍照移光心
    float arr[1][3];
    double circlex=0,ciecley=0,circled=0;
    for (auto i = 0; i < 30; ++i) pipe1.wait_for_frames();
    for (auto&& frame : pipe1.wait_for_frames())
    {
        if (auto vf = frame.as<rs2::video_frame>())
        {
            auto stream = frame.get_profile().stream_type();
            if (vf.is<rs2::depth_frame>()) vf = color_map.process(frame);
            std::stringstream png_file;
            png_file << "/dev/shm/rs-save-to-disk-output-" << vf.get_profile().stream_name() << ".png";
            stbi_write_png(png_file.str().c_str(), vf.get_width(), vf.get_height(),
                           vf.get_bytes_per_pixel(), vf.get_data(), vf.get_stride_in_bytes());
            std::cout << "Saved " << png_file.str() <<std::endl;
        }
    }

    //opencv
    src = imread("/dev/shm/rs-save-to-disk-output-Color.png");


    Rect rect(630-100, 373-100, 200, 200);
    Mat img = src(rect);

    detect::Btn myBtn(img, detect::BtnColor::green);
    myBtn.detect();
    cout << myBtn.getCircles() << endl;
    cout << myBtn.isLighted() << endl;

    return 0;

}
