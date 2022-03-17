#ifndef JKROBOT_H
#define JKROBOT_H

int Aspd();
int RobotConnect();
int RobotDisconnect();
double GetRobotPosition(double* add_x,double* add_y,double* add_z,double* add_rx,double* add_ry,double* add_rz);
double SetRobotPosition(double add_x,double add_y,double add_z,double add_rx,double add_ry,double add_rz);
int RobotInitialPosition();
int camerafix();
int move2calibrationpos(int equidx);
int button(int kkidx, int state, bool num);
int airswitch(int kkidx, int state, bool num);
int knob(int kkidx, int state, bool num);
 int operate (int kkidx, int state, bool num);
void tight();
void release();
#endif // JKROBOT_H
