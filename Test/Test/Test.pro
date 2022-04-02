TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

#libraries
unix: CONFIG += link_pkgconfig

#opencv
unix: PKGCONFIG += opencv

#use realsense sdk2.0
INCLUDEPATH += /usr/local/include/librealsense2
LIBS += -L/usr/loacl/lib \
        -lrealsense2

SOURCES += \
        main.cpp

HEADERS += \
        ../../detector/detector.h
