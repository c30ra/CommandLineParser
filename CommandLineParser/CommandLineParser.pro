#-------------------------------------------------
#
# Project created by QtCreator 2016-11-22T16:23:35
#
#-------------------------------------------------

QT       -= core gui

TARGET = CommandLineParser
TEMPLATE = lib
CONFIG += staticlib

SOURCES += commandlineparser.cpp

HEADERS += commandlineparser.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
