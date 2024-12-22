cmake_minimum_required(VERSION 3.5)

project(Zipster VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "D:/Qt/6.7.2/mingw_64")
set(CMAKE_PREFIX_PATH "D:\\vcpkg\\installed\\x64-windows")

if(WIN32)
    set(APP_ICON_RESOURCE "${CMAKE_CURRENT_SOURCE_DIR}/app_icon.rc")
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

find_package(ZLIB REQUIRED)
find_package(libzip CONFIG REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)

qt_add_executable(Zipster
        main.cpp
        inc/ui/MainForm.h
        src/UI/MainForm.cpp

        inc/logic/MainForm.h
        src/logic/MainForm.cpp
        inc/ui/ArchiveExplorer.h
        src/UI/ArchiveExplorer.cpp
        inc/logic/Register.h
        src/logic/Register.cpp
)

if(WIN32)
    target_sources(Zipster PRIVATE ${APP_ICON_RESOURCE})
endif()

target_link_libraries(Zipster PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ZLIB::ZLIB libzip::zip Shell32)
