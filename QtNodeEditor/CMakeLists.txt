cmake_minimum_required(VERSION 3.21)

cmake_policy(SET CMP0072 NEW) # new in 3.11. The NEW behavior for this policy is to set OpenGL_GL_PREFERENCE to GLVND.
cmake_policy(SET CMP0068 NEW) # new in 3.9. The NEW behavior of this policy is to ignore the RPATH settings for install_name on macOS.

project(QtNodesLibrary CXX)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(OpenGL_GL_PREFERENCE LEGACY)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_DEBUG_POSTFIX_D "Append d suffix to debug libraries" OFF)

if (BUILD_DEBUG_POSTFIX_D)
    set(CMAKE_DEBUG_POSTFIX "d")
    set(CMAKE_RELEASE_POSTFIX "")
    set(CMAKE_RELWITHDEBINFO_POSTFIX "rd")
    set(CMAKE_MINSIZEREL_POSTFIX "s")
endif ()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Gui OpenGL)
message(STATUS "QT_VERSION: ${QT_VERSION}, QT_DIR: ${QT_DIR}")

if (${QT_VERSION} VERSION_LESS 5.11.0)
    message(FATAL_ERROR "Requires qt version >= 5.11.0, Your current version is ${QT_VERSION}")
endif ()

if (${QT_VERSION_MAJOR} EQUAL 6)
    qt_add_resources(RESOURCES ./Resources/resources.qrc)
else ()
    qt5_add_resources(RESOURCES ./Resources/resources.qrc)
endif ()

# Unfortunately, as we have a split Include/QtNodes/Source, AUTOMOC doesn't work.
# We'll have to manually specify some files
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include_directories(Include)

set(CPP_SOURCE_FILES
        Source/AbstractGraphModel.cpp
        Source/AbstractNodeGeometry.cpp
        Source/BasicGraphicsScene.cpp
        Source/ConnectionGraphicsObject.cpp
        Source/ConnectionPainter.cpp
        Source/ConnectionState.cpp
        Source/ConnectionStyle.cpp
        Source/DataFlowGraphModel.cpp
        Source/DataFlowGraphicsScene.cpp
        Source/DefaultHorizontalNodeGeometry.cpp
        Source/DefaultVerticalNodeGeometry.cpp
        Source/Definitions.cpp
        Source/GraphicsView.cpp
        Source/GraphicsViewStyle.cpp
        Source/NodeDelegateModelRegistry.cpp
        Source/NodeConnectionInteraction.cpp
        Source/NodeDelegateModel.cpp
        Source/NodeGraphicsObject.cpp
        Source/DefaultNodePainter.cpp
        Source/NodeState.cpp
        Source/NodeStyle.cpp
        Source/StyleCollection.cpp
        Source/UndoCommands.cpp
        Source/locateNode.cpp
        Source/ConnectionGeometry.cpp
        Source/CustomNodePainter.cpp
        Source/CustomNodeGeometry.cpp
        Source/ConnectionPopupWindowBase.cpp
        Include/QtNodes/ConnectionPopupWindowBase.h


)

set(HPP_HEADER_FILES
        Include/QtNodes/internal/AbstractGraphModel.hpp
        Include/QtNodes/internal/AbstractNodeGeometry.hpp
        Include/QtNodes/internal/AbstractNodePainter.hpp
        Include/QtNodes/internal/BasicGraphicsScene.hpp
        Include/QtNodes/internal/Compiler.hpp
        Include/QtNodes/internal/ConnectionGraphicsObject.hpp
        Include/QtNodes/internal/ConnectionIdHash.hpp
        Include/QtNodes/internal/ConnectionIdUtils.hpp
        Include/QtNodes/internal/ConnectionState.hpp
        Include/QtNodes/internal/ConnectionStyle.hpp
        Include/QtNodes/internal/DataFlowGraphicsScene.hpp
        Include/QtNodes/internal/DataFlowGraphModel.hpp
        Include/QtNodes/internal/DefaultNodePainter.hpp
        Include/QtNodes/internal/Definitions.hpp
        Include/QtNodes/internal/Export.hpp
        Include/QtNodes/internal/GraphicsView.hpp
        Include/QtNodes/internal/GraphicsViewStyle.hpp
        Include/QtNodes/internal/locateNode.hpp
        Include/QtNodes/internal/NodeData.hpp
        Include/QtNodes/internal/NodeDelegateModel.hpp
        Include/QtNodes/internal/NodeDelegateModelRegistry.hpp
        Include/QtNodes/internal/NodeGraphicsObject.hpp
        Include/QtNodes/internal/NodeState.hpp
        Include/QtNodes/internal/NodeStyle.hpp
        Include/QtNodes/internal/OperatingSystem.hpp
        Include/QtNodes/internal/QUuidStdHash.hpp
        Include/QtNodes/internal/Serializable.hpp
        Include/QtNodes/internal/Style.hpp
        Include/QtNodes/internal/StyleCollection.hpp
        Source/ConnectionPainter.hpp
        Source/DefaultHorizontalNodeGeometry.hpp
        Source/DefaultVerticalNodeGeometry.hpp
        Source/NodeConnectionInteraction.hpp
        Source/UndoCommands.hpp
        Source/ConnectionGeometry.hpp
        Include/QtNodes/internal/CustomNodePainter.h
        Source/CustomNodeGeometry.hpp

)

add_library(QtNodes
        ${CPP_SOURCE_FILES}
        ${HPP_HEADER_FILES}
        ${RESOURCES}
)

target_include_directories(QtNodes
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Source>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include/QtNodes/internal>
)

target_link_libraries(QtNodes
        PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::OpenGL
)

target_compile_definitions(QtNodes
        PUBLIC
        NODE_EDITOR_STATIC
        PRIVATE
        NODE_EDITOR_EXPORTS
        #NODE_DEBUG_DRAWING
        QT_NO_KEYWORDS
)

target_compile_options(QtNodes
        PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /wd4127 /EHsc /utf-8>
        $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra>
        $<$<CXX_COMPILER_ID:AppleClang>:-Wall -Wextra -Werror>
)
if (NOT "${CMAKE_CXX_SIMULATE_ID}" STREQUAL "MSVC")
    # Clang-Cl on MSVC identifies as "Clang" but behaves more like MSVC:
    target_compile_options(QtNodes
            PRIVATE
            $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra>
    )
endif ()

set_target_properties(QtNodes
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

if (QT_NODES_DEVELOPER_DEFAULTS)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
endif ()

######
# Moc
##

file(GLOB_RECURSE HEADERS_TO_MOC Include/QtNodes/internal/*.hpp)

if (${QT_VERSION_MAJOR} EQUAL 6)
    qt_wrap_cpp(nodes_moc
            ${HEADERS_TO_MOC}
            TARGET QtNodes
            OPTIONS --no-notes # Don't display a note for the headers which don't produce a moc_*.cpp
    )
else ()
    qt5_wrap_cpp(nodes_moc
            ${HEADERS_TO_MOC}
            TARGET QtNodes
            OPTIONS --no-notes # Don't display a note for the headers which don't produce a moc_*.cpp
    )
endif ()

target_sources(QtNodes PRIVATE ${nodes_moc})
