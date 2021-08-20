//
// Created by lining on 8/14/21.
//
#include "ServerUdp.h"
#include "StatusBoard.h"
#include <iostream>
#include <unistd.h>
#include <cstring>

#include "Common.h"

using namespace std;

using namespace Common;

int main() {
    StatusBoard *statusBoard = new StatusBoard();

    ServerUdp *serverUdp = new ServerUdp(StatusBoard::ProcessRecv,statusBoard);

    serverUdp->Open();
    serverUdp->Run();

    string msg_send;
    bool isExit = false;
    while (!isExit) {
        //task

        //test

        sleep(1);
    }

}