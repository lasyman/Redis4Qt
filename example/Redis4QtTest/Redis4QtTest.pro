#-------------------------------------------------
#
# Project created by QtCreator 2017-07-19T17:09:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Redis4QtTest
TEMPLATE = app

INCLUDEPATH += $$PWD/../../include

SOURCES += main.cpp\
        example.cpp

HEADERS  += example.h

MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug

CONFIG += debug_and_release
CONFIG += debug_and_release_target
CONFIG += c++11

CONFIG(debug, debug|release){
    CONFIG -= release
    DESTDIR= $$PWD/../bin/debug
    LIBS += -L$$PWD/../../bin/debug \
            -lRedis4Qtd
}else{
    CONFIG -= debug
    DESTDIR= $$PWD/../bin/release
    LIBS += -L$$PWD/../../bin/release \
            -lRedis4Qt
}
