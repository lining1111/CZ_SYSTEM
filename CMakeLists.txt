cmake_minimum_required(VERSION 3.20)
project(CZ_SYSTEM)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#根据实际情况设置
#set(ARCH arm)
set(ARCH x86)
#set(ARCH arm64)

if (${ARCH} STREQUAL arm)
    set(CROSS_COMPILE /usr/local/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-)

    set(CMAKE_C_COMPILER ${CROSS_COMPILE}gcc)
    set(CMAKE_C_COMPILER_AR ${CROSS_COMPILE}ar)
    set(CMAKE_C_COMPILER_RANLIB ${CROSS_COMPILE}ranlib)
    set(CMAKE_CXX_COMPILER ${CROSS_COMPILE}g++)

    set(CMAKE_PREFIX_PATH "/home/lining/Qt5.10.0/5.10.0/rpi_32")

endif ()

if (${ARCH} STREQUAL arm64)
    set(CROSS_COMPILE /usr/local/arm64/gcc-linaro-4.9-2016.02-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-)

    set(CMAKE_C_COMPILER ${CROSS_COMPILE}gcc)
    set(CMAKE_C_COMPILER_AR ${CROSS_COMPILE}ar)
    set(CMAKE_C_COMPILER_RANLIB ${CROSS_COMPILE}ranlib)
    set(CMAKE_CXX_COMPILER ${CROSS_COMPILE}g++)

    set(CMAKE_PREFIX_PATH "/home/lining/Qt5.10.0/5.5.1/arm64")
endif ()

if (${ARCH} STREQUAL x86)
    set(CMAKE_PREFIX_PATH "/home/lining/Qt5.10.0/5.10.0/gcc_64")
endif ()

add_definitions(-D${ARCH})


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

