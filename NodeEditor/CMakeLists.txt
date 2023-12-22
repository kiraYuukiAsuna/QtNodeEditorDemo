cmake_minimum_required(VERSION 3.21)

project(EditorBase)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Gui OpenGL Svg)


include_directories(

)

link_directories(
        ${CMAKE_BINARY_DIR}/lib
)

set(PROJECTS_HEADERS
        EditorBase.hpp
        NodeModel.h
)

set(PROJECT_SOURCES
        NodeModel.cpp

        main.cpp
)

add_executable(${PROJECT_NAME} ${PROJECTS_HEADERS} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME}
        PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::OpenGL
        Qt${QT_VERSION_MAJOR}::Svg
        QtNodes
)

