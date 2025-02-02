cmake_minimum_required(VERSION 3.28)

project(MassaK)

set(CMAKE_CXX_STANDARD 17)

# Указываем исходные файлы
set(SOURCES  
    serial/paramsrs.cpp   
    serial/serial_factory.cpp  
    checkcomports.cpp
    controller.cpp
    driver.cpp
    driver_plain.cpp
    protocol.cpp
)

# Указываем заголовочные файлы
set(HEADERS
    serial/iserial.h        
    serial/paramsrs.h          
    serial/serial_factory.h  
    messages.h
    protocol.h
    aixlog.hpp
    checkcomports.h
    controller.h
    driver.h
    driver_plain.h
    scales_parameters.h
)

#include_directories(./serial)

#target_include_directories(MassaK PRIVATE ${SERIAL_INCLUDE})

# Создаем динамическую библиотеку
add_library(MassaK SHARED ${SOURCES} ${HEADERS})

if(UNIX)
    target_sources(MassaK PRIVATE serial/linuxserial.h)
    target_sources(MassaK PRIVATE serial/linuxserial.cpp)
Endif()

if(WIN32)
    add_definitions(-DMASSAK_WINDOWS)
    target_sources(MassaK PRIVATE serial/winserial.cpp)
    target_sources(MassaK PRIVATE serial/winserial.h)
    target_sources(MassaK PRIVATE serial/win_serial.h)
endif()


target_include_directories(MassaK PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/serial)
target_include_directories(MassaK PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Указываем имя выходной библиотеки
set_target_properties(MassaK PROPERTIES OUTPUT_NAME "MassaK")

# Указываем, что библиотека должна быть установлена в /usr/local/lib
#install(TARGETS MassaK LIBRARY DESTINATION /usr/local/lib)