#-------------------------------------------------
#
# Project created by QtCreator 2017-07-06T15:17:08
#
#-------------------------------------------------

QT       += network

QT       -= gui
TEMPLATE = lib
include(../Redis4Qt.pri)
DEFINES += REDIS4QT_LIBRARY

MOC_DIR += ../src/GeneratedFiles/debug
OBJECTS_DIR += debug

RCC_DIR += ../src/GeneratedFiles

CONFIG += debug_and_release
CONFIG += debug_and_release_target
CONFIG += c++11

CONFIG(debug, debug|release){
    CONFIG -= release
    DESTDIR= $$PWD/../bin/debug
    TARGET = Redis4Qtd
}else{
    CONFIG -= debug
    DESTDIR= $$PWD/../bin/release
    TARGET = Redis4Qtd
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}

HEADERS +=
