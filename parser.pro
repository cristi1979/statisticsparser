# -------------------------------------------------
# Project created by QtCreator 2009-12-22T18:58:29
# -------------------------------------------------
QT -= gui
QT += testlib
TARGET = parser
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += main.cpp \
    rrdtool.cpp \
    parse.cpp \
    parseiostat.cpp \
    parsedfstat.cpp \
    parsefanstat.cpp \
    parseprtpicl.cpp \
    parsesarstat.cpp \
    parsempstat.cpp
HEADERS += rrdtool.h \
    parse.h \
    parseiostat.h \
    parsedfstat.h \
    parsefanstat.h \
    parseprtpicl.h \
    parsesarstat.h \
    parsempstat.h
