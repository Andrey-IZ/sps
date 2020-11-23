PROJ_ROOT_PATH = $${PWD}

PRO_NAME=evap

unix {
    LIBS_PATH = $${PROJ_ROOT_PATH}/lib
    BIN_PATH = $${PROJ_ROOT_PATH}/bin
}
win32 {
    PROJ_ROOT_PATH = $$replace(PROJ_ROOT_PATH, /, \\)
    LIBS_PATH = $${PROJ_ROOT_PATH}\\lib
    BIN_PATH = $${PROJ_ROOT_PATH}\\bin
}

INC_PATH = $${PROJ_ROOT_PATH}/include


INSTALL_BIN_PATH = /usr/local/bin
INSTALL_ETC_PATH = /usr/local/$${PRO_NAME}/etc
INSTALL_LIB_PATH = /usr/local/$${PRO_NAME}/lib

QMAKE_CXXFLAGS += -std=c++0x
LIBS += -L$${LIBS_PATH}
INCLUDEPATH += $$INC_PATH
