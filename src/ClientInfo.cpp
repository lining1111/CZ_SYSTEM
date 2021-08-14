//
// Created by lining on 8/14/21.
//

#include <cstring>
#include <sys/time.h>
#include "ClientInfo.h"

ClientInfo::ClientInfo(struct sockaddr_in clientAddr, int client_fd) {
    this->clientAddr = clientAddr;
    this->fd = client_fd;
    gettimeofday(&this->receive_time, nullptr);
}

ClientInfo::~ClientInfo() {
}
