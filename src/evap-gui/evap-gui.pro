include(../../common.pri)
include(../../app.pri)

QT       += core gui network

CONFIG += serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = evap-gui
TEMPLATE = app

VERSION = 1.0.0

SOURCES += \
    aboutdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    debugwindow.cpp \
    monitorwidget.cpp \
    controlwidget.cpp \
    setup/setupdialog.cpp \
    setup/setupipconfigwidget.cpp \
    setup/setidwidget.cpp \
    statusbar/idinfowidget.cpp \
    statusbar/setinterfacewidget.cpp \
    statusbar/setipconfigevapdialog.cpp

HEADERS += \
    aboutdialog.h \
    mainwindow.h \
    debugwindow.h \
    monitorwidget.h \
    controlwidget.h \
    settings.h \
    setup/setupdialog.h \
    setup/setupipconfigwidget.h \
    setup/setidwidget.h \
    statusbar/idinfowidget.h \
    statusbar/setinterfacewidget.h \
    statusbar/setipconfigevapdialog.h

FORMS += \
    aboutdialog.ui \
    mainwindow.ui \
    debugwindow.ui \
    monitorwidget.ui \
    controlwidget.ui \
    setup/setupdialog.ui \
    setup/setupipconfigwidget.ui \
    setup/setidwidget.ui \
    statusbar/idinfowidget.ui \
    statusbar/setinterfacewidget.ui \
    statusbar/setipconfigevapdialog.ui

LIBS += -lgui-lib -levap -levap-protocol -lqtserialport


# Установка bin файла
unix {
    target.path = $${INSTALL_BIN_PATH}/
    INSTALLS += target
}

RESOURCES += \
    ../../rc/ui/res_ui.qrc

