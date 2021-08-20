cmake_minimum_required(VERSION 3.20)
project(qtUi)

set(CMAKE_CXX_STANDARD 11)

##qt 相关设置
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#设置qt路径
set(CMAKE_PREFIX_PATH "/home/lining/Qt5.10.0/5.10.0/gcc_64")

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)
include_directories(..)
include_directories(../background/include)

add_executable(${CMAKE_PROJECT_NAME} main.cpp MainWindow.cpp MainWindow.h MainWindow.ui
        ../background/src/MainControlBoard.cpp
        ../background/src/StatusBoard.cpp ../base/src/Common.cpp ../base/include/Common.h)

set(LINK_LIBS ${LINK_LIBS}
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets)


target_link_libraries(${CMAKE_PROJECT_NAME}
        ${LINK_LIBS})

