//
// Created by lining on 8/14/21.
//

#ifndef _MAINCONTROLBOARD_H
#define _MAINCONTROLBOARD_H

#include "ClientUdp.h"

//zhu kong ban

class MainControlBoard {
public:
    int cnt = 0;

public:
    static void ProcessRecv(ClientUdp::Msg msg, void *pUser);
};


#endif //_MAINCONTROLBOARD_H
