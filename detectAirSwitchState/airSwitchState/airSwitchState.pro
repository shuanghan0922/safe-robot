TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

#libraries
unix: CONFIG += link_pkgconfig

#opencv
unix: PKGCONFIG += opencv

SOURCES += \
        main.cpp
