cmake_minimum_required(VERSION 3.21)
project(QtNodeEditorDemo)

set(CMAKE_CXX_STANDARD 20)

if(UNIX)
    message("current platform: Linux ")
    set(CMAKE_PREFIX_PATH ~/Qt/5.15.2/gcc_64)
elseif(WIN32)
    message("current platform: Windows ")
    set(CMAKE_PREFIX_PATH C:/Qt/6.6.0/msvc2019_64)
else()
    message("current platform: unkonw(unsupported) ")
endif()

add_subdirectory(QtNodeEditor)
add_subdirectory(Demo)

