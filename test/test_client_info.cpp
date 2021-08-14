//
// Created by lining on 8/14/21.
//
#include "ClientUdp.h"
#include "StatusBoard.h"
#include <iostream>
#include <unistd.h>
#include <cstring>

using namespace std;

int main(){
    StatusBoard *statusBoard = new StatusBoard();

    ClientUdp *clientUdp = new ClientUdp("127.0.0.1",8313,StatusBoard::ProcessRecv,statusBoard);

    clientUdp->Open();
    clientUdp->Run();

    string msg_send;
    bool isExit = false;
    while (!isExit) {
        cout << "please enter(q:quit):" << endl;
        cin >> msg_send;

        if (msg_send == "q") {
            isExit = true;
            continue;
        }

        //task
        ClientUdp::Msg msg;
        bzero(msg.buf, sizeof(msg.buf));
        memcpy(msg.buf, msg_send.c_str(), strlen(msg_send.c_str()) + 1);
        msg.len = strlen(msg_send.c_str()) + 1;
        clientUdp->Send(msg);

        sleep(1);
    }

}