TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

#libraries
unix: CONFIG += link_pkgconfig

#opencv
unix: PKGCONFIG += opencv

SOURCES += \
        extractGraph/extractGraph.cpp \
        main.cpp \
        multipleImage/multipleImage.cpp

HEADERS += \
    extractGraph/extractGraph.h \
    multipleImage/multipleImage.h
