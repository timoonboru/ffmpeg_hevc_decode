#-------------------------------------------------
#
# Project created by QtCreator 2018-06-04T18:32:37
#
#-------------------------------------------------

QT        += concurrent core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ffmpeg64_HEVC_decode
TEMPLATE = app

INCLUDEPATH += D:/FFMPEG/ffmpeg_4.0_64/dev/include \
                D:/opencv/build/include \
                D:/opencv/build/include/opencv \
                D:/opencv/build/include/opencv2
#LIBS += -LD:/FFMPEG/ffmpeg_4.0_64/dev/lib/ -lavcodec -lavformat -lswscale -lavutil \
#        -LD:/opencv/build/x64/vc12/lib/ -lopencv_imgproc2410

LIBS += D:/opencv/build/x64/vc12/lib/opencv_calib3d2410d.lib \
D:/opencv/build/x64/vc12/lib/opencv_contrib2410d.lib \
D:/opencv/build/x64/vc12/lib/opencv_core2410d.lib \
D:/opencv/build/x64/vc12/lib/opencv_features2d2410d.lib \
D:/opencv/build/x64/vc12/lib/opencv_flann2410d.lib \
D:/opencv/build/x64/vc12/lib/opencv_gpu2410d.lib \
D:/opencv/build/x64/vc12/lib/opencv_highgui2410d.lib \
D:/opencv/build/x64/vc12/lib/opencv_imgproc2410d.lib \
D:/opencv/build/x64/vc12/lib/opencv_legacy2410d.lib \
D:/opencv/build/x64/vc12/lib/opencv_ml2410d.lib \
D:/opencv/build/x64/vc12/lib/opencv_nonfree2410d.lib \
D:/opencv/build/x64/vc12/lib/opencv_objdetect2410d.lib \
D:/opencv/build/x64/vc12/lib/opencv_ocl2410d.lib \
D:/opencv/build/x64/vc12/lib/opencv_photo2410d.lib \
D:/opencv/build/x64/vc12/lib/opencv_stitching2410d.lib \
D:/opencv/build/x64/vc12/lib/opencv_superres2410d.lib \
D:/opencv/build/x64/vc12/lib/opencv_ts2410d.lib \
D:/opencv/build/x64/vc12/lib/opencv_video2410d.lib \
D:/opencv/build/x64/vc12/lib/opencv_videostab2410d.lib \
D:/FFMPEG/ffmpeg_4.0_64/dev/lib/avcodec.lib \
D:/FFMPEG/ffmpeg_4.0_64/dev/lib/avformat.lib \
D:/FFMPEG/ffmpeg_4.0_64/dev/lib/swscale.lib \
D:/FFMPEG/ffmpeg_4.0_64/dev/lib/avutil.lib \
D:/FFMPEG/ffmpeg_4.0_64/dev/lib/avdevice.lib

SOURCES += main.cpp \
    mainwindow.cpp \
    imgshow.cpp

HEADERS  += mainwindow.h \
    imgshow.h

FORMS    += mainwindow.ui
