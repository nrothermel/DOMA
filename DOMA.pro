#-------------------------------------------------
#
# Project created by QtCreator 2016-03-31T12:46:38
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = DOMA
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        qcustomplot.cpp \
    Level2Snapshot.cpp \
    popup_window.cpp
HEADERS  += mainwindow.h \
        qcustomplot.h \
    mysql_functions.h \
    Level2Snapshot.h \
    popup_window.h \
    analysis_functions.h

FORMS    += mainwindow.ui \
    popup_window.ui


