include(../../common.pri)

QT       -= gui
QT       += network

TARGET = evap-protocol
TEMPLATE = lib

DEFINES += EVAPPROTOCOL_LIBRARY

SOURCES += \
    cmd/command.cpp \
    cmd/evapcommand.cpp \
    cmd/nocommand.cpp \
    cmd/okcommand.cpp \
    interfaces/evapserial.cpp \
    interfaces/evaptcp.cpp \
    msggetdeterminedfanspeed.cpp \
    msggetidinfo.cpp \
    msgmicrocontrollerreboot.cpp \
    msgreadblockaddress.cpp \
    msgreadindicatorstatus.cpp \
    msgselfcontrol.cpp \
    msgturnonindicators.cpp \
    msgreadaccidentregister.cpp \
    msgsetfanspeed.cpp \
    msggetfanspeed.cpp \
    msggetfanspeedcontrol.cpp \
    msgsetturnonps.cpp \
    msgsetturnonps_24v.cpp \
    msgsetturnon_12v_1.cpp \
    msgsetturnon_12v_2.cpp \
    msggetspscontrol.cpp \
    msggetzvscontrol.cpp \
    msggetbcmstate.cpp \
    msggetkpowercontrol.cpp \
    msggetpstemp.cpp \
    msgcalibrationadc.cpp \
    msgsetip_config.cpp \
    msgsetid.cpp

HEADERS += \
    evap-protocol_global.h \
    cmd/command.h \
    cmd/evapcommand.h \
    cmd/nocommand.h \
    cmd/okcommand.h \
    interfaces/evapinterface.h \
    interfaces/evapserial.h \
    interfaces/evaptcp.h \
    msggetdeterminedfanspeed.h \
    msggetidinfo.h \
    msgmicrocontrollerreboot.h \
    msgreadblockaddress.h \
    msgreadindicatorstatus.h \
    msgselfcontrol.h \
    msgturnonindicators.h \
    msgreadaccidentregister.h \
    msgsetfanspeed.h \
    msggetfanspeed.h \
    msggetfanspeedcontrol.h \
    msgsetturnonps.h \
    msgsetturnonps_24v.h \
    msgsetturnon_12v_1.h \
    msgsetturnon_12v_2.h \
    msggetspscontrol.h \
    msggetzvscontrol.h \
    msggetbcmstate.h \
    msggetkpowercontrol.h \
    msggetpstemp.h \
    msgcalibrationadc.h \
    msgsetip_config.h \
    msgsetid.h

include(../../lib.pri)
