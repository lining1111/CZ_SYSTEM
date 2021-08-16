//
// Created by lining on 8/14/21.
//

#include "MainControlBoard.h"
#include <iostream>
#include <cstring>
#include "Common.h"

using namespace std;
using namespace Common;

MainControlBoard::MainControlBoard() {
    //初始化测试板参数体内关键字和参数长度
    InitMapKey_Test(this->mapKey_Test);
}

MainControlBoard::~MainControlBoard() {
    mapKey_Test.clear();
}

void MainControlBoard::ProcessRecv(ClientUdp::Msg msg, void *pUser) {

    if (pUser == nullptr) {
        return;
    }

    auto mainControlBoard = (MainControlBoard *) pUser;
    cout << "msg recv" << endl;
    mainControlBoard->cnt++;

    //解析收到结果

}

int MainControlBoard::GetInfo_Base(ClientUdp::Msg &msg, DevType type, Cmd cmd, uint16_t paramKey, uint32_t value) {

    InfoBase infoBase;

    infoBase.dev = type;
    infoBase.cmd = cmd;

    uint16_t key = paramKey;//关键字
    uint8_t paramLen = 0x04;//默认参数长度为4
    ParamType paramType = Common::ParamType_Byte;
    uint8_t param[256] = {0};//参数值

    //获取参数长度
    auto iter = mapKey_Test.find(key);
    if (iter != mapKey_Test.end()) {
        paramLen = iter->second.len;
        paramType = iter->second.type;
    }
    //获取参数值数组
    bzero(param, sizeof(param));

    switch (paramType) {
        case Common::ParamType_Byte:
            for (int i = 0; i < paramLen; i++) {
                uint8_t postion = (8 * ((sizeof(value) - 1/*max*/) - (sizeof(value) - paramLen) - i));
                param[i] = 0xff & (value >> postion);
            }
            break;
        case Common::ParamType_ASCII:
            string format = "%0";
            format.append(to_string(paramLen));
            format.append("d");
            sprintf((char *) param, format.c_str(), value);
            break;
    }

    //组织参数体
    int index = 0;
    memcpy(&infoBase.data[index], &key, sizeof(key));
    index += sizeof(key);
    memcpy(&infoBase.data[index], &paramLen, sizeof(paramLen));
    index += sizeof(paramLen);
    memcpy(&infoBase.data[index], param, paramLen);
    index += paramLen;
    infoBase.len = index;

    bzero(msg.buf, sizeof(msg.buf));
    SetInfo(infoBase, msg.buf, &msg.len);

    return 0;
}

int MainControlBoard::GetInfo_SetTestBoardFreq(ClientUdp::Msg &msg, uint32_t freq) {
    return GetInfo_Base(msg, Common::DT_Simple_Process, Common::Cmd_Set, ParamKey_Test_Freq, freq);
}

int MainControlBoard::GetInfo_SetTestBoardAtten(ClientUdp::Msg &msg, uint32_t atten) {
    return GetInfo_Base(msg, Common::DT_Simple_Process, Common::Cmd_Set, ParamKey_Test_Atten, atten);
}

int MainControlBoard::GetInfo_SetTestBoardRef(ClientUdp::Msg &msg, uint8_t ref) {
    return GetInfo_Base(msg, Common::DT_Simple_Process, Common::Cmd_Set, ParamKey_Test_Ref, ref);
}

int MainControlBoard::GetInfo_SetTestBoardVcc(ClientUdp::Msg &msg, uint16_t vcc) {
    return GetInfo_Base(msg, Common::DT_Simple_Process, Common::Cmd_Set, ParamKey_Test_Vcc, vcc);
}

int MainControlBoard::GetInfo_SetTestBoardStartFreq(ClientUdp::Msg &msg, uint32_t startFreq) {
    return GetInfo_Base(msg, Common::DT_Simple_Process, Common::Cmd_Set, ParamKey_Test_StartFreq, startFreq);
}

int MainControlBoard::GetInfo_SetTestBoardEndFreq(ClientUdp::Msg &msg, uint32_t endFreq) {
    return GetInfo_Base(msg, Common::DT_Simple_Process, Common::Cmd_Set, ParamKey_Test_EndFreq, endFreq);
}

int MainControlBoard::GetInfo_SetTestBoardLevel(ClientUdp::Msg &msg, uint8_t level) {
    return GetInfo_Base(msg, Common::DT_Simple_Process, Common::Cmd_Set, ParamKey_Test_Level, level);
}

int MainControlBoard::GetInfo_SetTestBoardMode(ClientUdp::Msg &msg, uint8_t mode) {
    return GetInfo_Base(msg, Common::DT_Simple_Process, Common::Cmd_Set, ParamKey_Test_Mode, mode);
}

int MainControlBoard::GetInfo_SetTestBoardWorkMode(ClientUdp::Msg &msg, uint8_t workMode) {
    return GetInfo_Base(msg, Common::DT_Simple_Process, Common::Cmd_Set, ParamKey_Test_WorkMode, workMode);
}

int MainControlBoard::GetInfo_SetTestBoardAntMode(ClientUdp::Msg &msg, uint8_t antMode) {
    return GetInfo_Base(msg, Common::DT_Simple_Process, Common::Cmd_Set, ParamKey_Test_AntMode, antMode);
}
