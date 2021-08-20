//
// Created by lining on 8/14/21.
//

#ifndef _MAINCONTROLBOARD_H
#define _MAINCONTROLBOARD_H

#include "ClientUdp.h"
#include <map>
#include "Common.h"

using namespace std;
using namespace Common;

//zhu kong ban

class MainControlBoard {
public:
    int cnt = 0;

    map<uint16_t, struct ParamLen> mapKey_Test;

public:
    MainControlBoard();

    ~MainControlBoard();

public:
    static void ProcessRecv(ClientUdp::Msg msg, void *pUser);

public:
    /**
     * 组包基本函数
     * @param msg out
     * @param type in 设备类型
     * @param cmd in 命令码
     * @param paramKey in 参数体内关键字
     * @param value in 参数体内参数值
     * @return
     */
    int GetInfo_Base(ClientUdp::Msg &msg,DevType type, Cmd cmd, uint16_t paramKey, uint32_t value);

    /**
     * 组织信息，发送测试板设置频率指令
     * @param msg out
     * @param freq in
     * @return
     */
    int GetInfo_SetTestBoardFreq(ClientUdp::Msg &msg, uint32_t freq);

    /**
     * 组织信息，发送测试板设置衰减指令
     * @param msg out
     * @param atten
     * @return
     */
    int GetInfo_SetTestBoardAtten(ClientUdp::Msg &msg, uint32_t atten);

    /**
     * 组织信息，发送测试板设置参考切换指令
     * @param msg out
     * @param ref
     * @return
     */
    int GetInfo_SetTestBoardRef(ClientUdp::Msg &msg, uint8_t ref);

    /**
     * 组织信息，发送测试板设置偏置电压指令
     * @param msg
     * @param vcc
     * @return
     */
    int GetInfo_SetTestBoardVcc(ClientUdp::Msg &msg, uint16_t vcc);

    /**
     * 组织信息，发送测试板设置开始频率指令
     * @param msg out
     * @param startFreq
     * @return
     */
    int GetInfo_SetTestBoardStartFreq(ClientUdp::Msg &msg, uint32_t startFreq);

    /**
     * 组织信息，发送测试板设置结束频率指令
     * @param msg out
     * @param endFreq
     * @return
     */
    int GetInfo_SetTestBoardEndFreq(ClientUdp::Msg &msg, uint32_t endFreq);

    /**
     * 组织信息，发送测试板设置等级指令
     * @param msg out
     * @param level
     * @return
     */
    int GetInfo_SetTestBoardLevel(ClientUdp::Msg &msg, uint8_t level);

    /**
     * 组织信息，发送测试板设置模式指令
     * @param msg out
     * @param mode
     * @return
     */
    int GetInfo_SetTestBoardMode(ClientUdp::Msg &msg, uint8_t mode);

    /**
     * 组织信息，发送测试板设置工作模式指令
     * @param msg out
     * @param workMode
     * @return
     */
    int GetInfo_SetTestBoardWorkMode(ClientUdp::Msg &msg, uint8_t workMode);

    /**
     * 组织信息，发送测试板设置天线模式指令
     * @param msg
     * @param antMode
     * @return
     */
    int GetInfo_SetTestBoardAntMode(ClientUdp::Msg &msg, uint8_t antMode);

};


#endif //_MAINCONTROLBOARD_H
