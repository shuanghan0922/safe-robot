TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += opencv

#INCLUDEPATH += /usr/local/include \
#                /usr/local/include/opencv
#                /usr/local/include/opencv2

#LIBS += /usr/local/lib/libopencv_* \


#INCLUDEPATH += /usr/local/include/opencv4/opencv2\
#               /usr/local/include/opencv4/opencv2/imgproc\
#              /usr/local/include/opencv4


#DEPENDPATH += /usr/local/include/opencv4/opencv2\

#INCLUDEPATH += /usr/include/eigen3


##LIBS += -L/opencv-4.5.4/build/lib/ -lopencv_core -lopencv_highgui -lopencv_imgproc

#LIBS += /usr/local/lib/libopencv_calib3d.so \
##/usr/local/lib/libopencv_contrib.so \
#/usr/local/lib/libopencv_core.so \
##/usr/local/lib/libopencv_dnn.so \ //chg
#/usr/local/lib/libopencv_features2d.so \
#/usr/local/lib/libopencv_flann.so \
##/usr/local/lib/libopencv_gpu.so \
#/usr/local/lib/libopencv_highgui.so \
#/usr/local/lib/libopencv_imgcodecs.so \
#/usr/local/lib/libopencv_imgproc.so \
#/usr/local/lib/libopencv_ml.so \
##/usr/local/lib/libopencv_nonfree.so \
#/usr/local/lib/libopencv_objdetect.so \
##/usr/local/lib/libopencv_ocl.so \
#/usr/local/lib/libopencv_photo.so \
#/usr/local/lib/libopencv_stitching.so \
#/usr/local/lib/libopencv_videoio.so \
#/usr/local/lib/libopencv_video.so \

SOURCES += \
        ../../detector/detector.cpp \
        calibration/calibration.cpp \
        main.cpp

HEADERS += \
        ../../detector/detector.h \
        calibration/calibration.h
