TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += /usr/local/include \
                /usr/local/include/opencv
                /usr/local/include/opencv2
LIBS += /usr/local/lib/libopencv_* \

INCLUDEPATH += /usr/local/include/opencv4/opencv2\
               /usr/local/include/opencv4/opencv2/imgproc\
              /usr/local/include/opencv4
DEPENDPATH += /usr/local/include/opencv4/opencv2\

#use realsense sdk2.0
INCLUDEPATH += /usr/local/include/librealsense2
LIBS += -L/usr/loacl/lib \
        -lrealsense2

SOURCES += \
        main.cpp

HEADERS += \
        ../../detector/detector.h
