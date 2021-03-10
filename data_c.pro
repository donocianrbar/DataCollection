QT += core
QT -= gui

CONFIG += c++11

TARGET = data_c
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += -I/usr/local/include \
               -I/usr/local/include/opencv \
               -I/usr/local/include/opencv2

LIBS += -L/usr/local/lib -lopencv_ml -lopencv_dnn -lopencv_objdetect -lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_photo -lopencv_shape -lopencv_video -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_imgproc -lopencv_flann -lopencv_core

SOURCES += main.cpp \
    DataCollect.cpp

HEADERS += \
    DataCollect.h
