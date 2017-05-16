#-------------------------------------------------
#
# Project created by QtCreator 2016-10-20T09:42:01
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VLCDemo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
#    vlcpainter.cpp \
#    vlcplayer.cpp \
    graphicsview.cpp \
    graphicsscene.cpp \
    graphicsitembase.cpp \
    bdslimit30traficsign.cpp \
    bdsrounditemtraficsign.cpp

HEADERS  += mainwindow.h \
#    vlcpainter.h \
#    vlcplayer.h \
    graphicsview.h \
    graphicsscene.h \
    graphicsitembase.h \
    bdslimit30traficsign.h \
    bdsrounditemtraficsign.h

FORMS    += mainwindow.ui \
#    vlcplayer.ui

LIBS += -lvlc
