TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += /usr/local/include \
                /usr/local/include/opencv
                /usr/local/include/opencv2/*
LIBS += /usr/local/lib/libopencv_*


#use realsense sdk2.0
INCLUDEPATH += /usr/local/include/librealsense2
LIBS += -L/usr/loacl/lib \
        -lrealsense2

SOURCES += \
        ../../detector/detector.cpp \
        main.cpp

HEADERS += \
        ../../detector/detector.h
