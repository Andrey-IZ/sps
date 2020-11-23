include(../../common.pri)

QT       -= gui
QT      += network

TARGET = evap
TEMPLATE = lib

CONFIG += serialport

DEFINES += EVAP_LIBRARY

SOURCES += \
        messageworker.cpp \
        evapmessageservice.cpp

HEADERS += \
        evap_global.h \ 
        messageworker.h \
        thread.h \
        evapmessageservice.h

LIBS += -levap-protocol -lqtserialport

unix {
    #Установка путей динамических либ
    installinitscript.commands = sudo mkdir /etc/$${PRO_NAME}.conf.d; \
        echo "$${INSTALL_LIB_PATH}" > $${PRO_NAME}.so.conf ; \
        sudo mv $${PRO_NAME}.so.conf /etc/ld.so.conf.d ; \
        sudo ldconfig;
    install.depends += installinitscript
    QMAKE_EXTRA_TARGETS += installinitscript install
}

include(../../lib.pri)
