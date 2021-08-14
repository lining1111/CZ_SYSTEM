#test_server_mcb
add_executable(test_server_mcb test_server_mcb.cpp
        ../src/ClientInfo.cpp
        ../src/ServerUdp.cpp
        ../src/MainControlBoard.cpp)
target_link_libraries(test_server_mcb ${LINK_LIBS})

#test_client_info
add_executable(test_client_info test_client_info.cpp
        ../src/ClientUdp.cpp
        ../src/StatusBoard.cpp)
target_link_libraries(test_client_info ${LINK_LIBS})