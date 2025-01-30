cmake_minimum_required(VERSION 3.16)

project(MassaK)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)


find_package(QT NAMES Qt5 REQUIRED COMPONENTS  Widgets)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS  Widgets)

# Указываем исходные файлы
set(SOURCES
    main.cpp  
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)


#include_directories(./serial)

#target_include_directories(MassaK PRIVATE ${SERIAL_INCLUDE})

# Создаем app
add_executable(MassaK ${SOURCES})

target_link_libraries(MassaK PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(MassaK PRIVATE Threads::Threads)

target_include_directories(MassaK PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib)

target_link_libraries(MassaK PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/build/libMassaK.so)