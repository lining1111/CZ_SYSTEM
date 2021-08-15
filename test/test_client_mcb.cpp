//
// Created by lining on 8/14/21.
//

#include "ClientUdp.h"
#include "MainControlBoard.h"
#include "Common.h"
#include <iostream>
#include <unistd.h>
#include <cstring>

using namespace std;

using namespace Common;

int main(){
    MainControlBoard *mainControlBoard = new MainControlBoard();

    ClientUdp *clientUdp = new ClientUdp("127.0.0.1",8313,MainControlBoard::ProcessRecv,
                                         mainControlBoard);

    clientUdp->Open();
    clientUdp->Run();

    bool isExit = false;
    while (!isExit){
        sleep(1);

        string msg_send;
        cout << "please enter(q:quit):" << endl;
        cin >> msg_send;

        if (msg_send == "q") {
            isExit = true;
            continue;
        }



        //test send

        InfoBase infoBase;

        infoBase.dev = DT_Simple_Process;
        infoBase.cmd = Cmd_Set;

        uint16_t key_test = 0xffff;//关键字
        uint8_t param_len = 0x04;//参数长度
        uint32_t param = 0x12345678;//参数值

        int index = 0;
        memcpy(&infoBase.data[index],&key_test, sizeof(key_test));
        index += sizeof(key_test);
        memcpy(&infoBase.data[index],&param_len, sizeof(param_len));
        index += sizeof(param_len);
        memcpy(&infoBase.data[index],&param, sizeof(param));
        index += sizeof(param);
        infoBase.len = index;

        ClientUdp::Msg msg;
        bzero(msg.buf, sizeof(msg.buf));
        SetInfo(infoBase,msg.buf,&msg.len);

        clientUdp->Send(msg);


    }

}