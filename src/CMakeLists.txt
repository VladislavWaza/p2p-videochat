cmake_minimum_required(VERSION 3.5)

set(TARGET_NAME videochat)
set(LIB_NAME videochat-lib)

set(CMAKE_CXX_STANDARD 17)
if (MSVC)
    add_compile_options(/Zc:__cplusplus)
    add_compile_options(/permissive-)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_PREFIX_PATH "C:/Qt/6.2.8/MSVS")

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Widgets Network MultimediaWidgets Multimedia REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets Network MultimediaWidgets Multimedia REQUIRED)

set(LIBRARY_SOURCES
    MainWindow.h
    MainWindow.cpp
    ConnectionDialog.h
    ConnectionDialog.cpp
    UdpSocketHandler.h
    UdpSocketHandler.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_library(${LIB_NAME} STATIC ${LIBRARY_SOURCES})
else()
    add_library(${LIB_NAME} STATIC ${LIBRARY_SOURCES})
endif()

target_link_libraries(${LIB_NAME} PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::MultimediaWidgets
    Qt${QT_VERSION_MAJOR}::Multimedia
)

set(EXECUTABLE_SOURCES
    main.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${TARGET_NAME} MANUAL_FINALIZATION ${EXECUTABLE_SOURCES})
else()
    add_executable(${TARGET_NAME} ${EXECUTABLE_SOURCES})
endif()

target_link_libraries(${TARGET_NAME} PRIVATE
    ${LIB_NAME}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${TARGET_NAME})
endif()
