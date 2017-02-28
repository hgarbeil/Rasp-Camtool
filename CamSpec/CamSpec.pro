#-------------------------------------------------
#
# Project created by QtCreator 2016-12-09T02:02:46
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CamSpec
TEMPLATE = app


QMAKE_CXXFLAGS+= -std=c++11
QMAKE_LFLAGS +=  -std=c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    ../Avaspec.cpp \
    ../Cam.cpp \
    ../clientTCP.cpp \
    ../GPS.cpp \
    ../IMUThread.cpp \
    qcustomplot.cpp \
    myplot.cpp
    #Avantes.cpp

HEADERS  += mainwindow.h \
    ../Avaspec.h \
    ../Cam.h \
    ../clientTCP.h \
    ../GPS.h \
    ../IMUThread.h \
    qcustomplot.h \
    myplot.h
    #Avantes.h

FORMS    += mainwindow.ui


INCLUDEPATH += /home/pi/workdir/libgps/src /usr/local/include/libavs /home/pi/workdir/camtool

LIBS += -L/home/pi/workdir/libgps/build -L/usr/local/lib -lavs -lgps -lRTIMULib -lraspicam -ljpeg -lpthread -lm
