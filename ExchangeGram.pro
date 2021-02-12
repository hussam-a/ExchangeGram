#-------------------------------------------------
#
# Project created by QtCreator 2016-11-20T22:51:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ExchangeGram
TEMPLATE = app


QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
    image.cpp \
    base64.cpp \
    client.cpp \
    communication.cpp \
    message.cpp \
    packet.cpp \
    udpsocket.cpp \
    home.cpp \
    server.cpp \
    conversion_functions.cpp \
    testing_ui.cpp \
    starthere.cpp \
    mainwindows.cpp

HEADERS  += \
    image.h \
    includes.h \
    client.h \
    communication.h \
    message.h \
    packet.h \
    udpsocket.h \
    base64.h \
    home.h \
    conversion_functions.h \
    server.h \
    testing_ui.h \
    starthere.h \
    serverapplication.h \
    mainwindows.h

FORMS    += \
    home.ui \
    starthere.ui \
    mainwindows.ui
LIBS += -lboost_serialization

DISTFILES += \
    test.json \
    default.png

