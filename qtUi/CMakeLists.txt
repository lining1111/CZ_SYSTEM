cmake_minimum_required(VERSION 3.20)

set(TargetName qtUi)

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

add_executable(${TargetName} qtUi.cpp MainWindow.cpp
        ../background/src/MainControlBoard.cpp dialog1.cpp resource_dialog1.h)

set(LINK_LIBS ${LINK_LIBS}
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets)


target_link_libraries(${TargetName}
        ${LINK_LIBS})

