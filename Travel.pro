#-------------------------------------------------
#
# Project created by QtCreator 2016-03-19T21:10:56
#
#-------------------------------------------------

#CONFIG+=precompile_header
#PRECOMPILED_HEADER = stable.h

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Travel
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    route.cpp \
    passenger.cpp \
    timetable.cpp \
    graph.cpp \
    timer.cpp

HEADERS  += widget.h \
    route.h \
    passenger.h \
    timetable.h \
    graph.h \
    stable.h \
    timer.h

FORMS    += widget.ui \
    route.ui

RESOURCES += \
    src.qrc
