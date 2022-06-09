#ifndef CAMERA_H
#define CAMERA_H
// Import QT libs, one for threads and one for images
#include <QThread>
#include <QImage>
// Import librealsense header
#include <librealsense2/rs.hpp>
//#include "ComData.h"

// Let's define our camera as a thread, it will be constantly running and sending frames to
// our main window
class Camera : public QThread
{
    Q_OBJECT
public:
    // We need to instantiate a camera with both depth and rgb resolution (as well as fps)
    Camera(int rgb_width, int rgb_height, int depth_width, int depth_height, int fps);
    ~Camera() {}
    // Member function that handles thread iteration
    void run();
    uchar *p;
    unsigned long len;
    QImage qimg;
    // If called it will stop the thread
    //void stop() {camera_running = false;}
    //bool getcamera(){return camera_running;}
    //void setcamera(bool runing){camera_running=runing;}
    QImage realsenseFrameToQImage(const rs2::frame& f);
private:
    // Realsense configuration structure, it will define streams that need to be opened
    rs2::config cfg;
    // Our pipeline, main object used by realsense to handle streams
    //rs2::pipeline pipe;
    // Frames returned by our pipeline, they will be packed in this structure
    rs2::frameset frames;
    // A bool that defines if our thread is running
    //bool camera_running = false;
signals:
    // A signal sent by our class to notify that there are frames that need to be processed
    void framesReady();//, QImage frameDepth);
};
// A function that will convert realsense frames to QImage
//QImage realsenseFrameToQImage(const rs2::frame& f);
#endif // CAMERA_H
