TARGET = MassaK
TEMPLATE = lib

SOURCES += \
    driver.cpp \
    driver_plain.cpp \
    controller.cpp \
    checkcomports.cpp \
    protocol.cpp
    
#QMAKE_CXXFLAGS += -O2 -fPIC

QMAKE_CXXFLAGS += -std=c++17
CONFIG += c++17

LIBS += -lpthread

HEADERS += \
    aixlog.hpp \
    driver.h \
    driver_plain.h \
    controller.h \
    checkcomports.h \
    protocol.h \
    messages.h

#DEFINES += MULTI_GAME
