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
//#include<algorithm>
#include "ComData.h"




#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define PI 3.1415926


     //相机与工具末端的偏移
    // double sysparaset.dx = 167;
    // double syaparaset.dy = -14;
    // double sysparaset.dz = 56;


     using namespace cv;
     using namespace std;
     using namespace Eigen;
     using Eigen::MatrixXd;

class DetectRect {
public:
    Mat sourceImg;
    vector<Point> corners;
    vector<Vec3f> cornersWithDepth;

    DetectRect(Mat inputImg) : sourceImg(inputImg) {

    }
//    void getPointDepth() {
//        rs2::frameset data= pipe1.wait_for_frames();
//        rs2::frameset aligned_set = align_to.process(data);
//        rs2::depth_frame depth_frame = aligned_set.get_depth_frame();
//        rs2::depth_frame filtered = depth_frame;
//        filtered = thr_filter.process(filtered);
//        filtered = depth_to_disparity.process(filtered);
//        filtered = hole_filter.process(filtered);
//        filtered = disparity_to_depth.process(filtered);
//        filtered = spat_filter.process(filtered);
//        filtered = tem_filter.process(filtered);
//    }
    void getCorners() {
        goodFeaturesToTrack(sourceImg, corners, 3, 0.1, 10);
        for (auto rect : corners) { //add corners' depth info
            cornersWithDepth.push_back(Vec3f(rect.x, rect.y, 0));
        }
    }
    void printCorners() {
        cout << "find " << corners.size() << " points" << endl;
        for (auto i : corners)
            cout << i << endl;
    }
};


     float temp, s, temp1, s1;

     void metadata_to_csv(const rs2::frame& frm, const std::string& filename);

     //JAKAZuRobot JKrobot;
     CartesianPose cart;
     //CartesianPose tcp_posbd;
     CartesianPose tcp_pos;
     CartesianPose tcp_pos1;
     JointValue refP;
     Mat src, src1, src2, hsv, hsv1, hsv2, result, result1, result2, ys, ys1, ys2, ys3, ys11, ys21, ys31, ys12, ys22, ys32;
     Rect CRect, CRect1;

     //unsigned char type;//类型，按钮：0:带灯按钮, 1:不带灯按钮,（2-9预留）；空开：10:1P,11:2P,12:3P,（13-19预留）；旋钮：20:2档旋钮,21:2档旋钮自锁,22:3档旋钮
    // unsigned char value;//开关位置，按钮/空开:0-分，1-合；旋钮：0-左旋，1-右旋，2-中间

     /*extern struct KK kkparaset[]; //调用参数开关kkparaset[kkidx].type

     struct KK
     {

         QString id;//电气柜索引，开关与柜体联系
         QString no; //编号
         QString name;//名称

         unsigned char type;//类型，0:按钮,1:1P,2:2P,3:3P,4:2档旋扭,5:2档旋扭自锁,6:3档旋扭
         int value;//选择:类型0-5:分/0，合/1，6：左/0，中/2，右/1

         double dx;//每个开关相对于标定点的偏移坐标 double x,y,z, 单位mm
         double dy;
         double dz;

         double locate_dx;//开关定位修正值dx，dy，dz，单位mm
         double locate_dy;
         double locate_dz;
         double locate_angle6;

         double opt_x;//开关操作修正值
         double opt_y;
         double opt_z;
         double opt_angle6;

         unsigned char pos;

         int pix_x; //像素截图x
         int pix_y; //像素截图y

         int pix_w; //截图宽度
         int pix_h; //截图高度

         int color; //颜色值，0-无色，1-黑色，2-红色，3-绿色，4-蓝色5-带灯红色，6-带灯绿色

         int color_value1;//颜色hsv分量的最高最低阈值
         int color_value2;
         int color_value3;
         int color_value4;
         int color_value5;
         int color_value6;
         int color_value7;
         int color_value8;
         int color_value9;
         int color_value10;
         int color_value11;
         int color_value12;

     };*/




    // extern struct CABINET equparaset[]; //调用参数标定 equparaset[equidx]. calibration_dz
//     struct CABINET
//   {

//    QString id;//索引
//    QString no; //编号
//    QString title;//名称
//    unsigned char roomno;//0:室外，1/2/3：室1，室2，JKrobot
    //类型，柜体开关布局，界面设计，JKrobot:固定/0
    //室1：按空开设置分：0：zkk1/zk1/zk2,1：zkk1/zk1,2：fb61/fb62/zk1
    //室2：按路数设置分：
    //室外：
//    unsigned char type;
//    double x;//JAKA double x,y,z, 单位mm
//    double y;
//    double z;
//    double rx;//JAKA double rx,ry,rz,单位：rad
//    double ry;
//    double rz;
//    double shx;//ROS  position double x,y,z
//    double shy;
//    double shz;
//    double shx1;//ROS orientation  double x,y,z,
//    double shy1;
//    double shz1;
//    double shw1;
//    double calibration_dz;//标定点高度，由展开位置移动至标定位置
//};
//     CABINET equparaset;

//bool num;

     /*******************
     *****ms延时程序******
     *******************/
     void Delay_MSec(unsigned int msec)
     {
         QEventLoop loop;//定义一个新的事件循环
         QTimer::singleShot(msec, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
         loop.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
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
         //std::cout<<JKrobot.login_in("10.5.5.100")<<std::endl;
         i=JKrobot.login_in("10.5.5.100");          //连接机器人
         std::cout<<i<<std::endl;
         //if(i==0)    i=JKrobot.power_on();        //机器人上电
         //else        return (1);
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

         //i=JKrobot.login_in("10.5.5.100");

         //if(i==0)
         //{
         //Delay_MSec(2000);
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
         //CartesianPose cart;

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
         cart.tran.x = -100; cart.tran.y = 0; cart.tran.z = 0;
         cart.rpy.rx = 0; cart.rpy.ry = 0; cart.rpy.rz = 0;
         i = JKrobot.linear_move(&cart, MoveMode::INCR, TRUE, sysparaset.linearspeed);

         cart.tran.x = tcp_posbd.tran.x; cart.tran.y = tcp_posbd.tran.y; cart.tran.z = tcp_posbd.tran.z;
         cart.rpy.rx = tcp_posbd.rpy.rx; cart.rpy.ry = tcp_posbd.rpy.ry; cart.rpy.rz = tcp_posbd.rpy.rz;
         i = JKrobot.linear_move(&cart, MoveMode::ABS, TRUE, sysparaset.linearspeed);

         refP.jVal[0] =-270 * PI / 180; refP.jVal[1] = 0 * PI / 180; refP.jVal[2] = 152 * PI / 180;
         refP.jVal[3] = 120 * PI / 180; refP.jVal[4] = 0 * PI / 180; refP.jVal[5] = 0 * PI / 180;
         i = JKrobot.joint_move(&refP, MoveMode::ABS, TRUE, sysparaset.angularspeed);
         return (0);
     }


     /***************************
     ******相机平面矫正************
     ***************************/
     int camerafix()
     {

         Matrix3f m;

         MatrixXf k(3,1),n(3,1);

         m = In_parameter.inverse();


         //相机参数设置
         Delay_MSec(1000);
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


            //设置限定参数
            int differ_flag = true;
            int differ_x, differ_y ;
            float differ_z1 , differ_z2 , differ_z3 ;

            //设置相关参数
            float sz[3][4];
            double sum1=0,sum2=0,sum3=0,sum4=0,sum5=0,sum6=0,sum7=0,sum8=0,sum9=0,sum10=0,sum11=0,sum12=0;

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
                            if (!src.data)
                            {
                             qDebug("could not load image...\n");
                             return -1;
                            }

                            DetectRect detectRect(src);
                            detectRect.printCorners();










































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
                            float dist_to_center5;

                            float dist_to_center11;
                            float dist_to_center12;
                            float dist_to_center13;
                            float dist_to_center14;
                            float dist_to_center15;

                            float dist_to_center21;
                            float dist_to_center22;
                            float dist_to_center23;
                            float dist_to_center24;
                            float dist_to_center25;
                            for ( i = 0; i < circles.size(); i++)
                            {
                                //参数定义
                                Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
                                int radius_r = cvRound(circles[i][2]);
                                circle(hsv, center, 1, Scalar(255, 0, 0), -1, 8, 0);

                                //绘制圆轮廓
                                circle(hsv, center, radius_r, Scalar(255, 0, 0), 2, 8, 0);

                                qDebug("x = %d,y = %d，r = %d, i=%d", cvRound(circles[i][0]), cvRound(circles[i][1]),radius_r,i);

                                //测距算出实际直径进行3圆定位


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

                                double rd = dist_to_center*1000 * ( 2 * radius_r ) / 920.81640625;

                                qDebug("rd=%f",rd);

                                if( rd>=13.5&&rd<=16.5)

                                    break;

                            }

                            qDebug("x = %d,y = %d, i=%d", cvRound(circles[i][0]), cvRound(circles[i][1]),i);

                                  //imshow("0",hsv);

                            //int x1 = cvRound(circles[i][0]);
                            //int y1 = cvRound(circles[i][1]);

                            int x1 = 640;
                            int y1 = 360;


                            n(0, 0) = cvRound(circles[i][0]);
                            n(1, 0) = cvRound(circles[i][1]);
                            n(2, 0) = 1.0;


                            k = m * (dist_to_center*1000 * n);


                            double k1, k2, k3;
                            k1 = k(0, 0);
                            k2 = k(1, 0);
                            k3 = k(2, 0);




                            GetRobotPosition(&tcp_pos.tran.x, &tcp_pos.tran.y, &tcp_pos.tran.z, &tcp_pos.rpy.rx, &tcp_pos.rpy.ry, &tcp_pos.rpy.rz);



                            cart.tran.x = tcp_pos.tran.x + 480 - dist_to_center*1000;; cart.tran.y = tcp_pos.tran.y + k1; cart.tran.z = tcp_pos.tran.z - k2;
                            cart.rpy.rx = tcp_pos.rpy.rx; cart.rpy.ry = tcp_pos.rpy.ry; cart.rpy.rz = tcp_pos.rpy.rz;
                            JKrobot.linear_move(&cart, MoveMode::ABS, TRUE, sysparaset.linearspeed);


















                            //调整结束拍照看误差是否达到可跳出
                            float sz1[3][4];
                            double sum13=0,sum23=0,sum33=0,sum43=0,sum53=0,sum63=0,sum73=0,sum83=0,sum93=0,sum103=0,sum113=0,sum123=0;

                            rs2::frameset data= pipe1.wait_for_frames();
                            rs2::depth_frame depth_frame = data.get_depth_frame();
                            rs2::depth_frame filtered = depth_frame;
                            filtered = thr_filter.process(filtered);
                            filtered = depth_to_disparity.process(filtered);
                            filtered = hole_filter.process(filtered);
                            filtered = disparity_to_depth.process(filtered);
                            filtered = spat_filter.process(filtered);
                            filtered = tem_filter.process(filtered);
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

                            if (!src1.data)
                            {
                             qDebug("could not load image...\n");
                             return -1;
                            }

                            Rect rect1(x1-100, y1-100, 300, 300);

                            Mat img1 = src1(rect1);

                                cvtColor(img1, hsv2, COLOR_BGR2GRAY);

                                GaussianBlur(hsv2, hsv2, Size(9, 9), 2, 2);


                                vector<Vec3f> circles2;//进行霍夫圆变换
                                HoughCircles(hsv2, circles2, CV_HOUGH_GRADIENT, 1, 10, 80, 40, 0, 0);
                                int radius3 = cvRound(circles2[0][2]);
                                int radius4 = cvRound(circles2[1][2]);
                                int radius5 = cvRound(circles2[2][2]);

                                for (size_t i = 0; i < circles2.size(); i++)
                                {
                                    //参数定义
                                    Point center(cvRound(circles2[i][0]), cvRound(circles2[i][1]));
                                    int radius = cvRound(circles2[i][2]);
                                    //sz.push_back(radius);
                                    circle(hsv2, center, 1, Scalar(255, 0, 0), -1, 8, 0);

                                    //绘制圆轮廓
                                    circle(hsv2, center, radius, Scalar(255, 0, 0), 2, 8, 0);
                                }

                                //imshow("2",hsv2);
                                Delay_MSec(1000);

                                filtered = thr_filter.process(filtered);
                                filtered = depth_to_disparity.process(filtered);
                                filtered = hole_filter.process(filtered);
                                filtered = disparity_to_depth.process(filtered);
                                filtered = spat_filter.process(filtered);
                                filtered = tem_filter.process(filtered);

                                dist_to_center = filtered.get_distance(cvRound(circles2[0][0]) + x1-100, cvRound(circles2[0][1]) + y1-100);
                                dist_to_center1 = filtered.get_distance(cvRound(circles2[0][0]) + x1-100-1, cvRound(circles2[0][1]) + y1-100-1);
                                dist_to_center2 = filtered.get_distance(cvRound(circles2[0][0]) + x1-100-1, cvRound(circles2[0][1]) + y1-100+1);
                                dist_to_center3 = filtered.get_distance(cvRound(circles2[0][0]) + x1-100+1, cvRound(circles2[0][1]) + y1-100-1);
                                dist_to_center4 = filtered.get_distance(cvRound(circles2[0][0]) + x1-100+1, cvRound(circles2[0][1]) + y1-100+1);
                                dist_to_center3 = (dist_to_center+dist_to_center1+dist_to_center2+dist_to_center3+dist_to_center4)/5;



                                dist_to_center11 = filtered.get_distance(cvRound(circles2[1][0]) + x1-100, cvRound(circles2[1][1]) + y1-100);
                                dist_to_center12 = filtered.get_distance(cvRound(circles2[1][0]) + x1-100-1, cvRound(circles2[1][1]) + y1-100-1);
                                dist_to_center13 = filtered.get_distance(cvRound(circles2[1][0]) + x1-100-1, cvRound(circles2[1][1]) + y1-100+1);
                                dist_to_center14 = filtered.get_distance(cvRound(circles2[1][0]) + x1-100+1, cvRound(circles2[1][1]) + y1-100-1);
                                dist_to_center15 = filtered.get_distance(cvRound(circles2[1][0]) + x1-100+1, cvRound(circles2[1][1]) + y1-100+1);
                                dist_to_center4 = (dist_to_center11+dist_to_center12+dist_to_center13+dist_to_center14+dist_to_center15)/5;

                                dist_to_center21 = filtered.get_distance(cvRound(circles2[2][0]) + x1-100, cvRound(circles2[2][1]) + y1-100);
                                dist_to_center22 = filtered.get_distance(cvRound(circles2[2][0]) + x1-100-1, cvRound(circles2[2][1]) + y1-100-1);
                                dist_to_center23 = filtered.get_distance(cvRound(circles2[2][0]) + x1-100-1, cvRound(circles2[2][1]) + y1-100+1);
                                dist_to_center24 = filtered.get_distance(cvRound(circles2[2][0]) + x1-100+1, cvRound(circles2[2][1]) + y1-100-1);
                                dist_to_center25 = filtered.get_distance(cvRound(circles2[2][0]) + x1-100+1, cvRound(circles2[2][1]) + y1-100+1);
                                dist_to_center5 = (dist_to_center21+dist_to_center22+dist_to_center23+dist_to_center24+dist_to_center25)/5;




                                           vector<int>sy1;
                                           sy1.push_back(radius3);
                                           sy1.push_back(radius4);
                                           sy1.push_back(radius5);
                                           sort(sy1.begin(),sy1.end());
                                           if(sy1[0]==radius3)
                                           {
                                               sz1[0][0]=cvRound(circles2[0][0]);sz1[0][1]=cvRound(circles2[0][1]);sz1[0][2]=radius3;sz1[0][3]=dist_to_center3 * 1000;
                                           }
                                           else if(sy1[0]==radius4)
                                           {
                                               sz1[0][0]=cvRound(circles2[1][0]);sz1[0][1]=cvRound(circles2[1][1]);sz1[0][2]=radius4;sz1[0][3]=dist_to_center4 * 1000;
                                           }
                                           else
                                           {
                                               sz1[0][0]=cvRound(circles2[2][0]);sz1[0][1]=cvRound(circles2[2][1]);sz1[0][2]=radius5;sz1[0][3]=dist_to_center5 * 1000;
                                           }
                                           if(sy1[1]==radius3)
                                           {
                                               sz1[1][0]=cvRound(circles2[0][0]);sz1[1][1]=cvRound(circles2[0][1]);sz1[1][2]=radius3;sz1[1][3]=dist_to_center3 * 1000;
                                           }
                                           else if(sy1[1]==radius4)
                                           {
                                               sz1[1][0]=cvRound(circles2[1][0]);sz1[1][1]=cvRound(circles2[1][1]);sz1[1][2]=radius4;sz1[1][3]=dist_to_center4 * 1000;
                                           }
                                           else
                                           {
                                               sz1[1][0]=cvRound(circles2[2][0]);sz1[1][1]=cvRound(circles2[2][1]);sz1[1][2]=radius5;sz1[1][3]=dist_to_center5 * 1000;
                                           }
                                           if(sy1[2]==radius3)
                                           {
                                               sz1[2][0]=cvRound(circles2[0][0]);sz1[2][1]=cvRound(circles2[0][1]);sz1[2][2]=radius3;sz1[2][3]=dist_to_center3 * 1000;
                                           }
                                           else if(sy1[2]==radius4)
                                           {
                                               sz1[2][0]=cvRound(circles2[1][0]);sz1[2][1]=cvRound(circles2[1][1]);sz1[2][2]=radius4;sz1[2][3]=dist_to_center4 * 1000;
                                           }
                                           else
                                           {
                                               sz1[2][0]=cvRound(circles2[2][0]);sz1[2][1]=cvRound(circles2[2][1]);sz1[2][2]=radius5;sz1[2][3]=dist_to_center5 * 1000;
                                           }


                                           sum13=sz1[0][0],sum23=sz1[0][1],sum33=sz1[0][2],sum43=sz1[0][3],
                                           sum53=sz1[1][0],sum63=sz1[1][1],sum73=sz1[1][2],sum83=sz1[1][3],
                                           sum93=sz1[2][0],sum103=sz1[2][1],sum113=sz1[2][2],sum123=sz1[2][3];

                                          qDebug("x = %f,y = %f，r = %f,d = %f\n", sum13, sum23,sum33,sum43);
                                          qDebug("x = %f,y = %f，r = %f,d = %f\n", sum53, sum63,sum73,sum83);
                                          qDebug("x = %f,y = %f，r = %f,d = %f\n", sum93, sum103,sum113,sum123);
                                          differ_x = fabs (sum13-sum93);
                                          differ_y = fabs (sum23-sum63);
                                          differ_z1 = fabs (sum43 - sum83);
                                          differ_z2 = fabs (sum43-sum123);
                                          differ_z3 = fabs (sum83-sum123);
                                          differ_flag = (differ_x>2)||(differ_y>2)||(differ_z1>sysparaset.errthreshold)||(differ_z2>sysparaset.errthreshold)||(differ_z3>sysparaset.errthreshold);
                                          qDebug("x=%d,y=%d,z1=%f,z2=%f,z3=%f",differ_x,differ_y,differ_z1,differ_z2,differ_z3);
                                          qDebug("d=%d",differ_flag) ;
                                          qDebug("j=%d",j);


                                          if(j==11||differ_flag==0)
                                              break;




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
                                    std::cout << "Saved " << png_file.str() << std::endl;
                                }
                            }

                            //读取src
                            src = imread("/dev/shm/rs-save-to-disk-output-Color.png");
                            if (!src.data)
                            {
                                qDebug("could not load image...\n");
                                return -1;
                            }


                        Rect rect(x1-100, y1-100, 300, 300);

                        Mat img = src(rect);

                            cvtColor(img, hsv1, COLOR_BGR2GRAY);

                            GaussianBlur(hsv1, hsv1, Size(9, 9), 2, 2);


                            vector<Vec3f> circles1;//进行霍夫圆变换
                            HoughCircles(hsv1, circles1, CV_HOUGH_GRADIENT, 1, 10, 80, 40, 0, 0);
                            int radius = cvRound(circles1[0][2]);
                            int radius1 = cvRound(circles1[1][2]);
                            int radius2 = cvRound(circles1[2][2]);

                            for (size_t i = 0; i < circles1.size(); i++)
                            {
                                //参数定义
                                Point center(cvRound(circles1[i][0]), cvRound(circles1[i][1]));
                                int radius_r = cvRound(circles1[i][2]);
                                circle(hsv1, center, 1, Scalar(255, 0, 0), -1, 8, 0);

                                //绘制圆轮廓
                                circle(hsv1, center, radius_r, Scalar(255, 0, 0), 2, 8, 0);

                            }
                    //imshow("1",hsv1);
                            //测距


                    Delay_MSec(1000);





                                dist_to_center = filtered.get_distance(cvRound(circles1[0][0]) + x1-100, cvRound(circles1[0][1]) + y1-100);
                                dist_to_center1 = filtered.get_distance(cvRound(circles1[0][0]) + x1-100-1, cvRound(circles1[0][1]) + y1-100-1);
                                dist_to_center2 = filtered.get_distance(cvRound(circles1[0][0]) + x1-100-1, cvRound(circles1[0][1]) + y1-100+1);
                                dist_to_center3 = filtered.get_distance(cvRound(circles1[0][0]) + x1-100+1, cvRound(circles1[0][1]) + y1-100-1);
                                dist_to_center4 = filtered.get_distance(cvRound(circles1[0][0]) + x1-100+1, cvRound(circles1[0][1]) + y1-100+1);
                                dist_to_center = (dist_to_center+dist_to_center1+dist_to_center2+dist_to_center3+dist_to_center4)/5;

                                dist_to_center11 = filtered.get_distance(cvRound(circles1[1][0]) + x1-100, cvRound(circles1[1][1]) + y1-100);
                                dist_to_center12 = filtered.get_distance(cvRound(circles1[1][0]) + x1-100-1, cvRound(circles1[1][1]) + y1-100-1);
                                dist_to_center13 = filtered.get_distance(cvRound(circles1[1][0]) + x1-100-1, cvRound(circles1[1][1]) + y1-100+1);
                                dist_to_center14 = filtered.get_distance(cvRound(circles1[1][0]) + x1-100+1, cvRound(circles1[1][1]) + y1-100-1);
                                dist_to_center15 = filtered.get_distance(cvRound(circles1[1][0]) + x1-100+1, cvRound(circles1[1][1]) + y1-100+1);
                                dist_to_center1 = (dist_to_center11+dist_to_center12+dist_to_center13+dist_to_center14+dist_to_center15)/5;

                                dist_to_center21 = filtered.get_distance(cvRound(circles1[2][0]) + x1-100, cvRound(circles1[2][1]) + y1-100);
                                dist_to_center22 = filtered.get_distance(cvRound(circles1[2][0]) + x1-100-1, cvRound(circles1[2][1]) + y1-100-1);
                                dist_to_center23 = filtered.get_distance(cvRound(circles1[2][0]) + x1-100-1, cvRound(circles1[2][1]) + y1-100+1);
                                dist_to_center24 = filtered.get_distance(cvRound(circles1[2][0]) + x1-100+1, cvRound(circles1[2][1]) + y1-100-1);
                                dist_to_center25 = filtered.get_distance(cvRound(circles1[2][0]) + x1-100+1, cvRound(circles1[2][1]) + y1-100+1);
                                dist_to_center2 = (dist_to_center21+dist_to_center22+dist_to_center23+dist_to_center24+dist_to_center25)/5;

                            //三个圆的排序
                            vector<double>sy;
                            sy.push_back(radius);
                            sy.push_back(radius1);
                            sy.push_back(radius2);
                            sort(sy.begin(),sy.end());

                            if(sy[0]==radius)
                            {
                                sz[0][0]=cvRound(circles1[0][0]);sz[0][1]=cvRound(circles1[0][1]);sz[0][2]=radius;sz[0][3]=dist_to_center * 1000;
                            }
                            else if(sy[0]==radius1)
                            {
                                sz[0][0]=cvRound(circles1[1][0]);sz[0][1]=cvRound(circles1[1][1]);sz[0][2]=radius1;sz[0][3]=dist_to_center1 * 1000;
                            }
                            else
                            {
                                 sz[0][0]=cvRound(circles1[2][0]);sz[0][1]=cvRound(circles1[2][1]);sz[0][2]=radius2;sz[0][3]=dist_to_center2 * 1000;
                            }
                            if(sy[1]==radius)
                            {
                                sz[1][0]=cvRound(circles1[0][0]);sz[1][1]=cvRound(circles1[0][1]);sz[1][2]=radius;sz[1][3]=dist_to_center * 1000;
                            }
                            else if(sy[1]==radius1)
                            {
                                sz[1][0]=cvRound(circles1[1][0]);sz[1][1]=cvRound(circles1[1][1]);sz[1][2]=radius1;sz[1][3]=dist_to_center1 * 1000;
                            }
                            else
                            {
                                 sz[1][0]=cvRound(circles1[2][0]);sz[1][1]=cvRound(circles1[2][1]);sz[1][2]=radius2;sz[1][3]=dist_to_center2 * 1000;
                            }
                            if(sy[2]==radius)
                            {
                                sz[2][0]=cvRound(circles1[0][0]);sz[2][1]=cvRound(circles1[0][1]);sz[2][2]=radius;sz[2][3]=dist_to_center * 1000;
                            }
                            else if(sy[2]==radius1)
                            {
                                sz[2][0]=cvRound(circles1[1][0]);sz[2][1]=cvRound(circles1[1][1]);sz[2][2]=radius1;sz[2][3]=dist_to_center1 * 1000;
                            }
                            else
                            {
                                 sz[2][0]=cvRound(circles1[2][0]);sz[2][1]=cvRound(circles1[2][1]);sz[2][2]=radius2;sz[2][3]=dist_to_center2 * 1000;
                            }


                            sum1=sz[0][0],sum2=sz[0][1],sum3=sz[0][2],sum4=sz[0][3],
                            sum5=sz[1][0],sum6=sz[1][1],sum7=sz[1][2],sum8=sz[1][3],
                            sum9=sz[2][0],sum10=sz[2][1],sum11=sz[2][2],sum12=sz[2][3];

                           qDebug("x = %f,y = %f，r = %f,d = %f\n", sum1, sum2,sum3,sum4);
                           qDebug("x = %f,y = %f，r = %f,d = %f\n", sum5, sum6,sum7,sum8);
                           qDebug("x = %f,y = %f，r = %f,d = %f\n", sum9, sum10,sum11,sum12);

                           //旋转角度计算与机械臂执行
                           double  u1=sum1 + x1-100, v1=sum2 + y1-100,
                                   u2=sum5 + x1-100, v2=sum6 + y1-100,
                                   u3=sum9 + x1-100, v3=sum10 + y1-100;

                           double X1, Y1, X2, Y2, X3, Y3;

                           X1 = sum4 * (u1 - 630.4161376953125) / 920.81640625; Y1 = sum4 * (v1 - 373.34466552734375) / 919.99462890625;
                           X2 = sum8 * (u2 - 630.4161376953125) / 920.81640625; Y2 = sum8 * (v2 - 373.34466552734375) / 919.99462890625;
                           X3 = sum12 * (u3 - 630.4161376953125) / 920.81640625; Y3 = sum12 * (v3 - 373.34466552734375) / 919.99462890625;


                           double d12 = sum4 - sum8;   //1-2
                           double d13 = sum4 - sum12;   //1-3
                           double x12 = X1 - X2;               //1-2
                           double y12 = Y1 - Y2;               //1-2
                           double y13 = Y1 - Y3;               //1-3

                           double v12=v1-v2,u12=u1-u2;
                           double angle4 = atan(d13 / y13);
                           double angle5 = atan(d12 / x12);
                           double angle6 = atan(v12 / u12);

                           qDebug("angle6=%g,angle5=%g,angle4=%g\n",angle6/2,angle5/2,angle4/2);

                           //if((angle6/2>0.00087)||(angle6/2<-0.00087))
                           //{


                           JKrobot.disable_robot();
                           Delay_MSec(2000);
                           JKrobot.enable_robot();
                           Delay_MSec(2000);

                           JKrobot.jog(5, INCR, COORD_TOOL,5,angle6);
                           Delay_MSec(5000);
                           JKrobot.jog(4, INCR, COORD_TOOL,5,-angle4);
                           Delay_MSec(5000);
                           JKrobot.jog(3, INCR, COORD_TOOL,5,-angle5);
                           Delay_MSec(5000);

                           JKrobot.jog_stop(0);

                               //JointValue joint_pos6 = { 0 * PI / 180, 0 * PI / 180, 0 * PI / 180, 0 , 0 , angle6 };
                               //JKrobot.joint_move(&joint_pos6, INCR, TRUE, 0.1);
                               //JKrobot.jog(5, INCR, COORD_TOOL,5, angle6);
                               //Delay_MSec(3000);
                           //}
                           //if((angle5/2>0.00087)||(angle5/2<-0.00087))
                           //{
                               //JointValue joint_pos5 = { 0 * PI / 180, 0 * PI / 180, 0 * PI / 180, 0 , -angle5 , 0 };
                               //JKrobot.joint_move(&joint_pos5, INCR, TRUE, 0.1);
                               //JKrobot.jog(3, INCR, COORD_TOOL,5, -angle5);
                               //Delay_MSec(3000);
                           //}

                           //if((angle4/2>0.00087)||(angle4/2<-0.00087))
                           //{
                               //JointValue joint_pos4 = { 0 * PI / 180, 0 * PI / 180, 0 * PI / 180, angle4/2 , 0 , 0};
                               //JKrobot.joint_move(&joint_pos4, INCR, TRUE, 0.1);
                               //JKrobot.jog(4, INCR, COORD_TOOL,5,angle4);
                               //Delay_MSec(2000);
                               //JKrobot.jog_stop(0);
                           //}



                    GetRobotPosition (&tcp_posbd.tran.x,&tcp_posbd.tran.y,&tcp_posbd.tran.z,&tcp_posbd.rpy.rx,&tcp_posbd.rpy.ry,&tcp_posbd.rpy.rz);

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
                    if (!src.data)
                    {
                     qDebug("could not load image...\n");
                     return -1;
                    }

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
                        //参数定义
                        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
                        int radius_r = cvRound(circles[i][2]);
                        circle(hsv, center, 1, Scalar(255, 0, 0), -1, 8, 0);

                        //绘制圆轮廓
                        circle(hsv, center, radius_r, Scalar(255, 0, 0), 2, 8, 0);

                        qDebug("x = %d,y = %d，r = %d, i=%d", cvRound(circles[i][0]), cvRound(circles[i][1]),radius_r,i);

                        //测距算出实际直径进行3圆定位


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

                        double rd = dist_to_center*1000 * ( 2 * radius_r ) / 920.81640625;

                        qDebug("rd=%f",rd);

                        if( rd>=13.5&&rd<=16.5)

                            break;

                    }

                    qDebug("x = %d,y = %d, i=%d", cvRound(circles[i][0]), cvRound(circles[i][1]),i);

                          //imshow("0",hsv);

                    //int x1 = cvRound(circles[i][0]);
                    //int y1 = cvRound(circles[i][1]);

                    int x1 = 640;
                    int y1 = 360;


                    n(0, 0) = cvRound(circles[i][0]);
                    n(1, 0) = cvRound(circles[i][1]);
                    n(2, 0) = 1.0;


                    k = m * (dist_to_center*1000 * n);


                    double k1, k2, k3;
                    k1 = k(0, 0);
                    k2 = k(1, 0);
                    k3 = k(2, 0);




                    GetRobotPosition(&tcp_pos.tran.x, &tcp_pos.tran.y, &tcp_pos.tran.z, &tcp_pos.rpy.rx, &tcp_pos.rpy.ry, &tcp_pos.rpy.rz);



                    cart.tran.x = tcp_pos.tran.x + 480 - dist_to_center*1000;; cart.tran.y = tcp_pos.tran.y + k1; cart.tran.z = tcp_pos.tran.z - k2;
                    cart.rpy.rx = tcp_pos.rpy.rx; cart.rpy.ry = tcp_pos.rpy.ry; cart.rpy.rz = tcp_pos.rpy.rz;
                    JKrobot.linear_move(&cart, MoveMode::ABS, TRUE, sysparaset.linearspeed);


                    GetRobotPosition (&tcp_posbd.tran.x,&tcp_posbd.tran.y,&tcp_posbd.tran.z,&tcp_posbd.rpy.rx,&tcp_posbd.rpy.ry,&tcp_posbd.rpy.rz);

                        //if (j<10)
                          return (0);//成功

                         //else
                          //return (2);//失败

                 }


                 int move2calibrationpos(int equidx)
                 {
                     //JKrobot.jog(4, INCR, COORD_TOOL,5,-10* PI / 180);
                     //Delay_MSec(5000);
                     //JKrobot.jog_stop(0);
                      //JKrobot.jog(3, INCR, COORD_TOOL,5,10* PI / 180);
                     //JointValue joint_pos6 = { 0 * PI / 180, 0 * PI / 180, 0 * PI / 180, 0 , 0 , -10 * PI / 180 };
                     //JKrobot.joint_move(&joint_pos6, INCR, TRUE, 0.1);
                    //Delay_MSec(5000);
                     //JKrobot.jog(5, INCR, COORD_TOOL,5,10* PI / 180);
                     //JointValue joint_pos5 = { 0 * PI / 180, 0 * PI / 180, 0 * PI / 180, 0 , 10 * PI / 180 , 0 };
                     //JKrobot.joint_move(&joint_pos5, INCR, TRUE, 0.1);
                     //Delay_MSec(5000);
//JKrobot.jog(4, INCR,/ COORD_TOOL,5,-10* PI / 180);
//Delay_MSec(5000)
                     //return (0);




                     //int i=10;
                      static int inpos=1;
                      refP.jVal[0] = -90 * PI / 180; refP.jVal[1] = 0 * PI / 180; refP.jVal[2] = 152 * PI / 180;
                      refP.jVal[3] = 120 * PI / 180; refP.jVal[4] = 0 * PI / 180; refP.jVal[5] = 0 * PI / 180;
                      JKrobot.joint_move(&refP, MoveMode::ABS, TRUE, sysparaset.angularspeed);

                      refP.jVal[0] = -90 * PI / 180; refP.jVal[1] = 0 * PI / 180; refP.jVal[2] = -11 * PI / 180;
                      refP.jVal[3] = 120 * PI / 180; refP.jVal[4] = 0 * PI / 180; refP.jVal[5] = 0 * PI / 180;
                      JKrobot.joint_move(&refP, MoveMode::ABS, TRUE, sysparaset.angularspeed);

                      refP.jVal[0] = -90 * PI / 180; refP.jVal[1] = 21.508 * PI / 180; refP.jVal[2] = -136.859 * PI / 180;
                      refP.jVal[3] = 115.722 * PI / 180; refP.jVal[4] = -90 * PI / 180; refP.jVal[5] = -90 * PI / 180;
                      JKrobot.joint_move(&refP, MoveMode::ABS, TRUE, sysparaset.angularspeed);

                      refP.jVal[0] = -180 * PI / 180; refP.jVal[1] = 21.508 * PI / 180; refP.jVal[2] = -136.859 * PI / 180;
                      refP.jVal[3] = 115.722 * PI / 180; refP.jVal[4] = -90 * PI / 180; refP.jVal[5] = -90 * PI / 180;
                      JKrobot.joint_move(&refP, MoveMode::ABS, TRUE, sysparaset.angularspeed);

                      /****cart.tran.x = -360.502; cart.tran.y = 0; cart.tran.z = -250;
                      cart.rpy.rx = 1.069 * PI / 180; cart.rpy.ry = 89.612 * PI / 180; cart.rpy.rz = -178.931 * PI / 180;
                      JKrobot.linear_move(&cart, MoveMode::ABS, TRUE, sysparaset.linearspeed);***/
                      /***************
                      inpos=1;
                      while(!(inpos==0&&Jkstatus.inpos==1))
                      {
                          inpos=Jkstatus.inpos;
                      }
                      *******************/
                      /****cart.tran.x = 0; cart.tran.y = 0; cart.tran.z = equparaset[equidx]. calibration_dz;
                      cart.rpy.rx = 0 * PI / 180; cart.rpy.ry = 0 * PI / 180; cart.rpy.rz = 0 * PI / 180;
                      JKrobot.linear_move(&cart, MoveMode::INCR, TRUE, sysparaset.linearspeed);**/
                      cart.tran.x = -330; cart.tran.y = 0; cart.tran.z = equparaset[equidx]. calibration_dz - 166.219;
                      cart.rpy.rx = 0 * PI / 180; cart.rpy.ry = 90 * PI / 180; cart.rpy.rz = -180 * PI / 180;
                      //cart.tran.x = -321.273; cart.tran.y = -5.198; cart.tran.z = equparaset[equidx]. calibration_dz - 166.219;
                      //cart.rpy.rx = 130.042 * PI / 180; cart.rpy.ry = 82.488 * PI / 180; cart.rpy.rz = -50.707 * PI / 180;
                      JKrobot.linear_move(&cart, MoveMode::ABS, TRUE, sysparaset.linearspeed);
                      tight();                              //  电抓手夹紧初始化



                      //inpos=1;
                      //while(!(inpos==0&&Jkstatus.inpos==1))
                      //{
                        //  inpos=Jkstatus.inpos;
                      //}

                      //Delay_MSec(30000);



                      //while (i!=0);
                        //Delay_MSec(30000);

                      //GetRobotPosition (&tcp_posbd.tran.x,&tcp_posbd.tran.y,&tcp_posbd.tran.z,&tcp_posbd.rpy.rx,&tcp_posbd.rpy.ry,&tcp_posbd.rpy.rz);
                      //int b = 0;

                      int b = camerafix();                                                                  // 相机平面校准
                      if (b==0)
                      return (0);//成功
                      else
                      return (2);//失败

                 }


                 int button(int kkidx, int state, bool num)
                 {

                     //相机内参矩阵
                     /*************
                     Matrix3f In_parameter,m;
                     Matrix3f In_parameter,m;
                     In_parameter(0,0) = 920.81640625;
                     In_parameter(0,1) = 0.0;
                     In_parameter(0,2) = 630.4161376953125;
                     In_parameter(1,0) = 0.0;
                     In_parameter(1,1) = 919.99462890625;
                     In_parameter(1,2) = 373.34466552734375;
                     In_parameter(2,0) = 0.0;
                     In_parameter(2,1) = 0.0;
                     In_parameter(2,2) = 1.0;
                     ****************/
                     Matrix3f m;
                     MatrixXf k(3,1),n(3,1);

                     m = In_parameter.inverse();


                     /***if(num==0)
                     {
                     cart.tran.x = 0; cart.tran.y = kkparaset[kkidx].dy ; cart.tran.z = kkparaset[kkidx].dz;
                     cart.rpy.rx = 0; cart.rpy.ry = 0; cart.rpy.rz = 0;
                     JKrobot.linear_move(&cart, MoveMode::INCR, TRUE, 30);
                     }
                     else
                     {
                         GetRobotPosition (&tcp_pos.tran.x,&tcp_pos.tran.y,&tcp_pos.tran.z,&tcp_pos.rpy.rx,&tcp_pos.rpy.ry,&tcp_pos.rpy.rz);
                         cart.tran.x = -360  ; cart.tran.y = 60 + kkparaset[kkidx].dy ; cart.tran.z = 740 + kkparaset[kkidx].dz;
                         cart.rpy.rx = tcp_pos.rpy.rx ; cart.rpy.ry = tcp_pos.rpy.ry; cart.rpy.rz = tcp_pos.rpy.rz;
                         JKrobot.linear_move(&cart, MoveMode::ABS, TRUE, 30);
                     }***/

                     //cart.tran.x = tcp_posbd.tran.x ; cart.tran.y = tcp_posbd.tran.y +kkparaset[kkidx].dy; cart.tran.z = tcp_posbd.tran.z+kkparaset[kkidx].dz;
                     //cart.rpy.rx = tcp_posbd.rpy.rx ; cart.rpy.ry = tcp_posbd.rpy.ry; cart.rpy.rz = tcp_posbd.rpy.rz;
                     //JKrobot.linear_move(&cart, MoveMode::ABS, TRUE, sysparaset.linearspeed);

                     cart.tran.x = tcp_posbd.tran.x ; cart.tran.y = tcp_posbd.tran.y ; cart.tran.z = tcp_posbd.tran.z+kkparaset[kkidx].dz;
                     cart.rpy.rx = tcp_posbd.rpy.rx ; cart.rpy.ry = tcp_posbd.rpy.ry; cart.rpy.rz = tcp_posbd.rpy.rz;
                     JKrobot.linear_move(&cart, MoveMode::ABS, TRUE, sysparaset.linearspeed);

                     JKrobot.jog(1,INCR,COORD_TOOL,40,-kkparaset[kkidx].dy);

                     Delay_MSec(2000);
                     JKrobot.jog_stop(0);




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




                     //拍照
                     float sz[1][3];
                         double sum1=0,sum2=0,sum3=0;
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

                         if (!src.data)
                         {
                          qDebug("could not load image...\n");
                          return -1;
                         }

                         Rect rect(kkparaset[kkidx].pix_x, kkparaset[kkidx].pix_y, kkparaset[kkidx].pix_w, kkparaset[kkidx].pix_h);


                         Mat img = src(rect);


                             cvtColor(img, hsv, COLOR_BGR2HSV);//RGB转HSV


                             inRange(hsv, Scalar(kkparaset[kkidx].color_value1, kkparaset[kkidx].color_value2, kkparaset[kkidx].color_value3), Scalar(kkparaset[kkidx].color_value4, kkparaset[kkidx].color_value5, kkparaset[kkidx].color_value6), ys1);
                             inRange(hsv, Scalar(kkparaset[kkidx].color_value7, kkparaset[kkidx].color_value8, kkparaset[kkidx].color_value9), Scalar(kkparaset[kkidx].color_value10, kkparaset[kkidx].color_value11, kkparaset[kkidx].color_value12), ys2);
                             addWeighted(ys1, 1, ys2, 1, 0, ys3);


                             Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
                             morphologyEx(ys3, result, MORPH_OPEN, element);//形态学开运算降噪


                             vector<vector<Point> > contours1;//找轮廓
                             cv::findContours(result, contours1, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
                             vector<vector<Point>> contours2;

                             for (int i = 0; i < contours1.size(); ++i)
                             {
                                 if (contours1[i].size() > 10)//将比较小的轮廓剔除掉
                                 {
                                     contours2.push_back(contours1[i]);
                                 }
                             }

                             Mat hole(result.size(), CV_8U, Scalar(0)); //遮罩图层
                             cv::drawContours(hole, contours2, -1, Scalar(255), CV_FILLED); //在遮罩图层上，用白色像素填充轮廓


                             GaussianBlur(hole, hole, Size(9, 9), 2, 2);


                             vector<Vec3f> circles;//进行霍夫圆变换
                             HoughCircles(hole, circles, CV_HOUGH_GRADIENT, 1, 10, 80, 30, 0, 0);


                             //依次在图中绘制出圆
                             for (size_t i = 0; i < circles.size(); i++)
                             {
                                 //参数定义
                                 Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
                                 int radius = cvRound(circles[i][2]);

                                 //绘制圆心
                                 circle(hole, center, 1, Scalar(0, 255, 0), -1, 8, 0);

                                 //绘制圆轮廓
                                 circle(hole, center, radius, Scalar(160, 0, 0), 1, 8, 0);

                                 //打印圆心坐标
                                 qDebug("x = %d,y = %d\n", cvRound(circles[i][0]) + kkparaset[kkidx].pix_x, cvRound(circles[i][1])+ kkparaset[kkidx].pix_y);
                             }

                             //imshow("截取", img);
                             //imshow("hsv", hsv);
                             //imshow("green", green);
                             //imshow("jiang", result);
                             //imshow("遮罩图层", hole);



                            //操作灭（打开灯）/亮（关闭灯）

                             float g=0;
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
                                 dist_to_center = filtered.get_distance(cvRound(circles[0][0])+ kkparaset[kkidx].pix_x , cvRound(circles[0][1])+ kkparaset[kkidx].pix_y );
                                 dist_to_center1 = filtered.get_distance(cvRound(circles[0][0])+ kkparaset[kkidx].pix_x-1, cvRound(circles[0][1])+ kkparaset[kkidx].pix_y-1);
                                 dist_to_center2 = filtered.get_distance(cvRound(circles[0][0])+ kkparaset[kkidx].pix_x-1, cvRound(circles[0][1])+ kkparaset[kkidx].pix_y+1);
                                 dist_to_center3 = filtered.get_distance(cvRound(circles[0][0])+ kkparaset[kkidx].pix_x+1, cvRound(circles[0][1])+ kkparaset[kkidx].pix_y-1);
                                 dist_to_center4 = filtered.get_distance(cvRound(circles[0][0])+ kkparaset[kkidx].pix_x+1, cvRound(circles[0][1])+ kkparaset[kkidx].pix_y+1);

                             dist_to_center = (dist_to_center+dist_to_center1+dist_to_center2+dist_to_center3+dist_to_center4)/5;

                             std::cout << "The camera i1s facing an object " << dist_to_center << " meters away \r"<<std::endl;


                             sz[0][0]=cvRound(circles[0][0]),sz[0][1]=cvRound(circles[0][1]),sz[0][2]=dist_to_center;

                             sum1=sz[0][0],sum2=sz[0][1],sum3=sz[0][2];

                          qDebug("x = %f , y = %f , d = %f\n" , sum1 , sum2 , sum3);


                          n(0,0)= sum1 + kkparaset[kkidx].pix_x ;
                          n(1,0)= sum2 + kkparaset[kkidx].pix_y ;
                          n(2,0)= 1.0;

                             //std::cout << n << std::endl;

                             k = m * ((sum3*1000) * n);

                             //std::cout << k << std::endl;

                             double k1,k2,k3;
                             k1=k(0,0);
                             k2=k(1,0);
                             k3=k(2,0);

                             //std::cout << k1 << std::endl;
                             //std::cout << k2 << std::endl;
                             //std::cout << k3 << std::endl;



                             GetRobotPosition (&tcp_pos.tran.x,&tcp_pos.tran.y,&tcp_pos.tran.z,&tcp_pos.rpy.rx,&tcp_pos.rpy.ry,&tcp_pos.rpy.rz);

                             qDebug("x = %g,y = %g ,z = %g\n",  tcp_pos.tran.x , tcp_pos.tran.y + k1 , tcp_pos.tran.z - k2) ;

                             //cart.tran.x = tcp_pos.tran.x  ; cart.tran.y = tcp_pos.tran.y + k1 ; cart.tran.z = tcp_pos.tran.z - k2 ;
                             //cart.rpy.rx = tcp_pos.rpy.rx ; cart.rpy.ry = tcp_pos.rpy.ry; cart.rpy.rz = tcp_pos.rpy.rz;
                             //JKrobot.linear_move(&cart, MoveMode::ABS, TRUE, sysparaset.linearspeed);


                             cart.tran.x = tcp_pos.tran.x  ; cart.tran.y = tcp_pos.tran.y  ; cart.tran.z = tcp_pos.tran.z - k2 ;
                             cart.rpy.rx = tcp_pos.rpy.rx ; cart.rpy.ry = tcp_pos.rpy.ry; cart.rpy.rz = tcp_pos.rpy.rz;
                             JKrobot.jog(1,INCR,COORD_TOOL,40,-k1);
                             Delay_MSec(5000);
                             JKrobot.jog_stop(0);
                             JKrobot.linear_move(&cart, MoveMode::ABS, TRUE, sysparaset.linearspeed);

                             Delay_MSec(2000);





                             //yz移动后第二次拍照（深度）

                             int b=0;
                             while(1)//循环
                        {
                                 b=b+1;
                                 if(b>=2)
                                     break;

                             GetRobotPosition (&tcp_pos1.tran.x,&tcp_pos1.tran.y,&tcp_pos1.tran.z,&tcp_pos1.rpy.rx,&tcp_pos1.rpy.ry,&tcp_pos1.rpy.rz);

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

                             if (!src1.data)
                             {
                              qDebug("could not load image...\n");
                              return -1;
                             }
                             Rect rect1(kkparaset[kkidx].pix_x, kkparaset[kkidx].pix_y, kkparaset[kkidx].pix_w, kkparaset[kkidx].pix_h);


                             Mat img1 = src1(rect1);
                                 cvtColor(img1, hsv1, COLOR_BGR2HSV);//RGB转HSV
                                 inRange(hsv1, Scalar(kkparaset[kkidx].color_value1, kkparaset[kkidx].color_value2, kkparaset[kkidx].color_value3), Scalar(kkparaset[kkidx].color_value4, kkparaset[kkidx].color_value5, kkparaset[kkidx].color_value6), ys11);
                                 inRange(hsv1, Scalar(kkparaset[kkidx].color_value7, kkparaset[kkidx].color_value8, kkparaset[kkidx].color_value9), Scalar(kkparaset[kkidx].color_value10, kkparaset[kkidx].color_value11, kkparaset[kkidx].color_value12), ys21);
                                 addWeighted(ys11, 1, ys21, 1, 0, ys31);

                                 morphologyEx(ys31, result1, MORPH_OPEN, element);//形态学开运算降噪

                                 vector<vector<Point> > contours11;//找轮廓
                                 cv::findContours(result1, contours11, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
                                 vector<vector<Point>> contours21;

                                 for (int i = 0; i < contours11.size(); ++i)
                                 {
                                     if (contours11[i].size() > 10)//将比较小的轮廓剔除掉
                                     {
                                         contours21.push_back(contours11[i]);
                                     }
                                 }

                                 Mat hole1(result1.size(), CV_8U, Scalar(0)); //遮罩图层
                                 cv::drawContours(hole1, contours21, -1, Scalar(255), CV_FILLED); //在遮罩图层上，用白色像素填充轮廓

                                 GaussianBlur(hole1, hole1, Size(9, 9), 2, 2);


                                 //进行霍夫圆变换
                                 vector<Vec3f> circles1;
                                 HoughCircles(hole1, circles1, CV_HOUGH_GRADIENT, 1, 10, 80, 30, 0, 0);


                                 int radius1;
                                 //依次在图中绘制出圆
                                 for (size_t i = 0; i < circles1.size(); i++)
                                 {
                                     //参数定义
                                     Point center(cvRound(circles1[i][0]), cvRound(circles1[i][1]));
                                     radius1 = cvRound(circles1[i][2]);

                                     //绘制圆心
                                     circle(hole1, center, 1, Scalar(0, 255, 0), -1, 8, 0);

                                     //绘制圆轮廓
                                     circle(hole1, center, radius1, Scalar(160, 0, 0), 2, 8, 0);

                                     //打印圆心坐标
                                     qDebug("x = %d,y = %d,r = %d\n", cvRound(circles1[i][0]) + kkparaset[kkidx].pix_x, cvRound(circles1[i][1]) + kkparaset[kkidx].pix_y, radius1);

                                 }

                                 //imshow("截取1", img1);
                                 //imshow("遮罩图层1", hole1);

                                 float dist_to_center1;
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

                                     dist_to_center11 = filtered.get_distance(cvRound(circles1[0][0]) + kkparaset[kkidx].pix_x, cvRound(circles1[0][1]) + kkparaset[kkidx].pix_y);
                                     dist_to_center12 = filtered.get_distance(cvRound(circles1[0][0]) + kkparaset[kkidx].pix_x-1, cvRound(circles1[0][1]) + kkparaset[kkidx].pix_y-1);
                                     dist_to_center13 = filtered.get_distance(cvRound(circles1[0][0]) + kkparaset[kkidx].pix_x-1, cvRound(circles1[0][1]) + kkparaset[kkidx].pix_y+1);
                                     dist_to_center14 = filtered.get_distance(cvRound(circles1[0][0]) + kkparaset[kkidx].pix_x+1, cvRound(circles1[0][1]) + kkparaset[kkidx].pix_y-1);
                                     dist_to_center15 = filtered.get_distance(cvRound(circles1[0][0]) + kkparaset[kkidx].pix_x+1, cvRound(circles1[0][1]) + kkparaset[kkidx].pix_y+1);
                                     dist_to_center1 = (dist_to_center11+dist_to_center12+dist_to_center13+dist_to_center14+dist_to_center15)/5;

                                 std::cout << "The camera i1s facing an object " << dist_to_center1*1000 << " meters away \r"<<std::endl;

                                 qDebug("x = %g,y = %g ,z = %g\n",  tcp_pos1.tran.x - dist_to_center1*1000 + sysparaset.dx + kkparaset[kkidx].locate_dx, tcp_pos1.tran.y + sysparaset.dy + kkparaset[kkidx].locate_dy, tcp_pos1.tran.z + sysparaset.dz + kkparaset[kkidx].locate_dz);

                                 Delay_MSec(1000);
                                 //cart.tran.x = 0 ; cart.tran.y = sysparaset.dy + kkparaset[kkidx].locate_dy ; cart.tran.z = sysparaset.dz + kkparaset[kkidx].locate_dz ;
                                 //cart.rpy.rx = 0 ; cart.rpy.ry = 0  ; cart.rpy.rz = 0 ;
                                 //JKrobot.linear_move(&cart, MoveMode::INCR, TRUE, sysparaset.linearspeed);

                                 cart.tran.x = 0 ; cart.tran.y = 0 ; cart.tran.z = sysparaset.dz + kkparaset[kkidx].locate_dz ;
                                 cart.rpy.rx = 0 ; cart.rpy.ry = 0  ; cart.rpy.rz = 0 ;

                                 JKrobot.jog(1,INCR,COORD_TOOL,40,-(sysparaset.dy + kkparaset[kkidx].locate_dy));
                                 Delay_MSec(5000);
                                 JKrobot.jog_stop(0);
                                 Delay_MSec(1000);
                                 JKrobot.linear_move(&cart, MoveMode::INCR, TRUE, sysparaset.linearspeed);
                                 //JKrobot.jog(1,INCR,COORD_TOOL,40,-200);
                                 Delay_MSec(5000);

                                 //JKrobot.jog_stop(0);

                                 //进入按下
                                 //Delay_MSec(1000);
                                 //cart.tran.x = - dist_to_center1*1000 + sysparaset.dx + kkparaset[kkidx].locate_dx ; cart.tran.y = 0 ; cart.tran.z = 0 ;
                                // cart.rpy.rx = 0 ; cart.rpy.ry = 0  ; cart.rpy.rz = 0 ;
                                 //JKrobot.linear_move(&cart, MoveMode::INCR, TRUE, sysparaset.linearspeed);
                                 JKrobot.jog(2,INCR,COORD_TOOL,40,200);
                                 Delay_MSec(15000);
                                 //JKrobot.jog_stop(0);
                                 //cart.tran.x = -kkparaset[kkidx].opt_x ; cart.tran.y = kkparaset[kkidx].opt_y ; cart.tran.z = kkparaset[kkidx].opt_z ;
                                 //cart.rpy.rx = 0 ; cart.rpy.ry = 0  ; cart.rpy.rz = 0 ;
                                 //JKrobot.linear_move(&cart, MoveMode::INCR, TRUE, sysparaset.linearspeed);
                                  JKrobot.jog(2,INCR,COORD_TOOL,40,kkparaset[kkidx].opt_x);
                                 Delay_MSec(1000);
                                 JKrobot.jog_stop(0);
                                 cart.tran.x = kkparaset[kkidx].opt_x ; cart.tran.y = -kkparaset[kkidx].opt_y ; cart.tran.z = -kkparaset[kkidx].opt_z ;
                                 cart.rpy.rx = 0 ; cart.rpy.ry = 0  ; cart.rpy.rz = 0 ;
                                 JKrobot.linear_move(&cart, MoveMode::INCR, TRUE, sysparaset.linearspeed);


                                 //退出到第二次拍照点再次拍照判断灯是否操作完成

                                 Delay_MSec(1000);
                                 cart.tran.x = tcp_pos1.tran.x; cart.tran.y = tcp_pos1.tran.y; cart.tran.z = tcp_pos1.tran.z;
                                 cart.rpy.rx = tcp_pos1.rpy.rx; cart.rpy.ry = tcp_pos1.rpy.ry; cart.rpy.rz = tcp_pos1.rpy.rz;
                                 JKrobot.linear_move(&cart, MoveMode::ABS, TRUE, sysparaset.linearspeed);


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
                                 src2 = imread("/dev/shm/rs-save-to-disk-output-Color.png");

                                 if (!src2.data)
                                 {
                                  qDebug("could not load image...\n");
                                  return -1;
                                 }
                                 Rect rect2(cvRound(circles1[0][0]) + kkparaset[kkidx].pix_x - radius1,cvRound(circles1[0][1]) + kkparaset[kkidx].pix_y - radius1, radius1, radius1);

                                 Mat src_ROI1 = src1(rect2);//操作前
                                 Mat src_ROI2 = src2(rect2);//操作后


                                 Vec3b BGR;
                                 int min_B;
                                 int min_G;
                                 int min_R;
                                 if (kkparaset[kkidx].color_value1==0&&kkparaset[kkidx].color_value7==156)
                                 {
                                     min_B = 0;
                                     min_G = 0;
                                     min_R = 54;
                                 }

                                  if (kkparaset[kkidx].color_value1==35&&kkparaset[kkidx].color_value7==35)
                                 {
                                     min_B = 40;
                                     min_G = 58;
                                     min_R = 0;
                                 }


                                         int num1 = 0;
                                         //按列遍历，效率更高
                                         for (int i = 0; i < src_ROI1.cols; i++)
                                         {
                                             for (int j = 0; j < src_ROI1.rows; j++)
                                             {
                                                 BGR = src_ROI1.at<Vec3b>(j, i);
                                                 if (BGR.val[0] >= min_B && BGR.val[1] >= min_G && BGR.val[2] >= min_R) {
                                                     src_ROI1.at<Vec3b>(j, i)[0] = 0;
                                                     src_ROI1.at<Vec3b>(j, i)[1] = 0;
                                                     src_ROI1.at<Vec3b>(j, i)[2] = 0;
                                                     num1++;
                                                 }

                                             }
                                         }
                                         //imshow("new_src_ROI1", src_ROI1);
                                         qDebug("num1=%d\n", num1);

                                         int num2 = 0;
                                                 //按列遍历，效率更高
                                                 for (int i = 0; i < src_ROI2.cols; i++)
                                                 {
                                                     for (int j = 0; j < src_ROI2.rows; j++)
                                                     {
                                                         BGR = src_ROI2.at<Vec3b>(j, i);
                                                         if (BGR.val[0] >= min_B && BGR.val[1] >= min_G && BGR.val[2] >= min_R) {
                                                             src_ROI2.at<Vec3b>(j, i)[0] = 0;
                                                             src_ROI2.at<Vec3b>(j, i)[1] = 0;
                                                             src_ROI2.at<Vec3b>(j, i)[2] = 0;
                                                             num2++;
                                                         }

                                                     }
                                                 }
                                          //imshow("new_src_ROI2", src_ROI2);
                                          qDebug("num2=%d\n", num2);

                                          if (num1-num2<50&&num1-num2>-50)
                                          {
                                              qDebug("未改");
                                          }
                                          else
                                              break;



                             }
                                  qDebug("b=%d",b);


                                  Delay_MSec(1000);
                                  cart.tran.x = tcp_pos.tran.x; cart.tran.y = tcp_pos.tran.y; cart.tran.z = tcp_pos.tran.z;
                                  cart.rpy.rx = tcp_pos.rpy.rx; cart.rpy.ry = tcp_pos.rpy.ry; cart.rpy.rz = tcp_pos.rpy.rz;
                                  JKrobot.linear_move(&cart, MoveMode::ABS, TRUE, sysparaset.linearspeed);

                                  if (b<3)
                                     return (0);
                                     else
                                     return (2);


                 }


                 int airswitch(int kkidx, int state, bool num)
                 {
                     //相机内参矩阵
                     Matrix3f m;
                     /***************
                     Matrix3f In_parameter,m;
                     In_parameter(0,0) = 920.81640625;
                     In_parameter(0,1) = 0.0;
                     In_parameter(0,2) = 630.4161376953125;
                     In_parameter(1,0) = 0.0;
                     In_parameter(1,1) = 919.99462890625;
                     In_parameter(1,2) = 373.344665527343;
                     In_parameter(2,0) = 0.0;
                     In_parameter(2,1) = 0.0;
                     In_parameter(2,2) = 1.0;
                     *******************/

                     MatrixXf k(3,1),n(3,1);

                     //std::cout << In_parameter << std::endl;
                     m = In_parameter.inverse();
                     //std::cout << "逆矩阵\n" << m << std::endl;



                     /****if(num==0)
                     {
                     cart.tran.x = 0; cart.tran.y = kkparaset[kkidx].dy ; cart.tran.z = kkparaset[kkidx].dz;
                     cart.rpy.rx = 0; cart.rpy.ry = 0; cart.rpy.rz = 0;
                     JKrobot.linear_move(&cart, MoveMode::INCR, TRUE, 30);
                     }
                     else
                     {
                         GetRobotPosition (&tcp_pos.tran.x,&tcp_pos.tran.y,&tcp_pos.tran.z,&tcp_pos.rpy.rx,&tcp_pos.rpy.ry,&tcp_pos.rpy.rz);
                         cart.tran.x = -360  ; cart.tran.y = 60 + kkparaset[kkidx].dy ; cart.tran.z = 740 + kkparaset[kkidx].dz;
                         cart.rpy.rx = tcp_pos.rpy.rx ; cart.rpy.ry = tcp_pos.rpy.ry; cart.rpy.rz = tcp_pos.rpy.rz;
                         JKrobot.linear_move(&cart, MoveMode::ABS, TRUE, 30);
                     }***/

                     //cart.tran.x = tcp_posbd.tran.x ; cart.tran.y = tcp_posbd.tran.y +kkparaset[kkidx].dy; cart.tran.z = tcp_posbd.tran.z+kkparaset[kkidx].dz;
                     //cart.rpy.rx = tcp_posbd.rpy.rx ; cart.rpy.ry = tcp_posbd.rpy.ry; cart.rpy.rz = tcp_posbd.rpy.rz;
                     //JKrobot.linear_move(&cart, MoveMode::ABS, TRUE, sysparaset.linearspeed);


                     cart.tran.x = tcp_posbd.tran.x ; cart.tran.y = tcp_posbd.tran.y +kkparaset[kkidx].dy; cart.tran.z = tcp_posbd.tran.z+kkparaset[kkidx].dz;
                     cart.rpy.rx = tcp_posbd.rpy.rx ; cart.rpy.ry = tcp_posbd.rpy.ry; cart.rpy.rz = tcp_posbd.rpy.rz;
                     JKrobot.linear_move(&cart, MoveMode::ABS, TRUE, sysparaset.linearspeed);
                     JKrobot.jog(1,INCR,COORD_TOOL,10,-kkparaset[kkidx].dy);
                     Delay_MSec(2000);
                     //JKrobot.jog_stop(0);

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

                         if (!src.data)
                         {
                          qDebug("could not load image...\n");
                          return -1;
                         }
                         Mat kernel = (Mat_<char>(3,3)<<0,-1,0,-1,5,-1,0,-1,0);
                         filter2D(src,src,src.depth(),kernel);

                         Rect rect(kkparaset[kkidx].pix_x, kkparaset[kkidx].pix_y, kkparaset[kkidx].pix_w, kkparaset[kkidx].pix_h);

                             Mat img = src(rect);
                             cvtColor(img, hsv, COLOR_RGB2HSV);
                             inRange(hsv, Scalar(kkparaset[kkidx].color_value1, kkparaset[kkidx].color_value2, kkparaset[kkidx].color_value3), Scalar(kkparaset[kkidx].color_value4, kkparaset[kkidx].color_value5, kkparaset[kkidx].color_value6), ys);


                             Mat element = getStructuringElement(MORPH_RECT, Size(2, 2));
                             morphologyEx(ys, result, MORPH_OPEN, element);//形态学开运算降噪




                             vector<vector<Point>> contours1;//找轮廓
                             vector<vector<Point>> contours2;
                             cv::findContours(result, contours1, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

                             for (int t = 0; t < contours1.size(); t++)
                             {
                                 //double length = arcLength(contours1[t], true);
                                 //cout << "第" << t << "个轮廓长度=" << length << endl;
                                 if (contours1[t].size() > 100)//将比较小的轮廓剔除掉
                                 {
                                     contours2.push_back(contours1[t]);
                                 }
                             }

                             Mat hole(result.size(), CV_8U, Scalar(0)); //遮罩图层
                             cv::drawContours(hole, contours2, -1, Scalar(255), CV_FILLED); //在遮罩图层上，用白色像素填充轮廓
                             vector<vector<Point>> contours3; //放轮廓点
                             vector<Vec4i> hierachy;          //图像的拓扑结构
                             findContours(hole, contours3, hierachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(-1, -1));//根据轮廓点找最小矩形

                             vector<vector<Point>> contours_ploy(contours3.size()); //存放绘制矩形的点
                             vector<Rect> ploy_rects(contours3.size());
                             vector<RotatedRect> minRects(contours3.size());
                             Mat imageContours = Mat::zeros(hole.size(), CV_8UC1);	//最小外接矩形画布

                             for (int i = 0; i < contours3.size(); i++)
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
                                 //cout << "左上角坐标：" << CRect.tl() << endl;
                                 //cout << CRect.x << endl;
                                 //cout << CRect.y << endl;
                                 //cout << CRect.width << endl;
                                 //cout << CRect.height << endl;
                                 //imshow("截取", img);
                                 //imshow("hsv", ys);
                                 //imshow("jiang", result);
                                 //imshow("zao", hole);
                                 //imshow("最小外接矩形", imageContours);
                             }

                                 //判断空开合或闭
                                 //Aspd();
                                int b=0;
                                 while(1)
                               {
                                     b=b+1;
                                     if(b>=2)
                                         break;


                                if(state==1)//上拨
                                {

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

                                 dist_to_center = filtered.get_distance(kkparaset[kkidx].pix_x + CRect.x + CRect.width/2 , kkparaset[kkidx].pix_y  + CRect.y + 0.5 * CRect.height );
                                 dist_to_center1 = filtered.get_distance(kkparaset[kkidx].pix_x + CRect.x + CRect.width/2-1, kkparaset[kkidx].pix_y  + CRect.y + 0.5 * CRect.height-1);
                                 dist_to_center2 = filtered.get_distance(kkparaset[kkidx].pix_x + CRect.x + CRect.width/2-1, kkparaset[kkidx].pix_y  + CRect.y + 0.5 * CRect.height+1);
                                 dist_to_center3 = filtered.get_distance(kkparaset[kkidx].pix_x + CRect.x + CRect.width/2+1, kkparaset[kkidx].pix_y  + CRect.y + 0.5 * CRect.height-1);
                                 dist_to_center4 = filtered.get_distance(kkparaset[kkidx].pix_x + CRect.x + CRect.width/2+1, kkparaset[kkidx].pix_y  + CRect.y + 0.5 * CRect.height+1);

                             dist_to_center = (dist_to_center+dist_to_center1+dist_to_center2+dist_to_center3+dist_to_center4)/5;

                             std::cout << "The camera i1s facing an object " << dist_to_center << " meters away \r"<<std::endl;
                             qDebug("x = %d,y = %f,d = %f\n",  kkparaset[kkidx].pix_x + CRect.x + CRect.width/2 , kkparaset[kkidx].pix_y + CRect.y + 0.5 * CRect.height , dist_to_center) ;

                             n(0,0)= kkparaset[kkidx].pix_x + (CRect.x + CRect.x + CRect.width )/2  ;
                             n(1,0)= kkparaset[kkidx].pix_y + CRect.y + 0.5 * CRect.height  ;
                             n(2,0)= 1.0;

                             //std::cout << n << std::endl;
                             k = m * ((dist_to_center*1000) * n);
                             //std::cout << k << std::endl;
                             double k1,k2,k3;
                             k1=k(0,0);
                             k2=k(1,0);
                             k3=k(2,0);
                             //std::cout << k1 << std::endl;
                             //std::cout << k2 << std::endl;
                             //std::cout << k3 << std::endl;

                             GetRobotPosition (&tcp_pos.tran.x,&tcp_pos.tran.y,&tcp_pos.tran.z,&tcp_pos.rpy.rx,&tcp_pos.rpy.ry,&tcp_pos.rpy.rz);

                             //cart.tran.x = tcp_pos.tran.x  ; cart.tran.y = tcp_pos.tran.y + k1 ; cart.tran.z = tcp_pos.tran.z - k2 ;
                             //cart.rpy.rx = tcp_pos.rpy.rx ; cart.rpy.ry = tcp_pos.rpy.ry; cart.rpy.rz = tcp_pos.rpy.rz;
                             //JKrobot.linear_move(&cart, MoveMode::ABS, TRUE, sysparaset.linearspeed);

                             cart.tran.x = tcp_pos.tran.x  ; cart.tran.y = tcp_pos.tran.y  ; cart.tran.z = tcp_pos.tran.z - k2 ;
                             cart.rpy.rx = tcp_pos.rpy.rx ; cart.rpy.ry = tcp_pos.rpy.ry; cart.rpy.rz = tcp_pos.rpy.rz;
                             JKrobot.linear_move(&cart, MoveMode::ABS, TRUE, sysparaset.linearspeed);
                             JKrobot.jog(1,INCR,COORD_TOOL,40,-k1);
                             Delay_MSec(5000);
                             //JKrobot.jog_stop(0);

                         //pipe1.stop();
                         Delay_MSec(2000);
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

                              if (!src1.data)
                              {
                               qDebug("could not load image...\n");
                               return -1;
                              }
                              Mat kernel = (Mat_<char>(3,3)<<0,-1,0,-1,5,-1,0,-1,0);
                              filter2D(src1,src1,src1.depth(),kernel);
                              Rect rect1(kkparaset[kkidx].pix_x, kkparaset[kkidx].pix_y, kkparaset[kkidx].pix_w, kkparaset[kkidx]. pix_h);

                              Mat img1 = src1(rect1);
                              cvtColor(img1, hsv1, COLOR_RGB2HSV);
                              inRange(hsv1, Scalar(kkparaset[kkidx].color_value1, kkparaset[kkidx].color_value2, kkparaset[kkidx].color_value3), Scalar(kkparaset[kkidx].color_value4, kkparaset[kkidx].color_value5, kkparaset[kkidx].color_value6), ys1);


                              morphologyEx(ys1, result1, MORPH_OPEN, element);//形态学开运算降噪

                              vector<vector<Point>> contours11;//找轮廓
                              vector<vector<Point>> contours21;
                              cv::findContours(result1, contours11, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

                              for (int t = 0; t < contours11.size(); t++)
                              {
                                  double length = arcLength(contours11[t], true);
                                  //cout << "第" << t << "个轮廓长度=" << length << endl;
                                  if (contours11[t].size() > 100)//将比较小的轮廓剔除掉
                                  {
                                      contours21.push_back(contours11[t]);
                                  }
                              }

                              Mat hole1(result1.size(), CV_8U, Scalar(0)); //遮罩图层
                              cv::drawContours(hole1, contours21, -1, Scalar(255), CV_FILLED); //在遮罩图层上，用白色像素填充轮廓
                              vector<vector<Point>> contours31; //放轮廓点
                              vector<Vec4i> hierachy1;          //图像的拓扑结构
                              findContours(hole1, contours31, hierachy1, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(-1, -1));//根据轮廓点找最小矩形

                              vector<vector<Point>> contours_ploy1(contours31.size()); //存放绘制矩形的点
                              vector<Rect> ploy_rects1(contours31.size());
                              vector<RotatedRect> minRects1(contours31.size());
                              Mat imageContours1 = Mat::zeros(hole1.size(), CV_8UC1);	//最小外接矩形画布

                              for (int i = 0; i < contours31.size(); i++)
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
                                  //cout << "左上角坐标：" << CRect1.tl() << endl;
                                  //cout << CRect1.x << endl;
                                  //cout << CRect1.y << endl;
                                  //cout << CRect1.width << endl;
                                  //cout << CRect1.height << endl;
                                  //imshow("截取1", img1);
                                  //imshow("hsv1", ys1);
                                  //imshow("jiang1", result1);
                                  //imshow("zao1", hole1);
                                  //imshow("最小外接矩形1", imageContours1);


                                  float dist_to_center1;
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

                                      dist_to_center11 = filtered.get_distance(kkparaset[kkidx].pix_x + CRect1.x + CRect1.width/2 , kkparaset[kkidx].pix_y  + CRect1.y + 0.5 * CRect1.height );
                                      dist_to_center12 = filtered.get_distance(kkparaset[kkidx].pix_x + CRect1.x + CRect1.width/2-1, kkparaset[kkidx].pix_y  + CRect1.y + 0.5 * CRect1.height-1);
                                      dist_to_center13 = filtered.get_distance(kkparaset[kkidx].pix_x + CRect1.x + CRect1.width/2-1, kkparaset[kkidx].pix_y  + CRect1.y + 0.5 * CRect1.height+1);
                                      dist_to_center14 = filtered.get_distance(kkparaset[kkidx].pix_x + CRect1.x + CRect1.width/2+1, kkparaset[kkidx].pix_y  + CRect1.y + 0.5 * CRect1.height-1);
                                      dist_to_center15 = filtered.get_distance(kkparaset[kkidx].pix_x + CRect1.x + CRect1.width/2+1, kkparaset[kkidx].pix_y  + CRect1.y + 0.5 * CRect1.height+1);

                                  dist_to_center1 = (dist_to_center11+dist_to_center12+dist_to_center13+dist_to_center14+dist_to_center15)/5;




                                  std::cout << "The camera i1s facing an object " << dist_to_center1 << " meters away \r"<<std::endl;

                                  GetRobotPosition (&tcp_pos1.tran.x,&tcp_pos1.tran.y,&tcp_pos1.tran.z,&tcp_pos1.rpy.rx,&tcp_pos1.rpy.ry,&tcp_pos1.rpy.rz);

                                  qDebug("x = %d,y = %d,d = %f\n",  kkparaset[kkidx].pix_x + CRect1.x + CRect1.width/2 , kkparaset[kkidx].pix_y + CRect1.y + CRect1.height,dist_to_center1) ;

                                  qDebug("x = %g,y = %g ,z = %g\n",  tcp_pos1.tran.x - dist_to_center1*1000 + sysparaset.dx+kkparaset[kkidx].locate_dx , tcp_pos1.tran.y + sysparaset.dy+kkparaset[kkidx].locate_dy , tcp_pos1.tran.z + sysparaset.dz - kkparaset[kkidx].locate_dz);

                                  Delay_MSec(1000);
                                  //cart.tran.x = 0 ; cart.tran.y = sysparaset.dy + kkparaset[kkidx].locate_dy  ; cart.tran.z = sysparaset.dz - kkparaset[kkidx].locate_dz ;
                                  //cart.rpy.rx = 0 ; cart.rpy.ry = 0  ; cart.rpy.rz = 0 ;
                                  //JKrobot.linear_move(&cart, MoveMode::INCR, TRUE, sysparaset.linearspeed);


                                  cart.tran.x = 0 ; cart.tran.y = 0  ; cart.tran.z = sysparaset.dz - kkparaset[kkidx].locate_dz ;
                                  cart.rpy.rx = 0 ; cart.rpy.ry = 0  ; cart.rpy.rz = 0 ;
                                  JKrobot.linear_move(&cart, MoveMode::INCR, TRUE, sysparaset.linearspeed);
                                  //JKrobot.jog(1,INCR,COORD_TOOL,40,-(sysparaset.dy + kkparaset[kkidx].locate_dy));
                                  JKrobot.jog(1,INCR,COORD_TOOL,40,-200);

                                  Delay_MSec(5000);
                                  //cart.tran.x = - dist_to_center1*1000 + sysparaset.dx + kkparaset[kkidx].locate_dx ; cart.tran.y = 0 ; cart.tran.z = 0 ;
                                  //cart.rpy.rx = 0 ; cart.rpy.ry = 0  ; cart.rpy.rz = 0 ;
                                  //JKrobot.linear_move(&cart, MoveMode::INCR, TRUE, sysparaset.linearspeed);
                                  JKrobot.jog(2,INCR,COORD_TOOL,40,-(-dist_to_center1*1000 + sysparaset.dx + kkparaset[kkidx].locate_dx));

                                  Delay_MSec(15000);
                                  cart.tran.x = 0 ; cart.tran.y = 0 ; cart.tran.z = kkparaset[kkidx].opt_z ;
                                  cart.rpy.rx = 0 ; cart.rpy.ry = 0  ; cart.rpy.rz = 0 ;
                                  JKrobot.linear_move(&cart, MoveMode::INCR, TRUE, sysparaset.linearspeed);

                                  Delay_MSec(1000);
                                  cart.tran.x = 0 ; cart.tran.y = 0 ; cart.tran.z = -kkparaset[kkidx].opt_z ;
                                  cart.rpy.rx = 0 ; cart.rpy.ry = 0  ; cart.rpy.rz = 0 ;
                                  JKrobot.linear_move(&cart, MoveMode::INCR, TRUE, sysparaset.linearspeed);

                             cart.tran.x = tcp_pos.tran.x; cart.tran.y = tcp_pos.tran.y ; cart.tran.z = tcp_pos.tran.z;
                             cart.rpy.rx = tcp_pos.rpy.rx; cart.rpy.ry = tcp_pos.rpy.ry; cart.rpy.rz = tcp_pos.rpy.rz;
                             JKrobot.linear_move(&cart, MoveMode::ABS, TRUE, sysparaset.linearspeed);

                 }


                              //判断指定动作是否完成


                              /*for (auto i = 0; i < 30; ++i) pipe1.wait_for_frames();
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

                                  if (!src.data)
                                  {
                                   qDebug("could not load image...\n");
                                   return -1;
                                  }

                                  Rect rect(kkparaset[kkidx].pix_x, kkparaset[kkidx].pix_y, kkparaset[kkidx].pix_w, kkparaset[kkidx].pix_h);

                                      Mat img = src(rect);
                                      cvtColor(img, hsv, COLOR_RGB2HSV);
                                      inRange(hsv, Scalar(kkparaset[kkidx].color_value1, kkparaset[kkidx].color_value2, kkparaset[kkidx].color_value3), Scalar(kkparaset[kkidx].color_value4, kkparaset[kkidx].color_value5, kkparaset[kkidx].color_value6), ys);


                                      Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
                                      morphologyEx(ys, result, MORPH_OPEN, element);//形态学开运算降噪




                                      vector<vector<Point>> contours1;//找轮廓
                                      vector<vector<Point>> contours2;
                                      cv::findContours(result, contours1, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

                                      for (int t = 0; t < contours1.size(); t++)
                                      {
                                          double length = arcLength(contours1[t], true);
                                          cout << "第" << t << "个轮廓长度=" << length << endl;
                                          if (contours1[t].size() > 100)//将比较小的轮廓剔除掉
                                          {
                                              contours2.push_back(contours1[t]);
                                          }
                                      }

                                      Mat hole(result.size(), CV_8U, Scalar(0)); //遮罩图层
                                      cv::drawContours(hole, contours2, -1, Scalar(255), CV_FILLED); //在遮罩图层上，用白色像素填充轮廓
                                      vector<vector<Point>> contours3; //放轮廓点
                                      vector<Vec4i> hierachy;          //图像的拓扑结构
                                      findContours(hole, contours3, hierachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(-1, -1));//根据轮廓点找最小矩形

                                      vector<vector<Point>> contours_ploy(contours3.size()); //存放绘制矩形的点
                                      vector<Rect> ploy_rects(contours3.size());
                                      vector<RotatedRect> minRects(contours3.size());
                                      Mat imageContours = Mat::zeros(hole.size(), CV_8UC1);	//最小外接矩形画布

                                      for (int i = 0; i < contours3.size(); i++)
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
                                          //cout << "左上角坐标：" << CRect.tl() << endl;
                                          //cout << CRect.x << endl;
                                          //cout << CRect.y << endl;
                                          //cout << CRect.width << endl;
                                          //cout << CRect.height << endl;
                                          //imshow("截取", img);
                                          //imshow("hsv", ys);
                                          //imshow("jiang", result);
                                          //imshow("zao", hole);
                                          //imshow("最小外接矩形", imageContours);
                                      }

                                          //判断空开合或闭
                                          Aspd();

                              if(s>s1)
                              {
                                  std::cout << "上拨完成"<<std::endl;
                                  break;
                              }*/


                                }
                                //下拨

                                if(state==0)//下拨
                                {
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

                                        dist_to_center = filtered.get_distance(kkparaset[kkidx].pix_x + CRect.x + CRect.width/2 , kkparaset[kkidx].pix_y  + CRect.y + 0.5 * CRect.height );
                                        dist_to_center1 = filtered.get_distance(kkparaset[kkidx].pix_x + CRect.x + CRect.width/2-1, kkparaset[kkidx].pix_y  + CRect.y + 0.5 * CRect.height-1);
                                        dist_to_center2 = filtered.get_distance(kkparaset[kkidx].pix_x + CRect.x + CRect.width/2-1, kkparaset[kkidx].pix_y  + CRect.y + 0.5 * CRect.height+1);
                                        dist_to_center3 = filtered.get_distance(kkparaset[kkidx].pix_x + CRect.x + CRect.width/2+1, kkparaset[kkidx].pix_y  + CRect.y + 0.5 * CRect.height-1);
                                        dist_to_center4 = filtered.get_distance(kkparaset[kkidx].pix_x + CRect.x + CRect.width/2+1, kkparaset[kkidx].pix_y  + CRect.y + 0.5 * CRect.height+1);

                                    dist_to_center = (dist_to_center+dist_to_center1+dist_to_center2+dist_to_center3+dist_to_center4)/5;
                                    std::cout << "The camera i1s facing an object " << dist_to_center << " meters away \r"<<std::endl;
                                    qDebug("x = %d,y = %f,d = %f\n",  kkparaset[kkidx].pix_x + CRect.x + CRect.width/2 , kkparaset[kkidx].pix_y + CRect.y + 0.5 * CRect.height , dist_to_center) ;

                                    n(0,0)= kkparaset[kkidx].pix_x + (CRect.x + CRect.x + CRect.width )/2  ;
                                    n(1,0)= kkparaset[kkidx].pix_y + CRect.y + 0.5 * CRect.height   ;
                                    n(2,0)= 1.0;

                                    //std::cout << n << std::endl;
                                    k = m * ((dist_to_center*1000) * n);
                                    //std::cout << k << std::endl;
                                    double k1,k2,k3;
                                    k1=k(0,0);
                                    k2=k(1,0);
                                    k3=k(2,0);
                                    //std::cout << k1 << std::endl;
                                    //std::cout << k2 << std::endl;
                                    //std::cout << k3 << std::endl;

                                    GetRobotPosition (&tcp_pos.tran.x,&tcp_pos.tran.y,&tcp_pos.tran.z,&tcp_pos.rpy.rx,&tcp_pos.rpy.ry,&tcp_pos.rpy.rz);

                                    //cart.tran.x = tcp_pos.tran.x  ; cart.tran.y = tcp_pos.tran.y + k1 ; cart.tran.z = tcp_pos.tran.z - k2 ;
                                    //cart.rpy.rx = tcp_pos.rpy.rx ; cart.rpy.ry = tcp_pos.rpy.ry; cart.rpy.rz = tcp_pos.rpy.rz;
                                    //JKrobot.linear_move(&cart, MoveMode::ABS, TRUE, sysparaset.linearspeed);
                                    cart.tran.x = tcp_pos.tran.x  ; cart.tran.y = tcp_pos.tran.y + k1 ; cart.tran.z = tcp_pos.tran.z - k2 ;
                                    cart.rpy.rx = tcp_pos.rpy.rx ; cart.rpy.ry = tcp_pos.rpy.ry; cart.rpy.rz = tcp_pos.rpy.rz;
                                    JKrobot.linear_move(&cart, MoveMode::ABS, TRUE, sysparaset.linearspeed);
                                    JKrobot.jog(1,INCR,COORD_TOOL,20,-k1);
                                    Delay_MSec(2000);
                                    //JKrobot.jog_stop(0);

                                //pipe1.stop();
                                        Delay_MSec(2000);
                                //pipe1.start();

                                //rs2::decimation_filter dec_filter;
                                rs2::spatial_filter spat_filter;
                                rs2::threshold_filter thr_filter;
                                rs2::temporal_filter tem_filter;
                                rs2::hole_filling_filter hole_filter;
                                rs2::disparity_transform depth_to_disparity(true);
                                rs2::disparity_transform disparity_to_depth(false);

                                //dec_filter.set_option(RS2_OPTION_FILTER_MAGNITUDE, 5);
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
                                     src1 = imread("/dev/shm/rs-save-to-disk-output-Color.png");

                                     if (!src1.data)
                                     {
                                      qDebug("could not load image...\n");
                                      return -1;
                                     }
                                     Mat kernel = (Mat_<char>(3,3)<<0,-1,0,-1,5,-1,0,-1,0);
                                     filter2D(src1,src1,src1.depth(),kernel);
                                     Rect rect1(kkparaset[kkidx].pix_x, kkparaset[kkidx].pix_y, kkparaset[kkidx]. pix_w, kkparaset[kkidx]. pix_h);

                                     Mat img1 = src1(rect1);
                                     cvtColor(img1, hsv1, COLOR_RGB2HSV);
                                     inRange(hsv1, Scalar(kkparaset[kkidx].color_value1, kkparaset[kkidx].color_value2, kkparaset[kkidx].color_value3), Scalar(kkparaset[kkidx].color_value4, kkparaset[kkidx].color_value5, kkparaset[kkidx].color_value6), ys1);


                                     morphologyEx(ys1, result1, MORPH_OPEN, element);//形态学开运算降噪

                                     vector<vector<Point>> contours11;//找轮廓
                                     vector<vector<Point>> contours21;
                                     cv::findContours(result1, contours11, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

                                     for (int t = 0; t < contours11.size(); t++)
                                     {
                                         double length = arcLength(contours11[t], true);
                                         //cout << "第" << t << "个轮廓长度=" << length << endl;
                                         if (contours11[t].size() > 100)//将比较小的轮廓剔除掉
                                         {
                                             contours21.push_back(contours11[t]);
                                         }
                                     }

                                     Mat hole1(result1.size(), CV_8U, Scalar(0)); //遮罩图层
                                     cv::drawContours(hole1, contours21, -1, Scalar(255), CV_FILLED); //在遮罩图层上，用白色像素填充轮廓
                                     vector<vector<Point>> contours31; //放轮廓点
                                     vector<Vec4i> hierachy1;          //图像的拓扑结构
                                     findContours(hole1, contours31, hierachy1, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(-1, -1));//根据轮廓点找最小矩形

                                     vector<vector<Point>> contours_ploy1(contours31.size()); //存放绘制矩形的点
                                     vector<Rect> ploy_rects1(contours31.size());
                                     vector<RotatedRect> minRects1(contours31.size());
                                     Mat imageContours1 = Mat::zeros(hole1.size(), CV_8UC1);	//最小外接矩形画布

                                     for (int i = 0; i < contours31.size(); i++)
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
                                         //cout << "左上角坐标：" << CRect1.tl() << endl;
                                         //cout << CRect1.x << endl;
                                         //cout << CRect1.y << endl;
                                         //cout << CRect1.width << endl;
                                         //cout << CRect1.height << endl;
                                         //imshow("截取1", img1);
                                         //imshow("hsv1", ys1);
                                         //imshow("jiang1", result1);
                                         //imshow("zao1", hole1);
                                         //imshow("最小外接矩形1", imageContours1);


                                         float dist_to_center1;
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

                                             dist_to_center11 = filtered.get_distance(kkparaset[kkidx].pix_x + CRect1.x + CRect1.width/2  , kkparaset[kkidx].pix_y  + CRect1.y + 0.5 * CRect1.height );
                                             dist_to_center12 = filtered.get_distance(kkparaset[kkidx].pix_x + CRect1.x + CRect1.width/2-1, kkparaset[kkidx].pix_y  + CRect1.y + 0.5 * CRect1.height-1);
                                             dist_to_center13 = filtered.get_distance(kkparaset[kkidx].pix_x + CRect1.x + CRect1.width/2-1, kkparaset[kkidx].pix_y  + CRect1.y + 0.5 * CRect1.height+1);
                                             dist_to_center14 = filtered.get_distance(kkparaset[kkidx].pix_x + CRect1.x + CRect1.width/2+1, kkparaset[kkidx].pix_y  + CRect1.y + 0.5 * CRect1.height-1);
                                             dist_to_center15 = filtered.get_distance(kkparaset[kkidx].pix_x + CRect1.x + CRect1.width/2+1, kkparaset[kkidx].pix_y  + CRect1.y + 0.5 * CRect1.height+1);

                                         dist_to_center1 = (dist_to_center11+dist_to_center12+dist_to_center13+dist_to_center14+dist_to_center15)/5;
                                         std::cout << "The camera i1s facing an object " << dist_to_center1 << " meters away \r"<<std::endl;

                                         GetRobotPosition (&tcp_pos1.tran.x,&tcp_pos1.tran.y,&tcp_pos1.tran.z,&tcp_pos1.rpy.rx,&tcp_pos1.rpy.ry,&tcp_pos.rpy.rz);

                                         qDebug("x = %d,y = %d,d = %f\n",  kkparaset[kkidx].pix_x + CRect1.x + CRect1.width/2 , kkparaset[kkidx].pix_y + CRect1.y + CRect1.height,dist_to_center1) ;

                                         qDebug("x = %g,y = %g ,z = %g\n",  tcp_pos1.tran.x - dist_to_center1*1000 + sysparaset.dx + kkparaset[kkidx].locate_dx , tcp_pos1.tran.y + sysparaset.dy + kkparaset[kkidx].locate_dy, tcp_pos1.tran.z + sysparaset.dz + kkparaset[kkidx].locate_dz);

                                         Delay_MSec(1000);
                                         //cart.tran.x = 0 ; cart.tran.y = sysparaset.dy + kkparaset[kkidx].locate_dy; cart.tran.z = sysparaset.dz + kkparaset[kkidx].locate_dz;
                                         //cart.rpy.rx = 0 ; cart.rpy.ry = 0  ; cart.rpy.rz = 0 ;
                                         //JKrobot.linear_move(&cart, MoveMode::INCR, TRUE, sysparaset.linearspeed);
                                         cart.tran.x = 0 ; cart.tran.y = 0; cart.tran.z = sysparaset.dz + kkparaset[kkidx].locate_dz;
                                         cart.rpy.rx = 0 ; cart.rpy.ry = 0  ; cart.rpy.rz = 0 ;
                                         JKrobot.linear_move(&cart, MoveMode::INCR, TRUE, sysparaset.linearspeed);
                                         JKrobot.jog(1,INCR,COORD_TOOL,40,-(sysparaset.dy + kkparaset[kkidx].locate_dy));

                                         Delay_MSec(2000);
                                         //cart.tran.x = - dist_to_center1*1000 + sysparaset.dx+ kkparaset[kkidx].locate_dx  ; cart.tran.y = 0 ; cart.tran.z = 0 ;
                                         //cart.rpy.rx = 0 ; cart.rpy.ry = 0  ; cart.rpy.rz = 0 ;
                                         //JKrobot.linear_move(&cart, MoveMode::INCR, TRUE, sysparaset.linearspeed);
                                         JKrobot.jog(2,INCR,COORD_TOOL,40,-(- dist_to_center1*1000 + sysparaset.dx+ kkparaset[kkidx].locate_dx));

                                         Delay_MSec(15000);
                                         cart.tran.x = 0 ; cart.tran.y = 0 ; cart.tran.z = -kkparaset[kkidx].opt_z + 8;
                                         cart.rpy.rx = 0 ; cart.rpy.ry = 0  ; cart.rpy.rz = 0 ;
                                         JKrobot.linear_move(&cart, MoveMode::INCR, TRUE, sysparaset.linearspeed);

                                         Delay_MSec(1000);
                                         cart.tran.x = 0 ; cart.tran.y = 0 ; cart.tran.z = kkparaset[kkidx].opt_z - 8;
                                         cart.rpy.rx = 0 ; cart.rpy.ry = 0  ; cart.rpy.rz = 0 ;
                                         JKrobot.linear_move(&cart, MoveMode::INCR, TRUE, sysparaset.linearspeed);

                                    cart.tran.x = tcp_pos.tran.x; cart.tran.y = tcp_pos.tran.y ; cart.tran.z = tcp_pos.tran.z;
                                    cart.rpy.rx = tcp_pos.rpy.rx; cart.rpy.ry = tcp_pos.rpy.ry; cart.rpy.rz = tcp_pos.rpy.rz;
                                    JKrobot.linear_move(&cart, MoveMode::ABS, TRUE, sysparaset.linearspeed);


                        }

                                     //判断指定动作是否完成


                                     /*for (auto i = 0; i < 30; ++i) pipe1.wait_for_frames();
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

                                         if (!src.data)
                                         {
                                          qDebug("could not load image...\n");
                                          return -1;
                                         }

                                         Rect rect(kkparaset[kkidx].pix_x, kkparaset[kkidx].pix_y, kkparaset[kkidx].pix_w, kkparaset[kkidx].pix_h);

                                             Mat img = src(rect);
                                             cvtColor(img, hsv, COLOR_RGB2HSV);
                                             inRange(hsv, Scalar(kkparaset[kkidx].color_value1, kkparaset[kkidx].color_value2, kkparaset[kkidx].color_value3), Scalar(kkparaset[kkidx].color_value4, kkparaset[kkidx].color_value5, kkparaset[kkidx].color_value6), ys);


                                             Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
                                             morphologyEx(ys, result, MORPH_OPEN, element);//形态学开运算降噪




                                             vector<vector<Point>> contours1;//找轮廓
                                             vector<vector<Point>> contours2;
                                             cv::findContours(result, contours1, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

                                             for (int t = 0; t < contours1.size(); t++)
                                             {
                                                 double length = arcLength(contours1[t], true);
                                                 cout << "第" << t << "个轮廓长度=" << length << endl;
                                                 if (contours1[t].size() > 100)//将比较小的轮廓剔除掉
                                                 {
                                                     contours2.push_back(contours1[t]);
                                                 }
                                             }

                                             Mat hole(result.size(), CV_8U, Scalar(0)); //遮罩图层
                                             cv::drawContours(hole, contours2, -1, Scalar(255), CV_FILLED); //在遮罩图层上，用白色像素填充轮廓
                                             vector<vector<Point>> contours3; //放轮廓点
                                             vector<Vec4i> hierachy;          //图像的拓扑结构
                                             findContours(hole, contours3, hierachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(-1, -1));//根据轮廓点找最小矩形

                                             vector<vector<Point>> contours_ploy(contours3.size()); //存放绘制矩形的点
                                             vector<Rect> ploy_rects(contours3.size());
                                             vector<RotatedRect> minRects(contours3.size());
                                             Mat imageContours = Mat::zeros(hole.size(), CV_8UC1);	//最小外接矩形画布

                                             for (int i = 0; i < contours3.size(); i++)
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
                                                 //cout << "左上角坐标：" << CRect.tl() << endl;
                                                 //cout << CRect.x << endl;
                                                 //cout << CRect.y << endl;
                                                 //cout << CRect.width << endl;
                                                 //cout << CRect.height << endl;
                                                 //imshow("截取", img);
                                                 //imshow("hsv", ys);
                                                 //imshow("jiang", result);
                                                 //imshow("zao", hole);
                                                 //imshow("最小外接矩形", imageContours);
                                             }

                                                 //判断空开合或闭
                                                 Aspd();
                                               if(s<s1)
                                               {
                                                   std::cout << "下拨完成"<<std::endl;
                                                   break;
                                               }*/

                        }


                                 }
                                 qDebug("b=%d",b);

                                 if (b<3)
                                    return (0);
                                    else
                                    return (2);
                 }


                 int knob(int kkidx, int state, bool num)
                 {
                     //相机内参矩阵
                     Matrix3f m;
                     /***************
                     Matrix3f In_parameter,m;
                     In_parameter(0,0) = 920.81640625;
                     In_parameter(0,1) = 0.0;
                     In_parameter(0,2) = 630.4161376953125;
                     In_parameter(1,0) = 0.0;
                     In_parameter(1,1) = 919.99462890625;
                     In_parameter(1,2) = 373.344665527343;
                     In_parameter(2,0) = 0.0;
                     In_parameter(2,1) = 0.0;
                     In_parameter(2,2) = 1.0;
                    **************/
                     MatrixXf k(3,1),n(3,1);

                    // std::cout << In_parameter << std::endl;
                     m = In_parameter.inverse();
                    // std::cout << "逆矩阵\n" << m << std::endl;

                    int x1, y1, alpha1, w1 , h1;
                    int x2, y2, alpha2, w2 , h2;

                     /***if(num==0)
                     {
                     cart.tran.x = 0; cart.tran.y = kkparaset[kkidx].dy ; cart.tran.z = kkparaset[kkidx].dz;
                     cart.rpy.rx = 0; cart.rpy.ry = 0; cart.rpy.rz = 0;
                     JKrobot.linear_move(&cart, MoveMode::INCR, TRUE, 30);
                     }
                     else
                     {
                         GetRobotPosition (&tcp_pos.tran.x,&tcp_pos.tran.y,&tcp_pos.tran.z,&tcp_pos.rpy.rx,&tcp_pos.rpy.ry,&tcp_pos.rpy.rz);
                         cart.tran.x = -360  ; cart.tran.y = 60 + kkparaset[kkidx].dy ; cart.tran.z = 740 + kkparaset[kkidx].dz;
                         cart.rpy.rx = tcp_pos.rpy.rx ; cart.rpy.ry = tcp_pos.rpy.ry; cart.rpy.rz = tcp_pos.rpy.rz;
                         JKrobot.linear_move(&cart, MoveMode::ABS, TRUE, 30);
                     }***/

                    cart.tran.x = tcp_posbd.tran.x ; cart.tran.y = tcp_posbd.tran.y + kkparaset[kkidx].dy; cart.tran.z = tcp_posbd.tran.z+kkparaset[kkidx].dz;
                    cart.rpy.rx = tcp_posbd.rpy.rx ; cart.rpy.ry = tcp_posbd.rpy.ry; cart.rpy.rz = tcp_posbd.rpy.rz;
                    JKrobot.linear_move(&cart, MoveMode::ABS, TRUE, sysparaset.linearspeed);

                     Delay_MSec(1000);

                     rs2::colorizer color_map;
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

                     //for(int b=0; kkparaset[kkidx].type= 0 ;b++)
                //{
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

                         if (!src.data)
                         {
                          qDebug("could not load image...\n");
                          return -1;
                         }
                         Mat kernel = (Mat_<char>(3,3)<<0,-1,0,-1,5,-1,0,-1,0);
                         filter2D(src,src,src.depth(),kernel);
                         Rect rect(kkparaset[kkidx].pix_x, kkparaset[kkidx].pix_y, kkparaset[kkidx]. pix_w, kkparaset[kkidx]. pix_h);

                             Mat img = src(rect);
                             cvtColor(img, hsv, COLOR_RGB2HSV);
                             inRange(hsv, Scalar(kkparaset[kkidx].color_value1, kkparaset[kkidx].color_value2, kkparaset[kkidx].color_value3), Scalar(kkparaset[kkidx].color_value4, kkparaset[kkidx].color_value5, kkparaset[kkidx].color_value6), ys);


                             Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
                             morphologyEx(ys, result, MORPH_OPEN, element);//形态学开运算降噪


                             vector<vector<Point>> contours1;//找轮廓
                             vector<vector<Point>> contours2;
                             cv::findContours(result, contours1, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

                             for (int t = 0; t < contours1.size(); t++)
                             {
                                 double length = arcLength(contours1[t], true);
                                 cout << "第" << t << "个轮廓长度=" << length << endl;
                                 if (contours1[t].size() > 50)//将比较小的轮廓剔除掉
                                 {
                                     contours2.push_back(contours1[t]);
                                 }
                             }

                             Mat hole(result.size(), CV_8U, Scalar(0)); //遮罩图层
                             cv::drawContours(hole, contours2, -1, Scalar(255), CV_FILLED); //在遮罩图层上，用白色像素填充轮廓
                             vector<vector<Point>> contours3; //放轮廓点
                             vector<Vec4i> hierachy;          //图像的拓扑结构
                             findContours(hole, contours3, hierachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(-1, -1));//根据轮廓点找最小矩形

                             vector<vector<Point>> contours_ploy(contours3.size()); //存放绘制矩形的点
                             vector<Rect> ploy_rects(contours3.size());
                             vector<RotatedRect> minRects(contours3.size());
                             Mat imageContours = Mat::zeros(hole.size(), CV_8UC1);	//最小外接矩形画布

                             for (int i = 0; i < contours3.size(); i++)
                             {
                                 drawContours(imageContours, contours3, i, Scalar(255), 1, 8, hierachy);
                                 RotatedRect rect = minAreaRect(contours3[i]);
                                 Point2f P[4];
                                 rect.points(P);
                                 for (int j = 0; j <= 3; j++)
                                 {
                                     line(imageContours, P[j], P[(j + 1) % 4], Scalar(255), 2);
                                 }

                                 //imshow("截取", img);
                                 //imshow("hsv", ys);
                                 //imshow("jiang", result);
                                 //imshow("zao", hole);
                                 //imshow("最小外接矩形", imageContours);

                                 int x0, y0, alpha0, w0 , h0;
                                         x0 = rect.center.x;
                                         y0 = rect.center.y;
                                         alpha0 = rect.angle;
                                         w0 = rect.size.width;
                                         h0 = rect.size.height;

                                         cout << "angle0:" << rect.angle << endl;
                                         cout << "width0:" << rect.size.width << endl;
                                         cout << "height0:" << rect.size.height << endl;

                                         if (rect.angle <= 80)
                                         {
                                             if (w0 > h0)
                                             {
                                                 alpha0 = alpha0 + 90;
                                             }
                                             else
                                             {
                                                 alpha0 = alpha0;
                                             }
                                         }
                                         qDebug("x0 = %d,y0 = %d,w0 = %d,h0 = %d,alpha0 = %d", x0,y0,w0,h0,alpha0);



                             float dist_to_center;
                             float dist_to_center1;
                             float dist_to_center2;
                             float dist_to_center3;
                             float dist_to_center4;

                                 rs2::frameset data= pipe1.wait_for_frames();
                                 rs2::depth_frame depth_frame = data.get_depth_frame();
                                 rs2::depth_frame filtered = depth_frame;
                                 filtered = thr_filter.process(filtered);
                                 filtered = depth_to_disparity.process(filtered);
                                 filtered = hole_filter.process(filtered);
                                 filtered = disparity_to_depth.process(filtered);
                                 filtered = spat_filter.process(filtered);
                                 filtered = tem_filter.process(filtered);

                                 dist_to_center = filtered.get_distance(kkparaset[kkidx].pix_x+x0 , kkparaset[kkidx].pix_y+y0 );
                                 dist_to_center1 = filtered.get_distance(kkparaset[kkidx].pix_x+x0-1, kkparaset[kkidx].pix_y+y0-1);
                                 dist_to_center2 = filtered.get_distance(kkparaset[kkidx].pix_x+x0-1, kkparaset[kkidx].pix_y+y0+1);
                                 dist_to_center3 = filtered.get_distance(kkparaset[kkidx].pix_x+x0+1, kkparaset[kkidx].pix_y+y0-1);
                                 dist_to_center4 = filtered.get_distance(kkparaset[kkidx].pix_x+x0+1, kkparaset[kkidx].pix_y+y0+1);

                             dist_to_center = (dist_to_center+dist_to_center1+dist_to_center2+dist_to_center3+dist_to_center4)/5;



                             std::cout << "The camera i1s facing an object " << dist_to_center << " meters away \r"<<std::endl;
                             qDebug("x = %d,y = %d,d = %f\n",  kkparaset[kkidx].pix_x + x0, kkparaset[kkidx].pix_y  + y0 , dist_to_center) ;

                             n(0,0)= kkparaset[kkidx].pix_x + x0 ;
                             n(1,0)= kkparaset[kkidx].pix_y + y0 ;
                             n(2,0)= 1.0;

                             k = m * ((dist_to_center*1000) * n);
                             double k1,k2,k3;
                             k1=k(0,0);
                             k2=k(1,0);
                             k3=k(2,0);

                             GetRobotPosition (&tcp_pos.tran.x,&tcp_pos.tran.y,&tcp_pos.tran.z,&tcp_pos.rpy.rx,&tcp_pos.rpy.ry,&tcp_pos.rpy.rz);

                             //cart.tran.x = tcp_pos.tran.x  ; cart.tran.y = tcp_pos.tran.y + k1 ; cart.tran.z = tcp_pos.tran.z - k2 ;
                             //cart.rpy.rx = tcp_pos.rpy.rx ; cart.rpy.ry = tcp_pos.rpy.ry; cart.rpy.rz = tcp_pos.rpy.rz;
                             //JKrobot.linear_move(&cart, MoveMode::ABS, TRUE, sysparaset.linearspeed);
                             cart.tran.x = tcp_pos.tran.x  ; cart.tran.y = tcp_pos.tran.y  ; cart.tran.z = tcp_pos.tran.z - k2 ;
                             cart.rpy.rx = tcp_pos.rpy.rx ; cart.rpy.ry = tcp_pos.rpy.ry; cart.rpy.rz = tcp_pos.rpy.rz;
                             JKrobot.linear_move(&cart, MoveMode::ABS, TRUE, sysparaset.linearspeed);
                             JKrobot.jog(1,INCR,COORD_TOOL,20,-k1);
                             Delay_MSec(2000);
                             JKrobot.jog_stop(0);

                             }

                             int b=0;
                             while(1)
                             {

                             b=b+1;

                             if(b>=2)
                                 break;

                             //pipe1.stop();
                             Delay_MSec(2000);
                             //pipe1.start();
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
                              src1 = imread("/dev/shm/rs-save-to-disk-output-Color.png");

                              if (!src1.data)
                              {
                               qDebug("could not load image...\n");
                               return -1;
                              }
                              Mat kernel = (Mat_<char>(3,3)<<0,-1,0,-1,5,-1,0,-1,0);
                              filter2D(src1,src1,src1.depth(),kernel);
                              Rect rect1(kkparaset[kkidx].pix_x, kkparaset[kkidx].pix_y, kkparaset[kkidx]. pix_w, kkparaset[kkidx]. pix_h);

                              Mat img1 = src1(rect1);
                              cvtColor(img1, hsv1, COLOR_RGB2HSV);


                              inRange(hsv1, Scalar(kkparaset[kkidx].color_value1, kkparaset[kkidx].color_value2, kkparaset[kkidx].color_value3), Scalar(kkparaset[kkidx].color_value4, kkparaset[kkidx].color_value5, kkparaset[kkidx].color_value6), ys1);

                                  morphologyEx(ys1, result1, MORPH_OPEN, element);//形态学开运算降噪

                              vector<vector<Point>> contours11;//找轮廓
                              vector<vector<Point>> contours21;
                              cv::findContours(result1, contours11, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

                              for (int t = 0; t < contours11.size(); t++)
                              {
                                  double length = arcLength(contours11[t], true);
                                  cout << "第" << t << "个轮廓长度=" << length << endl;
                                  if (contours11[t].size() > 50)//将比较小的轮廓剔除掉
                                  {
                                      contours21.push_back(contours11[t]);
                                  }
                              }

                              Mat hole1(result1.size(), CV_8U, Scalar(0)); //遮罩图层
                              cv::drawContours(hole1, contours21, -1, Scalar(255), CV_FILLED); //在遮罩图层上，用白色像素填充轮廓
                              vector<vector<Point>> contours31; //放轮廓点
                              vector<Vec4i> hierachy1;          //图像的拓扑结构
                              findContours(hole1, contours31, hierachy1, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(-1, -1));//根据轮廓点找最小矩形

                              vector<vector<Point>> contours_ploy1(contours31.size()); //存放绘制矩形的点
                              vector<Rect> ploy_rects1(contours31.size());
                              vector<RotatedRect> minRects1(contours31.size());
                              Mat imageContours1 = Mat::zeros(hole1.size(), CV_8UC1);	//最小外接矩形画布

                              for (int i = 0; i < contours31.size(); i++)
                              {
                                  drawContours(imageContours1, contours31, i, Scalar(255), 1, 8, hierachy1);
                                  RotatedRect rect1 = minAreaRect(contours31[i]);
                                  Point2f P1[4];
                                  rect1.points(P1);
                                  for (int j = 0; j <= 3; j++)
                                  {
                                      line(imageContours1, P1[j], P1[(j + 1) % 4], Scalar(255), 2);
                                  }


                                  //imshow("截取1", img1);
                                  //imshow("hsv1", ys1);
                                  //imshow("jiang1", result1);
                                  //imshow("zao1", hole1);
                                  //imshow("最小外接矩形1", imageContours1);


                                          x1 = rect1.center.x;                    //  中心位置
                                          y1 = rect1.center.y;
                                          alpha1 = rect1.angle;
                                          w1 = rect1.size.width;
                                          h1 = rect1.size.height;

                                          cout << "angle1:" << rect1.angle << endl;
                                          cout << "width1:" << rect1.size.width << endl;
                                          cout << "height1:" << rect1.size.height << endl;

                                          if (rect1.angle <= 80)
                                          {
                                              if (w1 > h1)
                                              {
                                                  alpha1 = alpha1 + 90;
                                              }
                                              else
                                              {
                                                  alpha1 = alpha1;
                                              }
                                          }
                                          qDebug("x1 = %d,y1 = %d,w1 = %d,h1 = %d,alpha1 = %d", x1,y1,w1,h1,alpha1);

                                  float dist_to_center1;
                                  float dist_to_center11;
                                  float dist_to_center12;
                                  float dist_to_center13;
                                  float dist_to_center14;
                                  float dist_to_center15;

                                      rs2::frameset data= pipe1.wait_for_frames();
                                      rs2::depth_frame depth_frame = data.get_depth_frame();
                                      rs2::depth_frame filtered = depth_frame;
                                      filtered = thr_filter.process(filtered);
                                      filtered = depth_to_disparity.process(filtered);
                                      filtered = hole_filter.process(filtered);
                                      filtered = disparity_to_depth.process(filtered);
                                      filtered = spat_filter.process(filtered);
                                      filtered = tem_filter.process(filtered);

                                      dist_to_center11 = filtered.get_distance(kkparaset[kkidx].pix_x + x1  , kkparaset[kkidx].pix_y  + y1 );
                                      dist_to_center12 = filtered.get_distance(kkparaset[kkidx].pix_x + x1-1, kkparaset[kkidx].pix_y  + y1-1);
                                      dist_to_center13 = filtered.get_distance(kkparaset[kkidx].pix_x + x1-1, kkparaset[kkidx].pix_y  + y1+1);
                                      dist_to_center14 = filtered.get_distance(kkparaset[kkidx].pix_x + x1+1, kkparaset[kkidx].pix_y  + y1-1);
                                      dist_to_center15 = filtered.get_distance(kkparaset[kkidx].pix_x + x1+1, kkparaset[kkidx].pix_y  + y1+1);

                                  dist_to_center1 = (dist_to_center11+dist_to_center12+dist_to_center13+dist_to_center14+dist_to_center15)/5;


                                  std::cout << "The camera i1s facing an object " << dist_to_center1 << " meters away \r"<<std::endl;

                                  GetRobotPosition (&tcp_pos1.tran.x,&tcp_pos1.tran.y,&tcp_pos1.tran.z,&tcp_pos1.rpy.rx,&tcp_pos1.rpy.ry,&tcp_pos1.rpy.rz);

                                  qDebug("x = %d,y = %d,d = %f\n",  kkparaset[kkidx].pix_x + x1, kkparaset[kkidx].pix_y + y1 , dist_to_center1) ;

                                  qDebug("x = %g,y = %g ,z = %g\n",  tcp_pos1.tran.x - dist_to_center1*1000 + sysparaset.dx + kkparaset[kkidx].locate_dx , tcp_pos1.tran.y + sysparaset.dy + kkparaset[kkidx].locate_dy , tcp_pos1.tran.z + sysparaset.dz + kkparaset[kkidx].locate_dz);

                                  Delay_MSec(1000);
                                  //cart.tran.x = 0 ; cart.tran.y = sysparaset.dy + kkparaset[kkidx].locate_dy ; cart.tran.z = sysparaset.dz + kkparaset[kkidx].locate_dz;
                                  //cart.rpy.rx = 0 ; cart.rpy.ry = 0  ; cart.rpy.rz = 0 ;
                                  //JKrobot.linear_move(&cart, MoveMode::INCR, TRUE, sysparaset.linearspeed);
                                  cart.tran.x = 0 ; cart.tran.y = 0 ; cart.tran.z = sysparaset.dz + kkparaset[kkidx].locate_dz;
                                  cart.rpy.rx = 0 ; cart.rpy.ry = 0  ; cart.rpy.rz = 0 ;
                                  JKrobot.linear_move(&cart, MoveMode::INCR, TRUE, sysparaset.linearspeed);
                                  JKrobot.jog(1,INCR,COORD_TOOL,40,-(sysparaset.dy + kkparaset[kkidx].locate_dy));
                                  Delay_MSec(2000);

                                  release();

                                  if(alpha1>=125&&kkparaset[kkidx].value==-1)//当前为左
                                  {
                                  refP.jVal[0] = 0 * PI / 180; refP.jVal[1] = 0 * PI / 180; refP.jVal[2] = 0 * PI / 180;
                                  refP.jVal[3] = 0 * PI / 180; refP.jVal[4] = 0 * PI / 180; refP.jVal[5] = -kkparaset[kkidx].locate_angle6 * PI / 180;
                                  JKrobot.joint_move(&refP, MoveMode::INCR, TRUE, sysparaset.angularspeed);
                                  }
                                  if(alpha1>=80&&alpha1<=100&&kkparaset[kkidx].value==0)//当前为中
                                  {
                                  refP.jVal[0] = 0 * PI / 180; refP.jVal[1] = 0 * PI / 180; refP.jVal[2] = 0 * PI / 180;
                                  refP.jVal[3] = 0 * PI / 180; refP.jVal[4] = 0 * PI / 180; refP.jVal[5] = 0 * PI / 180;
                                  JKrobot.joint_move(&refP, MoveMode::INCR, TRUE, sysparaset.angularspeed);
                                  }
                                  if(alpha1<=55&&kkparaset[kkidx].value==1)//当前为右
                                  {
                                  refP.jVal[0] = 0 * PI / 180; refP.jVal[1] = 0 * PI / 180; refP.jVal[2] = 0 * PI / 180;
                                  refP.jVal[3] = 0 * PI / 180; refP.jVal[4] = 0 * PI / 180; refP.jVal[5] = kkparaset[kkidx].locate_angle6 * PI / 180;
                                  JKrobot.joint_move(&refP, MoveMode::INCR, TRUE, sysparaset.angularspeed);
                                  }


                                  Delay_MSec(1000);
                                  //cart.tran.x = - dist_to_center1*1000 + sysparaset.dx + kkparaset[kkidx].locate_dx ; cart.tran.y = 0 ; cart.tran.z = 0 ;
                                  //cart.rpy.rx = 0 ; cart.rpy.ry = 0  ; cart.rpy.rz = 0 ;
                                  //JKrobot.linear_move(&cart, MoveMode::INCR, TRUE, sysparaset.linearspeed);
                                  JKrobot.jog(2,INCR,COORD_TOOL,40,-(- dist_to_center1*1000 + sysparaset.dx + kkparaset[kkidx].locate_dx));
                                  Delay_MSec(15000);
                                  tight();


                                  if((kkparaset[kkidx].value==0&&state==-1&&kkparaset[kkidx].type==22)||(kkparaset[kkidx].value==1&&state==0&&kkparaset[kkidx].type==22))//左旋
                                  {
                                  refP.jVal[0] = 0 * PI / 180; refP.jVal[1] = 0 * PI / 180; refP.jVal[2] = 0 * PI / 180;
                                  refP.jVal[3] = 0 * PI / 180; refP.jVal[4] = 0 * PI / 180; refP.jVal[5] = -kkparaset[kkidx].opt_angle6 * PI / 180;
                                  JKrobot.joint_move(&refP, MoveMode::INCR, TRUE, sysparaset.angularspeed);
                                  }

                                  if((kkparaset[kkidx].value==-1&&state==0&&kkparaset[kkidx].type==22)||(kkparaset[kkidx].value==0&&state==1&&kkparaset[kkidx].type==22))//右旋
                                  {
                                  refP.jVal[0] = 0 * PI / 180; refP.jVal[1] = 0 * PI / 180; refP.jVal[2] = 0 * PI / 180;
                                  refP.jVal[3] = 0 * PI / 180; refP.jVal[4] = 0 * PI / 180; refP.jVal[5] = kkparaset[kkidx].opt_angle6 * PI / 180;
                                  JKrobot.joint_move(&refP, MoveMode::INCR, TRUE, sysparaset.angularspeed);
                                  }

                                  if(kkparaset[kkidx].value==1&&state==-1&&kkparaset[kkidx].type==22)//左旋
                                  {
                                  refP.jVal[0] = 0 * PI / 180; refP.jVal[1] = 0 * PI / 180; refP.jVal[2] = 0 * PI / 180;
                                  refP.jVal[3] = 0 * PI / 180; refP.jVal[4] = 0 * PI / 180; refP.jVal[5] = -kkparaset[kkidx].opt_angle6 * PI / 90;
                                  JKrobot.joint_move(&refP, MoveMode::INCR, TRUE, sysparaset.angularspeed);
                                  }

                                  if(kkparaset[kkidx].value==-1&&state==1&&kkparaset[kkidx].type==22)//右旋
                                  {
                                  refP.jVal[0] = 0 * PI / 180; refP.jVal[1] = 0 * PI / 180; refP.jVal[2] = 0 * PI / 180;
                                  refP.jVal[3] = 0 * PI / 180; refP.jVal[4] = 0 * PI / 180; refP.jVal[5] = kkparaset[kkidx].opt_angle6 * PI / 90;
                                  JKrobot.joint_move(&refP, MoveMode::INCR, TRUE, sysparaset.angularspeed);
                                  }

                                  if(kkparaset[kkidx].value==1&&state==-1&&kkparaset[kkidx].type==20)//左旋
                                  {
                                  refP.jVal[0] = 0 * PI / 180; refP.jVal[1] = 0 * PI / 180; refP.jVal[2] = 0 * PI / 180;
                                  refP.jVal[3] = 0 * PI / 180; refP.jVal[4] = 0 * PI / 180; refP.jVal[5] = -kkparaset[kkidx].opt_angle6 * PI / 180;
                                  JKrobot.joint_move(&refP, MoveMode::INCR, TRUE, sysparaset.angularspeed);
                                  refP.jVal[0] = 0 * PI / 180; refP.jVal[1] = 0 * PI / 180; refP.jVal[2] = 0 * PI / 180;
                                  refP.jVal[3] = 0 * PI / 180; refP.jVal[4] = 0 * PI / 180; refP.jVal[5] = kkparaset[kkidx].opt_angle6 * PI / 180;
                                  JKrobot.joint_move(&refP, MoveMode::INCR, TRUE, sysparaset.angularspeed);
                                  }

                                  if(kkparaset[kkidx].value==-1&&state==1&&kkparaset[kkidx].type==20)//右旋
                                  {
                                  refP.jVal[0] = 0 * PI / 180; refP.jVal[1] = 0 * PI / 180; refP.jVal[2] = 0 * PI / 180;
                                  refP.jVal[3] = 0 * PI / 180; refP.jVal[4] = 0 * PI / 180; refP.jVal[5] = kkparaset[kkidx].opt_angle6 * PI / 180;
                                  JKrobot.joint_move(&refP, MoveMode::INCR, TRUE, sysparaset.angularspeed);
                                  refP.jVal[0] = 0 * PI / 180; refP.jVal[1] = 0 * PI / 180; refP.jVal[2] = 0 * PI / 180;
                                  refP.jVal[3] = 0 * PI / 180; refP.jVal[4] = 0 * PI / 180; refP.jVal[5] = -kkparaset[kkidx].opt_angle6 * PI / 180;
                                  JKrobot.joint_move(&refP, MoveMode::INCR, TRUE, sysparaset.angularspeed);
                                  }

                             //退出再次拍照判断
                                  release();

                                  //cart.tran.x = 200 ; cart.tran.y = 0 ; cart.tran.z = 0;
                                  //cart.rpy.rx = 0 ; cart.rpy.ry = 0  ; cart.rpy.rz = 0 ;
                                  //JKrobot.linear_move(&cart, MoveMode::INCR, TRUE, sysparaset.linearspeed);
                                  JKrobot.jog(2,INCR,COORD_TOOL,40,-200);

                                  cart.tran.x = tcp_pos1.tran.x; cart.tran.y = tcp_pos1.tran.y ; cart.tran.z = tcp_pos1.tran.z;
                                  cart.rpy.rx = tcp_pos1.rpy.rx; cart.rpy.ry = tcp_pos1.rpy.ry; cart.rpy.rz = tcp_pos1.rpy.rz;
                                  JKrobot.linear_move(&cart, MoveMode::ABS, TRUE, sysparaset.linearspeed);


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


                              //opencv
                              src2 = imread("/dev/shm/rs-save-to-disk-output-Color.png");

                              if (!src2.data)
                              {
                               qDebug("could not load image...\n");
                               return -1;
                              }
                              filter2D(src2,src2,src2.depth(),kernel);
                              Rect rect2(kkparaset[kkidx].pix_x, kkparaset[kkidx].pix_y, kkparaset[kkidx]. pix_w, kkparaset[kkidx]. pix_h);

                                  Mat img2 = src2(rect2);
                                  cvtColor(img2, hsv2, COLOR_RGB2HSV);
                                  inRange(hsv2, Scalar(kkparaset[kkidx].color_value1, kkparaset[kkidx].color_value2, kkparaset[kkidx].color_value3), Scalar(kkparaset[kkidx].color_value4, kkparaset[kkidx].color_value5, kkparaset[kkidx].color_value6), ys2);


                                  morphologyEx(ys2, result2, MORPH_OPEN, element);//形态学开运算降噪


                                  vector<vector<Point>> contours12;//找轮廓
                                  vector<vector<Point>> contours22;
                                  cv::findContours(result2, contours12, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

                                  for (int t = 0; t < contours12.size(); t++)
                                  {
                                      double length = arcLength(contours12[t], true);
                                      cout << "第" << t << "个轮廓长度=" << length << endl;
                                      if (contours12[t].size() > 50)//将比较小的轮廓剔除掉
                                      {
                                          contours22.push_back(contours12[t]);
                                      }
                                  }

                                  Mat hole2(result2.size(), CV_8U, Scalar(0)); //遮罩图层
                                  cv::drawContours(hole2, contours22, -1, Scalar(255), CV_FILLED); //在遮罩图层上，用白色像素填充轮廓
                                  vector<vector<Point>> contours32; //放轮廓点
                                  vector<Vec4i> hierachy2;          //图像的拓扑结构
                                  findContours(hole2, contours32, hierachy2, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(-1, -1));//根据轮廓点找最小矩形

                                  vector<vector<Point>> contours_ploy2(contours32.size()); //存放绘制矩形的点
                                  vector<Rect> ploy_rects2(contours32.size());
                                  vector<RotatedRect> minRects2(contours32.size());
                                  Mat imageContours2 = Mat::zeros(hole2.size(), CV_8UC1);	//最小外接矩形画布


                                  for (int i = 0; i < contours32.size(); i++)
                                  {
                                      drawContours(imageContours2, contours32, i, Scalar(255), 1, 8, hierachy);
                                      RotatedRect rect2 = minAreaRect(contours32[i]);
                                      Point2f P2[4];
                                      rect2.points(P2);
                                      for (int j = 0; j <= 3; j++)
                                      {
                                          line(imageContours2, P2[j], P2[(j + 1) % 4], Scalar(255), 2);
                                      }

                                      //imshow("截取2", img2);
                                      //imshow("hsv2", ys2);
                                      //imshow("jiang2", result2);
                                      //imshow("zao2", hole2);
                                      //imshow("最小外接矩形2", imageContours2);


                                              x2 = rect2.center.x;
                                              y2 = rect2.center.y;
                                              alpha2 = rect2.angle;
                                              w2 = rect2.size.width;
                                              h2 = rect2.size.height;

                                              cout << "angle2:" << rect2.angle << endl;
                                              cout << "width2:" << rect2.size.width << endl;
                                              cout << "height2:" << rect2.size.height << endl;

                                              if (rect2.angle <= 80)
                                              {
                                                  if (w2 > h2)
                                                  {
                                                      alpha2 = alpha2 + 90;
                                                  }
                                                  else
                                                  {
                                                      alpha2 = alpha2;
                                                  }
                                              }
                                              qDebug("x2 = %d,y2 = %d,w2 = %d,h2 = %d,alpha2 = %d", x2,y2,w2,h2,alpha2);
                                  }

                            if(fabs(alpha2-alpha1)>20||kkparaset[kkidx].type==20)
                               break;


                             }

                         qDebug("b=%d",b);
                         tight();


                         if (b<3)
                            return (0);
                            else
                            return (2);


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


