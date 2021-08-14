//
// Created by lining on 8/15/21.
//

#ifndef _COMMON_H
#define _COMMON_H


#include <cstdint>

namespace Common {
    /**
     * 协议基本格式
     * 设备类型     命令字     帧长      参数体     校验字节
     * 2bytes     1byte    2bytes    nbytes     1byte
     * 高字节在前，低字节在后
     * 帧长 表示参数体的字节总数
     */
#pragma pack(1)
    struct CommonHead {
        uint16_t dev;//设备类型
        uint8_t cmd;//命令字
        uint16_t frameLen;//帧长
    };

    struct CommonTail {
        uint8_t checksum;//所有内容字节累加和，最高位不进位（即仅保留最低字节）
    };

#pragma pack()

    enum DevType {
        CZ_Monitor = 0x0101,//CZ检测设备 主控板卡
        DT_Simple_Process = 0x0201,//DT采集与处理设备 功能板卡
        JQ_Simple_Process = 0x0202,//JQ信号采集与处理设备 功能板卡
        WXCS_Simple_Process = 0x0203,//WXCS信号采集与处理设备 功能板卡
        BD_Simple_Process = 0x0204,//BD信号采集与处理设备 功能板卡
        YD_Simple_Process = 0x0205,//YD信号采集与处理设备 功能板卡
        ZH_Simple_Process = 0x0206,//ZH信号采集与处理设备 功能板卡
        JIDS_Simple_Process = 0x0207,//JIDS信号采集与处理设备 功能板卡
        SDH_Encrypt = 0x0301,//SDH加密采集设备 LC检测设备
        YTM_Simple = 0x0302,//YTW采集设备 LC检测设备
        DC_Simple = 0x0303,//DC信号采集设备 LC检测设备
        SZ_Communication_Monitor = 0x0304,//SZ通信监控设备 LC检测设备
    };

    enum Cmd {
        Cmd_Set = 0x80,//上级监控软件发送的设备参数设置命令
        Rsp_Set = 0x81,//被监控设备返回的设备参数设置响应
        Cmd_GetAll = 0x82,//上级监控软件发送的设备参数全查询命令
        Rsp_GetAll = 0x83,//被监控设备返回的设备参数全查询响应
        Cmd_Get = 0x84,//上级监控软件发送的单个参数查询命令
        Rsp_Get = 0x85,//被监控设备返回的单个参数查询响应
        Cmd_CheckControl = 0x87,//上级监控软件发送的设备检测控制命令
        Rsp_CheckControl = 0x88,//被监控设备返回的检测控制命令响应
        CheckResult_Report = 0x89,//信号检测设备返回的检测结果（采集数据）
        Cmd_GetHealth = 0xa1,//上级监控软件发送的健康参数查询命令
        Rsp_GetHealth = 0xa2,//被监控设备返回的健康参数查询响应
    };

    /**
     * 0x80 参数体格式：由若干 关键字(2bytes)+参数长度(1byte)+参数值(n bytes) 关键字由设备研制单位自定义
     * 0x81 参数体格式：由若干 关键字(2bytes)+设置结果(1byte 20H表示设置成功；21H表示设置失败)
     * 0x82 无参数体
     * 0x83 参数体格式：由若干 关键字(2bytes)+参数长度(1byte)+参数值(n bytes) 关键字由设备研制单位自定义
     * 0x84 参数体格式：关键字(2bytes)
     * 0x85 参数体格式：由1个 关键字(2bytes)+参数长度(1byte)+参数值(n bytes) 关键字由设备研制单位自定义
     *
     * 0x87 参数体格式：由1个 关键字(2bytes)+参数长度(1byte)+参数值(n bytes) 关键字由设备研制单位自定义
     * 0x88 参数体格式：由1个 关键字(2bytes)+参数长度(1byte)+参数值(n bytes) 关键字由设备研制单位自定义
     * 0x89 参数体格式：由1个 关键字(2bytes)+数据长度(2bytes)+上报数据(n bytes) 关键字由设备研制单位自定义
     *
     *
     */


    struct InfoBase {
        uint16_t dev;//设备类型
        uint8_t cmd;//命令字
        uint8_t data[1024 * 64];//参数体
        uint16_t len;//参数体长度
    };

    /**
     * 组包程序
     * @param infoBase 组包输入基本元素
     * @param out 得到的组包数据
     * @param lenOfOut 得到的组包数据长度
     * @return -1 fail 0 success
     */
    int SetInfo(InfoBase infoBase, uint8_t *out, uint32_t *lenOfOut);

    /**
     * 解包程序
     * @param in 原始帧
     * @param lenOfIn 原始帧长度
     * @param infoBase 解包输出
     * @return -1 fail 0 success
     */
    int GetInfo(uint8_t *in, uint32_t lenOfIn, InfoBase &infoBase);

};


#endif //_COMMON_H
