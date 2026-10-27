#-------------------------------------------------
#
# Project created by QtCreator 2014-01-30T20:04:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tyu2calc
TEMPLATE = app

TRANSLATIONS = tyu2calc.ts

DESTDIR = ../out

SOURCES += main.cpp\
	MainWindow.cpp \
    AccelaTime.cpp \
    SpeedConv.cpp \
    QDoubleEdit.cpp \
    Satellite.cpp \
    DistanceConv.cpp \
    QSpeedEdit.cpp \
    QAccelerEdit.cpp \
    QDistanceEdit.cpp \
    Acceleration1.cpp \
    Acceleration2.cpp \
    ../../Fs/FsPref.cpp \
    ../../Fs/FsP.cpp \
    QTimeValueEdit.cpp \
    CentrifugalForce.cpp \
    QWeightEdit.cpp \
    FreeFallTime.cpp \
    FreeFallDistance.cpp

HEADERS  += MainWindow.h \
    FsSpace.h \
    AccelaTime.h \
    SpeedConv.h \
    QDoubleEdit.h \
    Satellite.h \
    DistanceConv.h \
    QSpeedEdit.h \
    QAccelerEdit.h \
    QDistanceEdit.h \
    Acceleration1.h \
    Acceleration2.h \
    ../../Fs/FsPref.h \
    ../../Fs/FsP.h \
    QTimeValueEdit.h \
    CentrifugalForce.h \
    QWeightEdit.h \
    FreeFallTime.h \
    FreeFallDistance.h \
    FsSpeed.h \
    FsDistance.h \
    FsAcceleration.h \
    FsTime.h

FORMS +=

OTHER_FILES +=

RESOURCES +=
