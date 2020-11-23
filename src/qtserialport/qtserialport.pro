
include(../../common.pri)
QT = core

QMAKE_DOCS = $$PWD/doc/qtserialport.qdocconf
include($$PWD/serialport-lib.pri)

TEMPLATE = lib
TARGET = qtserialport

DEFINES += QT_BUILD_SERIALPORT_LIB

CONFIG += module create_prl

PRECOMPILED_HEADER =


include(../../lib.pri)
