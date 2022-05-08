#include <termio.h>
#include <termio.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
//#include <sys/select.h>
//#include <sys/time.h>
#include "ComData.h"
#include <signal.h>
#include <netinet/tcp.h>
#include <net/if.h>
#include <linux/sockios.h>
#include <sys/ioctl.h>
//#include <Python.h>
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include <QFile>
#include <math.h>
#include "string.h"
#include "jkrobot.h"

typedef struct sockaddr_in SOCKADDR_IN;
Matrix3f In_parameter;//内参矩阵
CartesianPose tcp_posbd;
//login
//********************
QString logname;
QString logid;
int logright;  //登录权限
bool blog;
QString sqid;
QString sqtitle;
QString slayout;
unsigned char nlayout;
int nequ; //柜总数
int nkk;  //开关总数
int nkktype;//开关类型总数
bool isnewequ=true;//电气柜更新
bool gchg1=true;//柜体或其中开关状态改变，
bool gchg2=true;//室2
bool gchg0=true;//室外
bool gchg3=true;//测试
bool gpicchg1=false;//室1图片变化
bool gpicchg2=false;
bool gpicchg0=false;
bool gpicchg3=false;
unsigned char pgno=0;//页面选择
QString gequid="01";//初始柜id号
unsigned char step=0;//控制流程
QString gno;//当前开关编号
bool ugv_dx;//位姿误差
bool ugv_dy;
bool ugv_dz;
bool ugv_drx;
bool ugv_dry;
bool ugv_drz;
bool ugv_drw;
unsigned char runmodel=0;//0:手动，1：自动，2：设置，3：调试
unsigned char btnopt=0;//按钮操作
int gstate;//当前要到达状态
bool gnum;//初次计算
bool gtestauto=false;//演示
bool gbd=false;//展开标定
bool camera_running=false;//视频采集，不启动
rs2::pipeline pipe1;
 char *dirpf="/home/hfut/JakaRobot/pf";
//char *dirpf=$$PWD+"/pf";
Webcmd *webcmd=new Webcmd();
Dialogequset *dlgequ;
struct SYSPARA sysparaset;
struct CABINET equparaset[30];//电气柜数量30
struct KK kkparaset[200];//开关数量200
struct KKTYPE kktype[10];//开关类型10
struct SETPOS posset;//目标位设置
struct ROSPOS rospos;//机器人位姿
struct robuster_BatteryState batterystate;
struct SingleMotorState motorstate[4];//4 motor

bool rosconnect=false;
unsigned char rosGoalStatus=0;
int roscmd=0;
JAKAZuRobot JKrobot;
RobotStatus Jkstatus;//=NULL;
unsigned char jakaopt=0;//20：舒展，21：到位，30：初始位，31：到位，50：开关初始位，51：到位
                        //60：开关终止位，61：到位

pthread_t pthrcv;


int i;

double rad2ang(double rad)
{
   double ang;
   ang=180/(4*atan(1.0))*rad;
   return ang;
}
double ang2rad(double ang)
{
    double rad;
    rad=4*atan(1.0)/180*ang;
}
//欧拉角转四元数
void eular2quat(double eu[3],double quat[4])
{
    //r = float(input("绕x轴旋转角度:"))
    //p = float(input("绕y轴旋转角度:"))
    //y = float(input("绕z轴旋转角度:"))
    double ps,ys,rs,pc,yc,rc;
    ps = sin(ang2rad(eu[1]/2));
    ys = sin(ang2rad(eu[2]/2));
    rs = sin(ang2rad(eu[0]/2));

    pc = cos(ang2rad(eu[1]/2));
    yc = cos(ang2rad(eu[2]/2));
    rc = cos(ang2rad(eu[0]/2));

    quat[3] = rc*pc*yc + rs*ps*ys;//w
    quat[0] = rs*pc*yc - rc*ps*ys;//x
    quat[1] = rc*ps*yc + rs*pc*ys;//y
    quat[2] = rc*pc*ys - rs*ps*yc;//z
}
void quat2eular(double quat[4],double eu[3])
{
    eu[0] = atan2(2*(quat[3]*quat[0]+quat[1]*quat[2]),1-2*(quat[0]*quat[0]+quat[1]*quat[1]));
    eu[0] = rad2ang(eu[0]);
    eu[1] = asin(2*(quat[3]*quat[1]-quat[2]*quat[0]));
    eu[1] = rad2ang(eu[1]);
    eu[2] = atan2(2*(quat[3]*quat[2]+quat[0]*quat[1]),1-2*(quat[1]*quat[1]+quat[2]*quat[2]));
    eu[2] = rad2ang(eu[2]);
}
/********************
int scanKeyboard()
{
 int in; 
 struct termios new_settings; 
 struct termios stored_settings; 
 tcgetattr(0,&stored_settings); 
 new_settings = stored_settings; 
 new_settings.c_lflag &= (~ICANON); 
 new_settings.c_cc[VTIME] = 0; 
 tcgetattr(0,&stored_settings); 
 new_settings.c_cc[VMIN] = 1; 
 tcsetattr(0,TCSANOW,&new_settings); 
 
 in = getchar(); 

 tcsetattr(0,TCSANOW,&stored_settings);
 return in; 
 } 
********************/

int equid2index(QString id)
{
    int i;
    for(i=0;i<nequ;i++)
        if(id==equparaset[i].id)break;
    return i;

}

int kkno2index(QString no)
{
    int i;
    for(i=0;i<nkk;i++)
        if(no==kkparaset[i].no)break;
    return i;
}

int kktype2index(int type)
{
    int i;
    for(i=0;i<nkktype;i++)
        if(type==kktype[i].type)break;
    return i;
}

int kkposandequid2index(QString id,int pos)
{
    int i;
    for(i=0;i<nkk;i++)
        if(id==kkparaset[i].id&&pos==kkparaset[i].pos)break;
    return i;
}

int jk2initpos(QString id)
{

    int idx;
    CartesianPose cart;
    if(Jkstatus.is_socket_connect!=1)
    {
        //QMessageBox::information(this,"提示","JAKA控制器连接失败，请检查！");
        return 1;
    }
    idx=equid2index(id);
    cart.tran.x =equparaset[idx].x;
    cart.tran.y =equparaset[idx].y;
    cart.tran.z =equparaset[idx].z;
    cart.rpy.rx =equparaset[idx].rx;
    cart.rpy.ry =equparaset[idx].ry;
    cart.rpy.rz =equparaset[idx].rz;
    //笛卡尔空间运动,其中 ABS 代表绝对运动,FALSE 代表指令是非阻塞的,10 代表速度为 10mm/s
    //printf("rx=%f , ry=%f, rz=%f\n", cart.rpy.rx, cart.rpy.ry, cart.rpy.rz);
    JKrobot.linear_move(&cart, ABS, TRUE, 10);
    //打印笛卡尔空间位置

   // printf("errcode=%d \nx=%f, y=%f, z=%f\n", status.errcode,status.cartesiantran_position
   // [0], status.cartesiantran_position[1], status.cartesiantran_position[2]);

   // printf("rx=%f, ry=%f, rz=%f",status.cartesiantran_position[3], status.cartesiantran_po
  //  sition[4], status.cartesiantran_position[5]);
    return 0;
}

void rosinitpos()
{
    QJsonObject root_Obj;
    int sec,usec;
    struct timeval us;
    QString topic;
    //int idx;
    //q0="00";
    double quat[4],eu[3],coord[3];
    coord[0]=-0.34058359265327453613;//equparaset[idx].shx;
    coord[1]=-0.11496724188327789307;//equparaset[idx].shy;
    coord[2]=-0.49346005916595458984;//equparaset[idx].shz;
    quat[0] =0.02654558356676450098;//equparaset[idx].shx1;
    quat[1] =-0.03758920266706877039;//equparaset[idx].shy1;
    quat[2] =0.03435658208791222273;//equparaset[idx].shz1;//
    quat[3] =0.99834964271200621333;//equparaset[idx].shw1;

    quat2eular(quat,eu);
    eu[0]=0.0;
    eu[1]=0.0;
    eular2quat(eu,quat);


    gettimeofday(&us,NULL);
    sec=us.tv_sec;
    usec=us.tv_usec;
    double timestamp=sec;
    double ff=usec/1000000.0;
    timestamp=sec+ff;
    //qDebug()<<"gettimeofday: "<<ff<<sec<<QString::number(timestamp, 'f', 6) ;


    QJsonObject header;
    header.insert("seq",1);
    header.insert("stamp",timestamp);
    header.insert("frame_id","map");

    QJsonObject position;
    position.insert("x", coord[0]);
    position.insert("y", coord[1]);
    position.insert("z", coord[2]);

    QJsonObject orientation;
    orientation.insert("x", quat[0]);
    orientation.insert("y", quat[1]);
    orientation.insert("z", quat[2]);
    orientation.insert("w", quat[3]);

    QJsonObject pose;
    pose.insert("position", position);
    pose.insert("orientation", orientation);

    // QJsonObject pose1;
    // pose1.insert("pose",pose);


    QJsonObject msg;
    msg.insert("header",header);
    msg.insert("pose",pose);

    topic="/move_base_simple/goal";
    webcmd->pubcmd(topic,msg);
}


//----------------------------------------------------
void *Threadjaka(void *) //
{


    while(1)
    {
       // qDebug()<<"JAKA thread!"<<'\n';

      if(Jkstatus.is_socket_connect)JKrobot.get_robot_status(&Jkstatus);




       usleep(100);

    }//while 1


}

//--------------------------------------------------------------
 void *ThreadProcrcv(void *)//通信线程
{



    while(1)
	{
        //qDebug()<<"Recv thread!"<<'\n';





       usleep(100);
      
    }//while 1
}

//--------------------------------------------------------------
 void *Threadros(void *)//
{
    while(1)
    {
     // qDebug()<<"ROS thread!"<<'\n';








        usleep(100);
    }//while 1

}

 //--------------------------------------------------
 void *Threadbutton(void *)
 {
     while(1)
     {
         //qDebug()<<"Photo thread!"<<'\n';
         if(btnopt==1)
         {
             RobotInitialPosition();
             btnopt=0;
         }

         if(btnopt==2)
         {
             int kkidx,equidx;
             QString qsl,qst,equid;
             //gno=""
             //kkidx=kkno2index(gno);
             //equid=kkparaset[kkidx].id;
             equid="00";
             equidx=equid2index(equid);
             gbd=true;//展开标定
             int res=move2calibrationpos(equidx);
             btnopt=0;
         }



         usleep(100);
     }//while 1

 }
 //----------------------------------------------------
 void *ThreadProcess(void *)//处理线程
 {
     QSqlQuery query;
     bool value;
     QString qsl,qst,equid;
     int kkidx,equidx;
     static unsigned char rosGoalStatusold=0;
     static int jakainpos=0;
     //kkopt，gno，gstate，

     while(1)
     {
      //qDebug()<<"Photo thread!"<<'\n';

         if(step==1) //检测小车到位，到位后令step=2;步骤2：JAKA舒展
         {
//***************
             kkidx=kkno2index(gno);
             equid=kkparaset[kkidx].id;
             equidx=equid2index(equid);
             //if(equparaset[equidx].shx)//comp.cood:x,y,z,orientationx,y,z,w
             if(rosGoalStatusold==1&&rosGoalStatus==3)//if(rosGoalStatus==3)//ros inpos`
             {
                 QString topic="/move_base/status";
                 QString type="";
                 webcmd->unsubcmd(topic);//停止获取导航状态
                 usleep(100);
                 topic="/robot_pose";
                 type="geometry_msgs/Pose";
                 webcmd->subcmd(topic,type);//获取位姿
                 usleep(100);
                 //计算误差？？？？？？
                 ugv_dx=rospos.x-equparaset[equidx].shx;
                 ugv_dy=rospos.y-equparaset[equidx].shy;
                 ugv_dz=rospos.z-equparaset[equidx].shz;
                 ugv_drx=rospos.x1-equparaset[equidx].shx1;
                 ugv_dry=rospos.y1-equparaset[equidx].shy1;
                 ugv_drz=rospos.z1-equparaset[equidx].shz1;
                 ugv_drw=rospos.w1-equparaset[equidx].shw1;

                 //发JAKA舒展指令
                 //CartesianPose cart;
                 if(Jkstatus.is_socket_connect==1)
                 {

                    int res=move2calibrationpos(equidx);
                    //step=0;
                     step=2;
                 }
             }


         }

         if(step==2) //检测JAKA舒展，舒展到位后令step=3;步骤3：JAKA标定位
         {
             QString kkno[3];
             int kkst[3];
             kkno[0]=gno;//"001";
             kkst[0]=gstate;//1;
             kkno[1]="004";
             kkst[1]=1;
             kkno[2]="0013";
             kkst[2]=1;
             if(gtestauto)
             {
                 for(int i=0;i<3;i++)
                 {
                     gno=kkno[i];
                     gstate=kkst[i];
                     int idx=kkno2index(gno);
                     int res=operate(idx,gstate,gnum);
                     gnum=1;
                     //gnum;//operate中 修改为1
                     if(res==0)
                     {
                         //step=0;
                         kkparaset[idx].value=gstate;
                         qst=QString::number(gstate);
                         qsl="update kkpara set value='" + qst + "' where no='" + gno + "'";
                         value=query.exec(qsl);//"update equpara set x="+qx+",y="+qy+",z="+qz+",rx="+qrx+",ry="+qry+",rz="+qrz+" where id="+q0);
                         qDebug()<<"update kkpara:"<<value;
                         if(i==2)
                         {
                             RobotInitialPosition();
                             //rosinitpos();
                         }
                         //if(value) QMessageBox::information(this,"提示","状态修改成功！以后需要改到Process线程中...");
                         //else QMessageBox::information(this,"提示","状态修改失败！");
                        // RobotInitialPosition();

                     }
                     else
                     {
                        step=0;
                        gtestauto=false;
                        //gbd=false;
                        //RobotInitialPosition();
                     }
                     sleep(1);
                 }
                 step=0;
                 gtestauto=false;
                 //gbd=false;

             }
             else
             {
                 int idx=kkno2index(gno);
                 int res=operate(idx,gstate,gnum);
                 gnum=1;
                 //gnum;//operate中 修改为1
                 if(res==0)
                 {
                     step=0;
                     //gbd=false;
                     kkparaset[idx].value=gstate;
                     qst=QString::number(gstate);
                     qsl="update kkpara set value='" + qst + "' where no='" + gno + "'";
                     value=query.exec(qsl);//"update equpara set x="+qx+",y="+qy+",z="+qz+",rx="+qrx+",ry="+qry+",rz="+qrz+" where id="+q0);
                     //qDebug()<<"update kkpara:"<<value;
                     //if(value) QMessageBox::information(this,"提示","状态修改成功！以后需要改到Process线程中...");
                     //else QMessageBox::information(this,"提示","状态修改失败！");
                    // RobotInitialPosition();

                 }
                 else
                 {
                    step=0;
                    //gbd=false;
                    //RobotInitialPosition();
                 }
             }

         }


         /*******************
         if(step==3) //检测JAKA标定位，到位后令step=4;步骤4：JAKA手眼标定
         {
             CartesianPose cart;
             if(jakainpos==1&&Jkstatus.inpos==0)jakaopt=30;//Jaka move
            //JAKA标定位到位,标定
             if(jakainpos==0&&Jkstatus.inpos==1)//Jaka inpos
             {
                jakaopt=31;






                 step=4;
              }




            // step=4;

         }

         if(step==4) //检测手眼标定完成，完成后令step=5;步骤5：JAKA检测开关操作位
         {2
             CartesianPose cart;
             kkidx=kkno2index(gno);
             equid=kkparaset[kkidx].id;
             equidx=equid2index(equid);
            //检测手眼标定完成
            //计算开关操作位姿
             if(Jkstatus.is_socket_connect==1)//MOve to 开关操作位
             {

             //****************
             cart.tran.x =equparaset[equidx].x;
             cart.tran.y =equparaset[equidx].y;
             cart.tran.z =equparaset[equidx].z;
             cart.rpy.rx =equparaset[equidx].rx;
             cart.rpy.ry =equparaset[equidx].ry;
             cart.rpy.rz =equparaset[equidx].rz;
             //笛卡尔空间运动,其中 ABS 代表绝对运动,FALSE 代表指令是非阻塞的,10 代表速度为 10mm/s
             //printf("rx=%f , ry=%f, rz=%f\n", cart.rpy.rx, cart.rpy.ry, cart.rpy.rz);
             JKrobot.linear_move(&cart, ABS, TRUE, 10);
             //****************
             step=5;
             }


         }

         if(step==5)  //检测检测开关操作位，到位后令step=6;步骤6：JAKA操作开关
         {
             CartesianPose cart;
             kkidx=kkno2index(gno);
             equid=kkparaset[kkidx].id;
             e2quidx=equid2index(equid);
             if(jakainpos==1&&Jkstatus.inpos==0)jakaopt=50;//Jaka move
            //检测检测开关操作，到位
             if(jakainpos==0&&Jkstatus.inpos==1)//Jaka inpos
             {
                 jakaopt=51;//Jaka move
                 if(Jkstatus.is_socket_connect==1)//操作开关
                 {

                 //****************
                 cart.tran.x =equparaset[equidx].x;
                 cart.tran.y =equparaset[equidx].y;
                 cart.tran.z =equparaset[equidx].z;
                 cart.rpy.rx =equparaset[equidx].rx;
                 cart.rpy.ry =equparaset[equidx].ry;
                 cart.rpy.rz =equparaset[equidx].rz;
                 //笛卡尔空间运动,其中 ABS 代表绝对运动,FALSE 代表指令是非阻塞的,10 代表速度为 10mm/s
                 //printf("rx=%f , ry=%f, rz=%f\n", cart.rpy.rx, cart.rpy.ry, cart.rpy.rz);
                 JKrobot.linear_move(&cart, ABS, TRUE, 10);
                 //****************
                 step=6;
                 }

             }



         }

         if(step==6)  //检测操作开关到位，到位后令step=0;操作开关完成
         {
            if(jakainpos==1&&Jkstatus.inpos==0)jakaopt=60;//Jaka move
            //检测操作开关到位
             if(jakainpos==0&&Jkstatus.inpos==1)//Jaka inpos
             {
                 jakaopt=61;//Jaka move
                 //到位后，JAKA，小车回原位






                 //修改状态及记录
                 kkparaset[kkidx].value=gstate;
                 qst=QString::number(gstate);
                 qsl="update kkpara set value='" + qst + "' where no='" + gno + "'";
                 value=query.exec(qsl);
                 //if(value) QMessageBox::information(this,"提示","状态修改成功！");
                 //else QMessageBox::information(this,"提示","状态修改失败！");
                 jakaopt=0;
                 step=0;
             }



         }
         *************************/
         rosGoalStatusold=rosGoalStatus;
       //  jakainpos=Jkstatus.inpos;


         usleep(100000);
     }//while 1





 }




