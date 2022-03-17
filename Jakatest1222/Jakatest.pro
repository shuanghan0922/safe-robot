#QT       += core gui
QT       += core gui sql network websockets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    Comdata.cpp \
    WebSocketClient.cpp \
    dialogauto.cpp \
    dialogequset.cpp \
    dialoggl.cpp \
    dialogjakamovetest.cpp \
    dialogkkset.cpp \
    dialogkktype.cpp \
    dialoglogin.cpp \
    dialogmanual.cpp \
    dialogparaset.cpp \
    dialogpose.cpp \
    dialogtest.cpp \
    dialogusergl.cpp \
    jkrobot.cpp \
    main.cpp \
    mainwindow.cpp \
    mygauge2.cpp \
    webcmd.cpp

HEADERS += \
    ComData.h \
    WebSocketClient.h \
    connect.h \
    dialogauto.h \
    dialogequset.h \
    dialoggl.h \
    dialogjakamovetest.h \
    dialogkkset.h \
    dialogkktype.h \
    dialoglogin.h \
    dialogmanual.h \
    dialogparaset.h \
    dialogpose.h \
    dialogtest.h \
    dialogusergl.h \
    jkrobot.h \
    mainwindow.h \
    mygauge2.h \
    stb_image_write.h \
    webcmd.h

FORMS += \
    dialogauto.ui \
    dialogequset.ui \
    dialoggl.ui \
    dialogjakamovetest.ui \
    dialogkkset.ui \
    dialogkktype.ui \
    dialoglogin.ui \
    dialogmanual.ui \
    dialogparaset.ui \
    dialogpose.ui \
    dialogtest.ui \
    dialogusergl.ui \
    mainwindow.ui

QMAKE_CFLAGS += -std=c++11
QMAKE_CXXFLAGS = $$QMAKE_CFLAGS

INCLUDEPATH += /usr/local/include \
                /usr/local/include/opencv
                /usr/local/include/opencv2

LIBS += /usr/local/lib/libopencv_* \


INCLUDEPATH += /usr/local/include/opencv4/opencv2\
               /usr/local/include/opencv4/opencv2/imgproc\
              /usr/local/include/opencv4


DEPENDPATH += /usr/local/include/opencv4/opencv2\

INCLUDEPATH += /usr/include/eigen3


#LIBS += -L/opencv-4.5.4/build/lib/ -lopencv_core -lopencv_highgui -lopencv_imgproc

LIBS += /usr/local/lib/libopencv_calib3d.so \
#/usr/local/lib/libopencv_contrib.so \
/usr/local/lib/libopencv_core.so \
#/usr/local/lib/libopencv_dnn.so \ //chg
/usr/local/lib/libopencv_features2d.so \
/usr/local/lib/libopencv_flann.so \
#/usr/local/lib/libopencv_gpu.so \
/usr/local/lib/libopencv_highgui.so \
/usr/local/lib/libopencv_imgcodecs.so \
/usr/local/lib/libopencv_imgproc.so \
/usr/local/lib/libopencv_ml.so \
#/usr/local/lib/libopencv_nonfree.so \
/usr/local/lib/libopencv_objdetect.so \
#/usr/local/lib/libopencv_ocl.so \
/usr/local/lib/libopencv_photo.so \
/usr/local/lib/libopencv_stitching.so \
/usr/local/lib/libopencv_videoio.so \
/usr/local/lib/libopencv_video.so \
/usr/local/lib/librealsense2.so

LIBS += -lglut -lGLU -lGL\
-lpthread\

LIBS+=-L/usr/local/lib -lglfw3 -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lGL -lpthread -ldl

#use realsense sdk2.0
INCLUDEPATH += /usr/local/include/librealsense2

LIBS += -L/usr/loacl/lib \
-lrealsense2


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += /home/xzh/Jakatest/inc
#LIBS += -L /home/xzh/Jakatest/lib/ -ljakaAPI
LIBS += -L $$PWD/lib/ -ljakaAPI

#LIBS += /home/xzh/Jakatest/lib/libjakaAPI.so

QMAKE_LFLAGS_DEBUG = "-Wl,-rpath,$$PWD/lib"
QMAKE_LFLAGS_RELEASE = "-Wl,-rpath,$$PWD/lib"

QMAKE_LFLAGS+=-no-pie
