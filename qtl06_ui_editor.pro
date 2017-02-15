#-------------------------------------------------
#
# Project created by QtCreator 2014-06-04T12:45:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtl06_ui_editor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    find_dialog.cpp \
    replace_dialog.cpp

HEADERS  += mainwindow.h \
    find_dialog.h \
    replace_dialog.h

FORMS    += mainwindow.ui \
    find_dialog.ui \
    replace_dialog.ui

RESOURCES += \
    mainwindow.qrc
