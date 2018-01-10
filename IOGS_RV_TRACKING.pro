#-------------------------------------------------
#
# Project created by QtCreator 2014-10-03T11:50:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IOGS_RV_TRACKING
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialogimg.cpp

HEADERS  += mainwindow.h \
    dialogimg.h

INCLUDEPATH += /usr/local/include/opencv2

LIBS += -L/usr/local/lib
LIBS += -lopencv_core
LIBS += -lopencv_imgproc
LIBS += -lopencv_highgui
LIBS += -lopencv_ml
LIBS += -lopencv_video
LIBS += -lopencv_features2d
LIBS += -lopencv_calib3d
LIBS += -lopencv_objdetect
LIBS += -lopencv_contrib
LIBS += -lopencv_legacy
LIBS += -lopencv_flann

FORMS    += mainwindow.ui \
    dialogimg.ui
