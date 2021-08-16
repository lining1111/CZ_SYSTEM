//
// Created by lining on 8/14/21.
//

#include <cstring>
#include <sys/time.h>
#include "ClientInfo.h"

ClientInfo::ClientInfo(struct sockaddr_in clientAddr, DevType devType) {
    this->clientAddr = clientAddr;
    this->devType = devType;
    gettimeofday(&this->receive_time, nullptr);
}

ClientInfo::~ClientInfo() {
}
