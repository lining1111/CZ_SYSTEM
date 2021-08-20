cmake_minimum_required(VERSION 3.20)
project(CZ_SYSTEM)

set(CMAKE_CXX_STANDARD 11)

include_directories(base/include)

#Common
set(LibCommon_Files base/src/Common.cpp)
add_library(LibCommon STATIC ${LibCommon_Files})
#RingBuffer
set(LibRingBuffer_Files base/src/RingBuffer.cpp)
add_library(LibRingBuffer STATIC ${LibRingBuffer_Files})

#ClientUdp
set(LibClientUdp_Files base/src/ClientUdp.cpp)
add_library(LibClientUdp STATIC ${LibClientUdp_Files})

#ServerUdp
set(LibServerUdp_Files base/src/ServerUdp.cpp
                        base/src/ClientInfo.cpp)
add_library(LibServerUdp STATIC ${LibServerUdp_Files})

set(LINK_LIBS  ${LINK_LIBS} LibClientUdp LibServerUdp LibCommon LibRingBuffer m pthread)

#后台
add_subdirectory(background)
#qt ui
add_subdirectory(qtUi)