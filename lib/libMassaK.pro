TARGET = MassaK
TEMPLATE = lib

SOURCES += \
    driver.cpp \
    controller.cpp \
    checkcomports.cpp \    
    protocol.cpp
    
#QMAKE_CXXFLAGS += -O2 -fPIC

QMAKE_CXXFLAGS += -std=c++17
CONFIG += c++17

LIBS += -lpthread

HEADERS += \
    driver.h \
    controller.h \
    checkcomports.h \
    protocol.h \
    messages.h

#DEFINES += MULTI_GAME
