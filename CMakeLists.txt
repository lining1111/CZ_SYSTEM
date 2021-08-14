cmake_minimum_required(VERSION 3.20)
project(CZ_SYSTEM)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "/home/lining/Qt5.10.0/5.10.0/gcc_64")

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)
include_directories(.)
include_directories(include)

add_executable(CZ_SYSTEM main.cpp MainWindow.cpp MainWindow.h MainWindow.ui
        src/RingBuffer.cpp
        src/ClientInfo.cpp
        src/ServerUdp.cpp
        src/ClientUdp.cpp

        src/MainControlBoard.cpp
        src/StatusBoard.cpp src/Common.cpp include/Common.h)

set(LINK_LIBS Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        m pthread)


target_link_libraries(CZ_SYSTEM
        ${LINK_LIBS})

add_subdirectory(test)

