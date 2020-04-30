#-------------------------------------------------
#
# Project created by QtCreator 2012-03-05T10:38:43
#
#-------------------------------------------------

QT += network

QT -= gui

TARGET = QtWebsocket
TEMPLATE = lib
CONFIG += shared dll
DEFINES += QWS_LIBRARY

SOURCES += \
    QWsServer.cpp \
    QWsSocket.cpp \
    QWsHandshake.cpp \
    QWsFrame.cpp \
    functions.cpp

HEADERS += \
    QWsServer.h \
    QWsSocket.h \
    QWsHandshake.h \
    QWsFrame.h \
    functions.h \
    WsEnums.h
