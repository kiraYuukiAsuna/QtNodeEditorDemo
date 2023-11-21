cmake_minimum_required(VERSION 3.21)

project(Demo)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Gui OpenGL Svg)


include_directories(
        ../QtNodeEditor/Include
        ThirdParty/QtAdvancedStylesheet
)

link_directories(${CMAKE_BINARY_DIR}/lib)

set(PROJECTS_HEADERS
        ThirdParty/QtAdvancedStylesheet/acss_globals.h
        ThirdParty/QtAdvancedStylesheet/QtAdvancedStylesheet.h

        FunctionDelegateModel.h
        PerformanceDelegateModel.h
        ConditionDelegateModel.h
        DefaultNodeData.h
        CustomSelectWindow.h
        CustomSelectWidget.h
)

set(PROJECT_SOURCES
        ThirdParty/QtAdvancedStylesheet/QtAdvancedStylesheet.cpp

        main.cpp
        FunctionDelegateModel.cpp
        PerformanceDelegateModel.cpp
        ConditionDelegateModel.cpp
        DefaultNodeData.cpp
        PerformanceJobInternalWidget.h
        PerformanceJobInternalWidget.cpp
        FunctionJobInternalWidget.h
        FunctionJobInternalWidget.cpp
        CustomSelectWindow.cpp
        CustomSelectWidget.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ConnectionPopupWindow.h
        ConnectionPopupWindow.cpp
        ConnectionPopupWindow.ui
        PerformanceJobInternalWidgetV1.cpp
        PerformanceJobInternalWidgetV1.h
        FunctionJobInternalWidgetV1.cpp
        FunctionJobInternalWidgetV1.h
)

add_executable(Demo ${PROJECTS_HEADERS} ${PROJECT_SOURCES})

target_link_libraries(Demo
        PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::OpenGL
        Qt${QT_VERSION_MAJOR}::Svg
        QtNodes
)

