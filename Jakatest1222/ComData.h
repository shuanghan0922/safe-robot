
#include <stdio.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <QString>
#include <sys/socket.h>
#include <netinet/in.h>
#include <QDateTime>
#include "JAKAZuRobot.h"
#include "jktypes.h"
#include <webcmd.h>
#include "dialogequset.h"
#include <Eigen/Geometry>
#include <Eigen/Dense>
#include <QObject>

#define  ERRR_NET   0x1000
using namespace Eigen;
using Eigen::MatrixXd;

//***************************
#define INVALID_SOCKET -1 //creat socket err
#define SOCKET_ERROR -1   //
#define COMMTEST  1//有效，通信调试;注释，不调试
#define UDPTEST 1//有效，UDP调试;注释，TCP
//#define SECSTEST 1//有效，调试;注释，不调试
#define PHOTONUM  1//photo num
typedef int SOCKET;

extern Matrix3f In_parameter;//内参矩阵
extern CartesianPose tcp_posbd;

//login
//********************
 extern QString logname;
 extern QString logid;
 extern int logright;  //登录权限
 extern bool blog;
 extern QString sqid;
 extern QString sqtitle;
 extern QString slayout;
 extern unsigned char nlayout;
 extern int nequ; //柜总数
 extern int nkk;  //开关总数
 extern int nkktype;//开关类型总数
 extern bool isnewequ;//电气柜更新
 extern bool gnum;//初次计算
 extern bool gbd;//展开标定

 extern bool ugv_dx;//位姿误差
 extern bool ugv_dy;
 extern bool ugv_dz;
 extern bool ugv_drx;
 extern bool ugv_dry;
 extern bool ugv_drz;
 extern bool ugv_drw;

 extern unsigned char runmodel;//0:手动，1：自动，2：设置，3：调试

 extern bool gchg1;//室1中开关状态变化或选择柜体变化，启动显示更新
 extern bool gchg2;//室2
 extern bool gchg0;//室外
 extern bool gchg3;//测试
 extern bool gpicchg1;//室1图片变化
 extern bool gpicchg2;
 extern bool gpicchg0;
 extern bool gpicchg3;
 extern unsigned char pgno;//页面选择

 extern QString gequid;//当前柜体id号
 extern unsigned char step;//控制流程
 extern QString gno;//当前开关编号
 extern int gstate;//当前要到达状态
 extern bool gtestauto;//演示
 extern int roscmd;

 extern char *dirpf;

 extern JAKAZuRobot JKrobot;
 extern RobotStatus Jkstatus;
 //extern bool isJKconn;

 extern  pthread_t pthrcv;

 extern bool rosconnect;
 extern unsigned char rosGoalStatus;
 extern unsigned char rosopt;//1:移动，到位,0
 extern unsigned char jakaopt;//20：舒展，21：到位，30：初始位，31：到位，50：开关初始位，51：到位
 //60：开关终止位，61：到位

 extern Webcmd *webcmd;
 extern Dialogequset *dlgequ;

 struct CABINET
 {

     QString id;//索引
     QString no; //编号
     QString title;//名称
     unsigned char roomno;//0:室外，1/2/3：室1，室2，test
     //类型，柜体开关布局，界面设计，test:固定/0
     //室1：按空开设置分：0：zkk1/zk1/zk2,1：zkk1/zk1,2：fb61/fb62/zk1
     //室2：按路数设置分：
     //室外：
     unsigned char type;
     double x;//JAKA double x,y,z, 单位mm
     double y;
     double z;
     double rx;//JAKA double rx,ry,rz,单位：rad
     double ry;
     double rz;
     double shx;//ROS  position double x,y,z
     double shy;
     double shz;
     double shx1;//ROS orientation  double x,y,z,
     double shy1;
     double shz1;
     double shw1;
     double calibration_dz;//标定点高度，由展开位置移动至标定位置     
 };

 extern struct CABINET equparaset[];

 struct KK
 {

     QString id;//电气柜索引，开关与柜体联系
     QString no; //编号
     QString name;//名称
     unsigned char type;//类型，0:按钮,1:1P,2:2P,3:3P,4:2档旋扭,5:2档旋扭自锁,6:3档旋扭
     int value;//选择:类型0-5:分/0，合/1，6：左/0，中/2，右/1
     double dx;//偏移坐标 double x,y,z, 单位mm
     double dy;
     double dz;     
     unsigned char pos;     
     int pix_x; //像素截图坐标x
     int pix_y; //像素截图坐标y
     int pix_w; //截图宽度
     int pix_h; //截图高度
     double locate_dx;  // 开关定位坐标修正值dx、dy、dz（单位mm），以及角度修正值（单位度）
     double locate_dy;
     double locate_dz;
     double locate_angle6;
     double opt_x;      // 开关操作坐标修正值dx，dy，dz（单位mm），以及角度修正值（单位度）
     double opt_y;
     double opt_z;
     double opt_angle6;
     int color; //颜色值，0-无色，1-黑色，2-红色，3-绿色，4-蓝色5-带灯红色，6-带灯绿色
     int color_value1;  // 开关颜色阈值设定值
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

 };

extern struct KK kkparaset[];

struct KKTYPE
{
    QString name;
    int type;
    int stunum;
};

extern struct KKTYPE kktype[];

 struct SYSPARA //系统参数
{

    bool bsl;
    bool bpp;
    bool bcy;
    bool bur;
    bool bsf;
    bool bsj;
    /***********8
    char pfno[10];
    char pfname[20];
    char cpno[10];
    char cpname[20];
    ****************/
    QString pfno;
    QString pfname;
   // QString cpno;
   // QString cpname;
    unsigned short dayok;
    unsigned short daybj;
    unsigned short dayop;
    QString addplc;
    unsigned short  portplc;
    QString addplc1;
    unsigned short  portplc1;
    QString addplc2;
    unsigned short  portplc2;
    QString addsecs;
    unsigned short  portsecs;
    QString addpcplc;
    QString addpcsecs;
    QString addpcphoto;
    float stepdj;
    float stepupdj;//
    float freshold1;
    float freshold2;
    float freshold3;
    float freshold41;
    int freshold42;
    float freshold43;
    int freshold44;
    int dytm;
    int szunit;
    int kjunit;
    int szmin;
    int kjmin;
    int sjposnum;
    int ppdelay;
    int urruntm;
    int ppruntm;
    int yjhmruntm;
    int yjqsruntm;
    int szruntm;
    int cqruntm;
    int cqqcruntm;
    int xlruntm;
    int ppdelay1;
    int szkg;
    int ppnum;
    unsigned char modelsel;
    double dx; //单位mm
    double dy;
    double dz;
    int linearspeed;
    float angularspeed;
    float errthreshold;

    //double In_parameter[3][3];
} ;


extern struct SYSPARA sysparaset;

struct SETPOS
{
   double px;
   double py;
   double pz;
   double ox;
   double oy;
   double oz;
   double ow;
   bool bsetpos;
};

extern struct SETPOS posset;

struct ROSPOS
{
   double x;
   double y;
   double z;
   double x1;
   double y1;
   double z1;
   double w1;

};

extern struct ROSPOS rospos;

struct robuster_BatteryState
{
 //std_msgs/Header header;
 float battery_percentage; // 剩余电量
 float voltage; // 电压值
 float current; // 电流值。负数表示充电，正-放电
 float battery_temperature;
 float remaining_capacity;
 float rated_capacity;
};

extern struct robuster_BatteryState batterystate;

struct SingleMotorState
{
 float voltage;// no use
 float current;
 int rpm;
 int tick;
 float temperature;
 int fault;
};

extern struct SingleMotorState motorstate[];

void *Threadjaka(void *);
void *ThreadProcrcv(void *);
void *Threadros(void *);
void *Threadphoto(void *);
void *ThreadProcess(void *);

int equid2index(QString id);
int kkno2index(QString no);
int kktype2index(int type);
void rosinitpos();

double rad2ang(double rad);
double ang2rad(double ang);
//欧拉角转四元数
void eular2quat(double eu[3],double quat[4]);
void quat2eular(double quat[4],double eu[3]);
//int kk2init//JAKA 开关操作
int jk2initpos(QString id);//JAKA to initpos
int kkposandequid2index(QString id,int pos);



