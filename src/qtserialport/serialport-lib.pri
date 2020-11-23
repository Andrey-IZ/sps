INCLUDEPATH += $$PWD \
    $$PWD/qt4support/include \
    $$PWD/qt4support/include/private \
    $$PWD/qt4support/include/QtCore

unix {
    packagesExist(libudev) {
        CONFIG += link_pkgconfig
        DEFINES += LINK_LIBUDEV
        PKGCONFIG += libudev
    }
}

unix {
    SOURCES += \
        $$PWD/qt4support/src/qlockfile.cpp \
        $$PWD/qt4support/src/qlockfile_unix.cpp
}

PUBLIC_HEADERS += \
    $$PWD/qserialportglobal.h \
    $$PWD/qserialport.h \
    $$PWD/qserialportinfo.h

PRIVATE_HEADERS += \
    $$PWD/qserialport_p.h \
    $$PWD/qserialportinfo_p.h

SOURCES += \
    $$PWD/qserialport.cpp \
    $$PWD/qserialportinfo.cpp

win32:!wince* {
    PRIVATE_HEADERS += \
        $$PWD/qserialport_win_p.h

    SOURCES += \
        $$PWD/qserialport_win.cpp \
        $$PWD/qserialportinfo_win.cpp

    LIBS_PRIVATE += -lsetupapi -ladvapi32
}

wince* {
    PRIVATE_HEADERS += \
        $$PWD/qserialport_wince_p.h

    SOURCES += \
        $$PWD/qserialport_wince.cpp \
        $$PWD/qserialportinfo_wince.cpp
}

unix: {
    PRIVATE_HEADERS += \
        $$PWD/qserialport_unix_p.h

    SOURCES += \
        $$PWD/qserialport_unix.cpp

    !mac {
        SOURCES += \
            $$PWD/qserialportinfo_unix.cpp
    } else {
        SOURCES += \
            $$PWD/qserialportinfo_mac.cpp

        LIBS_PRIVATE += -framework IOKit -framework CoreFoundation
    }
}

HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS
