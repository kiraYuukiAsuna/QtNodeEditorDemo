cmake_minimum_required(VERSION 3.21)

project(Demo)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_AUTOMOC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Gui OpenGL)


include_directories(../QtNodeEditor/Include)

link_directories(${CMAKE_BINARY_DIR}/lib)

set(PROJECTS_HEADERS
        FunctionDelegateModel.h
        PerformanceDelegateModel.h
        ConditionDelegateModel.h
        DefaultNodeData.h
        )

set(PROJECT_SOURCES
        main.cpp
        FunctionDelegateModel.cpp
        PerformanceDelegateModel.cpp
        ConditionDelegateModel.cpp
        DefaultNodeData.cpp
        )

add_executable(Demo ${PROJECTS_HEADERS} ${PROJECT_SOURCES})

target_link_libraries(Demo
        PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::OpenGL

        QtNodes
        )

