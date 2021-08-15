#test_client_mcb
add_executable(test_client_mcb test_client_mcb.cpp
        ../src/ClientUdp.cpp
        ../src/MainControlBoard.cpp
        ../src/Common.cpp)
target_link_libraries(test_client_mcb ${LINK_LIBS})

#test_server_info
add_executable(test_server_info test_server_info.cpp
        ../src/ClientInfo.cpp
        ../src/ServerUdp.cpp
        ../src/StatusBoard.cpp
        ../src/Common.cpp)
target_link_libraries(test_server_info ${LINK_LIBS})