DESTDIR = $${LIBS_PATH}/
QMAKE_CLEAN += $${LIBS_PATH}/lib$${TARGET}.*

win32 {
    copydata.commands = $(COPY_FILE)  $${LIBS_PATH}\\$${TARGET}.dll $${BIN_PATH}
    QMAKE_POST_LINK +=  $(COPY_FILE)  $${LIBS_PATH}\\$${TARGET}.dll $${BIN_PATH}
}

unix {
    target.path = $${INSTALL_LIB_PATH}/
    INSTALLS += target
}

