//
// Created by lining on 8/15/21.
//

#include "StatusBoard.h"
#include "Common.h"
#include <iostream>
#include <cstring>

using namespace std;
using namespace Common;

void StatusBoard::ProcessRecv(ServerUdp &server, ServerUdp::Msg msg, void *pUser) {
    if (pUser == nullptr) {
        return;
    }
    auto statusBoard = (StatusBoard *) pUser;
    cout << "msg recv" << endl;
    statusBoard->cnt++;

    //task
    InfoBase infoBase;
    GetInfo(msg.buf, msg.len, infoBase);

    if (infoBase.dev != statusBoard->devType) {
        //not same to local dev type
        return;
    }

    switch (infoBase.cmd) {
        case Cmd_Set: {
            cout << "cmd set" << endl;

            //do something


            uint16_t key;
            uint8_t param_len;
            uint8_t param_data[64];
            uint8_t result = Result_Success;

            //get set
            int index_frame = 0;
            //get key
            uint16_t key_get;//关键字，网络字节
            memcpy(&key_get, &infoBase.data[index_frame], sizeof(key_get));
            index_frame += sizeof(key_get);
            key = ntohs(key_get);
            //get param_len
            memcpy(&param_len, &infoBase.data[index_frame], sizeof(param_len));
            index_frame += sizeof(param_len);
            //get param_data
            bzero(param_data, sizeof(param_data));
            memcpy(param_data, &infoBase.data[index_frame], param_len);
            index_frame += param_len;

            printf("key:%04x,param_len:%d \n", key, param_len);

            //set dev

            //response
            InfoBase infoBaseRsp;
            infoBaseRsp.dev = statusBoard->devType;
            infoBaseRsp.cmd = Rsp_Set;

            bzero(infoBaseRsp.data, sizeof(infoBaseRsp.data));
            int index = 0;
            memcpy(&infoBaseRsp.data[index], &key, sizeof(key));
            index += sizeof(key);
            memcpy(&infoBaseRsp.data[index], &result, sizeof(result));
            index += sizeof(result);
            infoBaseRsp.len = index;

            ServerUdp::Msg msgRsp;
            memcpy(&msgRsp.client_addr, &msg.client_addr, sizeof(msg.client_addr));
            bzero(msgRsp.buf, sizeof(msgRsp.buf));
            SetInfo(infoBaseRsp, msgRsp.buf, &msgRsp.len);
            server.Send(msgRsp);

        }
            break;
        case Cmd_GetAll: {

        }
            break;
        case Cmd_Get: {

        }
            break;
        case Cmd_CheckControl: {

        }
            break;
        case Cmd_GetHealth: {

        }
            break;
        default:
            break;
    }


}
