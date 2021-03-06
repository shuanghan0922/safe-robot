#include "videodevice.h"
#include <iostream>

VideoDevice::VideoDevice(QString dev_name)
{
    QString temp_dev_name = dev_name;
    this->dev_name = temp_dev_name;
    this->fd = -1;
    this->buffers = NULL;
    this->n_buffers = 0;
    this->index = -1;
    //display_error(tr("dev_name"));
}

int VideoDevice::open_device()
{
    fd = open(dev_name.toStdString().c_str(), O_RDWR, 0);
   // fd = open(dev_name.toStdString().c_str(), O_RDWR|O_NONBLOCK, 0);
   // QString tmp;
 //display_error(tmp.setNum(fd));

    if(-1 == fd)
    {
        emit display_error(tr("open: %1").arg(QString(strerror(errno))));
        return -1;
    }
    return 0;
}

int VideoDevice::close_device()
{
    if(-1 == close(fd))
    {
        emit display_error(tr("close: %1").arg(QString(strerror(errno))));
        return -1;
    }
    return 0;
}

int VideoDevice::init_device()
{
    v4l2_capability cap;
    v4l2_cropcap cropcap;
    v4l2_crop crop;
    v4l2_format fmt;
    v4l2_std_id std;
    if(-1 == ioctl(fd, VIDIOC_QUERYCAP, &cap))
    {
        if(EINVAL == errno)
        {
            emit display_error(tr("%1 is no V4l2 device").arg(dev_name));
        }
        else
        {
            emit display_error(tr("VIDIOC_QUERYCAP: %1").arg(QString(strerror(errno))));
        }
        return -1;
    }
    ioctl(fd,VIDIOC_QUERYSTD,&std);
    switch(std){
            case V4L2_STD_NTSC:
                fprintf(stderr,"V4L2_STD_NTSC\n");
                break;
            case V4L2_STD_PAL:
                fprintf(stderr,"V4L2_STD_PAL\n");
            default:
                fprintf(stderr,"VIDIOC_QUERYSTD std = %d\n",std);
                break;
        }


    if(!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE))
    {
        emit display_error(tr("%1 is no video capture device").arg(dev_name));
        return -1;
    }

    if(!(cap.capabilities & V4L2_CAP_STREAMING))
    {
        emit display_error(tr("%1 does not support streaming i/o").arg(dev_name));
        return -1;
    }

    CLEAR(cropcap);

    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    //if(0)
     if(0 == ioctl(fd, VIDIOC_CROPCAP, &cropcap))
    {
        CLEAR(crop);
        crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        crop.c = cropcap.defrect;

        if(-1 == ioctl(fd, VIDIOC_S_CROP, &crop))
       {
      //      if(EINVAL == errno)
      //      {
//                emit display_error(tr("VIDIOC_S_CROP not supported"));
       //     }
       //     else
        //    {
        //        emit display_error(tr("VIDIOC_S_CROP: %1").arg(QString(strerror(errno))));
        //        return -1;
        //    }
       }
    }
    else
    {
   //     emit display_error(tr("VIDIOC_CROPCAP: %1").arg(QString(strerror(errno))));
    //    return -1;
    }

   /* crop.c.height = 640; //?????????????????????
    crop.c.width = 480; //??????
    crop.c.left = 0; //?????????
    crop.c.top = 0;
    crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    */
     struct v4l2_streamparm *parm = new struct v4l2_streamparm;
          memset(parm,0,sizeof(struct v4l2_streamparm));
          parm->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
         // parm->parm.capture.capturemode = V4L2_MODE_HIGHQUALITY;
          parm->parm.capture.capability = V4L2_CAP_TIMEPERFRAME;
          parm->parm.capture.timeperframe.denominator = 40 ;//??????????????????
          parm->parm.capture.timeperframe.numerator = 1;//??????
          if(-1 == ioctl(fd,VIDIOC_S_PARM,parm))
          {
             std::cout<<"Failed to set frame rate"<<std::endl;
              return false;
          }else
          {
              ioctl(fd, VIDIOC_G_PARM, parm);
              int x = parm->parm.capture.timeperframe.denominator;
              int y = parm->parm.capture.timeperframe.numerator;
              int frame = y*1000/x;
              std::cout<<"current frame rate is:"<<x<<" "<<y<<" "<<frame<<std::endl;
          }
    CLEAR(fmt);
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (-1 == ioctl(fd, VIDIOC_G_FMT, &fmt))
           std::cout<<"can not get format"<<std::endl;
    std::cout << "format(num): "<<fmt.fmt.pix.pixelformat<<std::endl;
    std::cout<<"width:"<<fmt.fmt.pix.width<<std::endl;
    std::cout<<"height:"<<fmt.fmt.pix.height<<std::endl;
    std::cout<<"field:"<<fmt.fmt.pix.field<<std::endl;
    std::cout<<"bytesperline:"<<fmt.fmt.pix.bytesperline<<std::endl;
    std::cout<<"sizeimage:"<<fmt.fmt.pix.sizeimage<<std::endl;
    std::cout<<"colorspace:"<<fmt.fmt.pix.colorspace<<std::endl;
    std::cout<<"private field:"<<fmt.fmt.pix.priv<<std::endl;
    std::cout<<"get fomat OK!"<<std::endl;
   // CLEAR(fmt);

    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = 320;
    fmt.fmt.pix.height = 240;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;//fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;

    if(-1 == ioctl(fd, VIDIOC_S_FMT, &fmt))
    {
        emit display_error(tr("VIDIOC_S_FMT11").arg(QString(strerror(errno))));
        return -1;
    }

    if(-1 == init_mmap())
    {
        return -1;
    }

    return 0;
}

int VideoDevice::init_mmap()
{
    v4l2_requestbuffers req;
    CLEAR(req);

    req.count = 4;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if(-1 == ioctl(fd, VIDIOC_REQBUFS, &req))
    {
        if(EINVAL == errno)
        {
            emit display_error(tr("%1 does not support memory mapping").arg(dev_name));
            return -1;
        }
        else
        {
            emit display_error(tr("VIDIOC_REQBUFS %1").arg(QString(strerror(errno))));
            return -1;
        }
    }

    if(req.count < 2)
    {
        emit display_error(tr("Insufficient buffer memory on %1").arg(dev_name));
        return -1;
    }
    fprintf(stderr,"req.count = %d\n",req.count);
    buffers = (buffer*)calloc(req.count, sizeof(*buffers));

    if(!buffers)
    {
        emit display_error(tr("out of memory"));
        return -1;
    }

    for(n_buffers = 0; n_buffers < req.count; ++n_buffers)
    {
        v4l2_buffer buf;
        CLEAR(buf);

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = n_buffers;

        if(-1 == ioctl(fd, VIDIOC_QUERYBUF, &buf))
        {
            emit display_error(tr("VIDIOC_QUERYBUF: %1").arg(QString(strerror(errno))));
            return -1;
        }

        buffers[n_buffers].length = buf.length;
        buffers[n_buffers].start =
                mmap(NULL, // start anywhere
                     buf.length,
                     PROT_READ | PROT_WRITE,
                     MAP_SHARED,
                     fd, buf.m.offset);

        if(MAP_FAILED == buffers[n_buffers].start)
        {
            emit display_error(tr("mmap %1").arg(QString(strerror(errno))));
            return -1;
        }
    }
    return 0;

}

int VideoDevice::start_capturing()
{
    unsigned int i;
    for(i = 0; i < n_buffers; ++i)
    {
        v4l2_buffer buf;
        CLEAR(buf);

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory =V4L2_MEMORY_MMAP;
        buf.index = i;

        if(-1 == ioctl(fd, VIDIOC_QBUF, &buf))
        {
            emit display_error(tr("VIDIOC_QBUF: %1").arg(QString(strerror(errno))));
            return -1;
        }
    }

    v4l2_buf_type type;
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if(-1 == ioctl(fd, VIDIOC_STREAMON, &type))
    {
        emit display_error(tr("VIDIOC_STREAMON: %1").arg(QString(strerror(errno))));
        return -1;
    }
    return 0;
}

int VideoDevice::stop_capturing()
{
    v4l2_buf_type type;
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if(-1 == ioctl(fd, VIDIOC_STREAMOFF, &type))
    {
        emit display_error(tr("VIDIOC_STREAMOFF: %1").arg(QString(strerror(errno))));
        return -1;
    }
    return 0;
}

int VideoDevice::uninit_device()
{
    unsigned int i;
    for(i = 0; i < n_buffers; ++i)
    {
        if(-1 == munmap(buffers[i].start, buffers[i].length))
        {
            emit display_error(tr("munmap: %1").arg(QString(strerror(errno))));
            return -1;
        }

    }
    free(buffers);
    return 0;
}

int VideoDevice::get_frame(void **frame_buf, size_t* len)
{
    v4l2_buffer queue_buf;
    CLEAR(queue_buf);

    queue_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    queue_buf.memory = V4L2_MEMORY_MMAP;

    if(-1 == ioctl(fd, VIDIOC_DQBUF, &queue_buf))
    {
        switch(errno)
        {
        case EAGAIN:
//            perror("dqbuf");
            return -1;
        case EIO:
            return -1 ;
        default:
            emit display_error(tr("VIDIOC_DQBUF: %1").arg(QString(strerror(errno))));
            return -1;
        }
    }

    *frame_buf = buffers[queue_buf.index].start;
    *len = buffers[queue_buf.index].length;
    index = queue_buf.index;
   // std::cout << "a frame" << std::endl;
    return 0;

}

int VideoDevice::unget_frame()
{
    if(index != -1)
    {
        v4l2_buffer queue_buf;
        CLEAR(queue_buf);

        queue_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        queue_buf.memory = V4L2_MEMORY_MMAP;
        queue_buf.index = index;

        if(-1 == ioctl(fd, VIDIOC_QBUF, &queue_buf))
        {
            emit display_error(tr("VIDIOC_QBUF: %1").arg(QString(strerror(errno))));
            return -1;
        }
        return 0;
    }
    return -1;
}


