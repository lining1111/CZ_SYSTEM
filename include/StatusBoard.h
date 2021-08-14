//
// Created by lining on 8/15/21.
//

#ifndef _STATUSBOARD_H
#define _STATUSBOARD_H

#include "ClientUdp.h"

class StatusBoard {
public:
    int cnt = 0;
public:
    static void ProcessRecv(ClientUdp::Msg msg, void *pUser);
};


#endif //_STATUSBOARD_H
