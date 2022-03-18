#include "camera.h"
#include <QImage>
#include <QDebug>
#include "ComData.h"
//#include "processImage.h"
Camera::Camera(int rgb_width, int rgb_height, int depth_width, int depth_height, int fps)
{
    // Enable depth stream with given resolution. Pixel will have a bit depth of 16 bit
    //cfg.enable_stream(RS2_STREAM_DEPTH, depth_width, depth_height, RS2_FORMAT_Z16, fps);

    // Enable RGB stream as frames with 3 channel of 8 bit
    cfg.enable_stream(RS2_STREAM_COLOR, rgb_width, rgb_height, RS2_FORMAT_RGB8, fps);
    // Start our pipeline
    try
    {
        pipe1.start();//cfg
    }
    catch(...)
    {
      qDebug()<<"ERROR:";//<<exception;

    }

}

void Camera::run()
{
    while(1)
    {
     while(camera_running)
     {
        // Wait for frames and get them as soon as they are ready
        frames = pipe1.wait_for_frames();
        // Let's get our depth frame
        //rs2::depth_frame depth = frames.get_depth_frame();
        // And our rgb frame
        rs2::frame rgb = frames.get_color_frame();
        // Let's convert them to QImage
        //auto q_rgb = realsenseFrameToQImage(rgb);
        qimg=realsenseFrameToQImage(rgb);
        //auto q_depth = realsenseFrameToQImage(depth);
        // And finally we'll emit our signal
        //emit framesReady();//, q_depth);
     }
     if(!camera_running) usleep(100);
    }
}

QImage Camera::realsenseFrameToQImage(const rs2::frame &f)
{
    using namespace rs2;
    auto vf = f.as<video_frame>();
    const int w = vf.get_width();
    const int h = vf.get_height();
    if (f.get_profile().format() == RS2_FORMAT_RGB8)
    {
        auto r = QImage((uchar*) f.get_data(), w, h, w*3, QImage::Format_RGB888);
        len=f.get_data_size();
        p=(uchar*) f.get_data();
        //qDebug()<<"w,h,len:"<<w<<","<<h<<","<<len;
        return r;
    }
   // else if (f.get_profile().format() == RS2_FORMAT_Z16)
   // {
        // only if you have Qt > 5.13
       // auto r = QImage((uchar*) f.get_data(), w, h, w*2, QImage::Format_Grayscale16);
        //return r;
   // }
    throw std::runtime_error("Frame format is not supported yet!");
}

