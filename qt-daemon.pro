QT += core
QT -= gui
QT += network #sockets

CONFIG += c++11

TARGET = qt-daemon
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    logger.cpp \
    server.cpp

HEADERS += \
    logger.h \
    server.h
