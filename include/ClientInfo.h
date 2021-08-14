//
// Created by lining on 8/14/21.
//

#ifndef _CLIENTINFO_H
#define _CLIENTINFO_H


#include <sys/socket.h>
#include <arpa/inet.h>
#include "RingBuffer.h"
#include <string>

using namespace std;

class ClientInfo {
public:
    struct sockaddr_in clientAddr;
    int fd;
    timeval receive_time;
public:
    ClientInfo(struct sockaddr_in clientAddr, int client_fd);

    ~ClientInfo();
};


#endif //_CLIENTINFO_H
