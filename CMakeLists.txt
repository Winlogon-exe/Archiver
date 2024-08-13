cmake_minimum_required(VERSION 3.5)

project(Detroit VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "D:\\QT\\6.7.2\\mingw_64")
set(CMAKE_PREFIX_PATH "D:/vcpkg/installed/x64-windows")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

find_package(ZLIB REQUIRED)
find_package(libzip CONFIG REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)

qt_add_executable(Detroit
        main.cpp
        inc/ui/MainForm.h
        src/ui/MainForm.cpp

        inc/logic/MainFormLogic.h
        src/logic/MainFormLogic.cpp
)

target_link_libraries(Detroit PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ZLIB::ZLIB libzip::zip)
