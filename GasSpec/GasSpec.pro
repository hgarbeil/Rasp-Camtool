#-------------------------------------------------
#
# Project created by QtCreator 2017-01-21T00:19:07
#
#-------------------------------------------------

QT       += core

QT       -= gui
QMAKE_CXXFLAGS += -std=c++11
QMAKE_LFLAGS += -std=c++11

TARGET = GasSpec
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    toplevel.cpp \
    ../camtool/Avaspec.cpp

HEADERS += \
    toplevel.h \
    ../camtool/Avaspec.h

INCLUDEPATH += /home/pi/workdir/camtool /usr/local/include/libavs

LIBS += -L/usr/local/include -lavs -lm
