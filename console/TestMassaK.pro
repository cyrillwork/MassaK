
QMAKE_CXXFLAGS += -std=c++17
CONFIG += c++17

CONFIG += console

CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lpthread

LIBS += -rdynamic -ldl

TEMPLATE = app
TARGET = TestMassaK

INCLUDEPATH += .
INCLUDEPATH += ../lib

#HEADERS = \

# Input
SOURCES += \
    main.cpp

