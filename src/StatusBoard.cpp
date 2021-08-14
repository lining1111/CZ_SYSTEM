//
// Created by lining on 8/15/21.
//

#include "StatusBoard.h"
#include <iostream>

using namespace std;

void StatusBoard::ProcessRecv(ClientUdp::Msg msg, void *pUser) {
    if (pUser == nullptr) {
        return;
    }
    auto statusBoard = (StatusBoard *) pUser;
    cout << "msg:" << msg.buf << endl;
    statusBoard->cnt++;
}
