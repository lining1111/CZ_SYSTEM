//
// Created by lining on 8/14/21.
//

#include "MainControlBoard.h"
#include <iostream>

using namespace std;


void MainControlBoard::ProcessRecv(ServerUdp::Msg msg, void *pUser) {

    if (pUser == nullptr) {
        return;
    }

    auto mainControlBoard = (MainControlBoard *) pUser;
    cout << "msg:" << msg.buf << endl;
    mainControlBoard->cnt++;

}
