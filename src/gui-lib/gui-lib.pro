include(../../common.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gui-lib
TEMPLATE = lib

DEFINES += GUILIB_LIBRARY

SOURCES += \
        notification/popup.cpp \
        utilsforgui.cpp \
        component/clicklabel.cpp \
        component/led.cpp \
        component/ledlabel.cpp \
        widgets/tablestates.cpp \
        widgets/tablewidget.cpp \
    widgets/tabsmartwidget.cpp

HEADERS += \
        gui-lib_global.h \ 
        notification/popup.h \
        utilsforgui.h \
        component/clicklabel.h \
        component/led.h \
        component/ledlabel.h \
        widgets/tablestates.h \
        widgets/tablewidget.h \
    widgets/tabsmartwidget.h

include(../../lib.pri)

FORMS += \
    widgets/tabsmartwidget.ui
