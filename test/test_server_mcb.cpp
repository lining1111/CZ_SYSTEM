//
// Created by lining on 8/14/21.
//

#include "ServerUdp.h"
#include "MainControlBoard.h"
#include <iostream>
#include <unistd.h>

using namespace std;

int main(){
    MainControlBoard *mainControlBoard = new MainControlBoard();

    ServerUdp *serverUdp = new ServerUdp(MainControlBoard::ProcessRecv,
                                         mainControlBoard);

    serverUdp->Open();
    serverUdp->Run();

    while (true){
        sleep(1);
    }

}