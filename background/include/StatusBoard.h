//
// Created by lining on 8/15/21.
//

#ifndef _STATUSBOARD_H
#define _STATUSBOARD_H

#include "ServerUdp.h"
#include "Common.h"

using namespace Common;

class StatusBoard {
public:
    DevType devType = Common::DT_Simple_Process;//本机设备类型

    int cnt = 0;
public:
    static void ProcessRecv(ServerUdp &server, ServerUdp::Msg msg, void *pUser);
};


#endif //_STATUSBOARD_H
