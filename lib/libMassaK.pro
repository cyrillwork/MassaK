TARGET = MassaK
TEMPLATE = lib

INCLUDEPATH += ./serial

SOURCES += \
    serial/serial_factory.cpp \
    #serial/rsinterface.cpp \
    serial/paramsrs.cpp \
    driver.cpp \
    #driver_plain.cpp \
    controller.cpp \
    checkcomports.cpp \
    protocol.cpp

windows {
    DEFINES += MASSAK_WINDOWS
    SOURCES += serial/winserial.cpp
} else {
    SOURCES += serial/linuxserial.cpp
}

    
#QMAKE_CXXFLAGS += -O2 -fPIC

QMAKE_CXXFLAGS += -std=c++17 -fPIC
CONFIG += c++17

windows {
    QMAKE_CXXFLAGS += -mno-ms-bitfields
}


LIBS += -lpthread

HEADERS += \
    #serial/iinterface.h \
    serial/iserial.h \
    serial/serial_factory.h \
    #serial/rsinterface.h \
    serial/paramsrs.h \
    aixlog.hpp \
    driver.h \
    #driver_plain.h \
    controller.h \
    checkcomports.h \
    scales_parameters.h \
    protocol.h \
    messages.h

windows {
        HEADERS +=  serial/win_serial.h \
                    serial/winserial.h
} else {
        HEADERS +=  serial/linuxserial.h
}
