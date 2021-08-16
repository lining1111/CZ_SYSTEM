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

        int select = atoi(msg_send.c_str());

        ClientUdp::Msg msg;

        bzero(msg.buf, sizeof(msg.buf));
        msg.len = 0;

        switch (select) {
            case 1:{
                //发送设置频率
                mainControlBoard->GetInfo_SetTestBoardFreq(msg,123456);
            }
                break;
            case 2:{
                //发送设置衰减
                mainControlBoard->GetInfo_SetTestBoardAtten(msg,123456);
            }
            break;
            case 3:{
                //发送设置参考
                mainControlBoard->GetInfo_SetTestBoardRef(msg,0x12);
            }
            break;
            case 4:{
                //发送设置偏置电压
                mainControlBoard->GetInfo_SetTestBoardVcc(msg,0x1234);
            }
            break;
            case 5:{
                //发送设置开始频率
                mainControlBoard->GetInfo_SetTestBoardStartFreq(msg,123456);
            }
            break;
            case 6:{
                //发送设置结束频率
                mainControlBoard->GetInfo_SetTestBoardEndFreq(msg,654321);
            }
            break;
            case 7:{
                //发送设置等级
                mainControlBoard->GetInfo_SetTestBoardLevel(msg,2);
            }
            break;
            case 8:{
                //发送设置模式
                mainControlBoard->GetInfo_SetTestBoardMode(msg,3);
            }
            break;
            case 9:{
                //发送设置工作模式
                mainControlBoard->GetInfo_SetTestBoardWorkMode(msg,4);
            }
            break;
            case 10:{
                //发送设置天线模式
                mainControlBoard->GetInfo_SetTestBoardAntMode(msg,5);
            }
            break;
            default:{
                cout<<"unknown select"<<endl;
                msg.len = 0;
            }
            break;
        }

        if (msg.len >0){
            clientUdp->Send(msg);
        }

    }

}