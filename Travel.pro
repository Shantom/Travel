#-------------------------------------------------
#
# Project created by QtCreator 2016-03-19T21:10:56
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Travel
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    route.cpp \
    passenger.cpp \
    timetable.cpp

HEADERS  += widget.h \
    route.h \
    passenger.h \
    timetable.h

FORMS    += widget.ui \
    route.ui

RESOURCES += \
    src.qrc
