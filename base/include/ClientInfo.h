//
// Created by lining on 8/14/21.
//

#ifndef _CLIENTINFO_H
#define _CLIENTINFO_H


#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include "RingBuffer.h"
#include "Common.h"

using namespace std;
using namespace Common;

class ClientInfo {
public:
    struct sockaddr_in clientAddr;
    DevType devType = UnknownDev;
    timeval receive_time;
public:
    ClientInfo(struct sockaddr_in clientAddr, DevType devType);

    ~ClientInfo();
};


#endif //_CLIENTINFO_H
