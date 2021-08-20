//
// Created by lining on 8/15/21.
//

#include <cstring>
#include <netinet/in.h>
#include "Common.h"

namespace Common {


    int SetInfo(InfoBase infoBase, uint8_t *out, uint32_t *lenOfOut) {
        if (out == nullptr || lenOfOut == nullptr) {
            return -1;
        }
        int index = 0;

        CommonHead head;//网络字节
        CommonTail tail;//网络字节

        //head
        head.dev = htons(infoBase.dev);
        head.cmd = infoBase.cmd;
        head.frameLen = htons(infoBase.len);
        memcpy(&out[index], &head, sizeof(head));
        index += sizeof(head);
        //frame
        memcpy(&out[index], infoBase.data, infoBase.len);
        index += infoBase.len;
        //tail
        tail.checksum = 0;
        for (int i = 0; i < index; i++) {
            tail.checksum += out[i];
        }
        memcpy(&out[index], &tail, sizeof(tail));
        index += sizeof(tail);

        *lenOfOut = index;

        return 0;
    }

    int GetInfo(uint8_t *in, uint32_t lenOfIn, InfoBase &infoBase) {
        if (in == nullptr) {
            return -1;
        }
        if (lenOfIn == 0) {
            return 0;
        }
        int index = 0;
        CommonHead head;//网络字节
        CommonTail tail;//网络字节

        //head
        memcpy(&head, &in[index], sizeof(head));
        index += sizeof(head);
        infoBase.dev = ntohs(head.dev);
        infoBase.cmd = head.cmd;
        infoBase.len = ntohs(head.frameLen);
        //frame
        bzero(infoBase.data, sizeof(infoBase.data));
        memcpy(infoBase.data, &in[index], infoBase.len);
        index += infoBase.len;

        //tail
        memcpy(&tail, &in[index], sizeof(tail));
        index += sizeof(tail);


        return 0;
    }

    int InitMapKey_Test(map<uint16_t, struct ParamLen> &mapKey_Test) {
        int count = 0;

        struct ParamLen paramLen;

        paramLen.len = 9;
        paramLen.type = ParamType_ASCII;
        mapKey_Test[ParamKey_Test_Freq] = paramLen;
        count++;

        paramLen.len = 5;
        paramLen.type = ParamType_ASCII;
        mapKey_Test[ParamKey_Test_Atten] = paramLen;
        count++;

        paramLen.len = 1;
        paramLen.type = ParamType_Byte;
        mapKey_Test[ParamKey_Test_Ref] = paramLen;
        count++;

        paramLen.len = 2;
        paramLen.type = ParamType_Byte;
        mapKey_Test[ParamKey_Test_Vcc] = paramLen;
        count++;

        paramLen.len = 9;
        paramLen.type = ParamType_ASCII;
        mapKey_Test[ParamKey_Test_StartFreq] = paramLen;
        count++;

        paramLen.len = 9;
        paramLen.type = ParamType_ASCII;
        mapKey_Test[ParamKey_Test_EndFreq] = paramLen;
        count++;

        paramLen.len = 1;
        paramLen.type = ParamType_Byte;
        mapKey_Test[ParamKey_Test_Level] = paramLen;
        count++;

        paramLen.len = 1;
        paramLen.type = ParamType_Byte;
        mapKey_Test[ParamKey_Test_Mode] = paramLen;
        count++;

        paramLen.len = 1;
        paramLen.type = ParamType_Byte;
        mapKey_Test[ParamKey_Test_WorkMode] = paramLen;
        count++;

        paramLen.len = 1;
        paramLen.type = ParamType_Byte;
        mapKey_Test[ParamKey_Test_AntMode] = paramLen;
        count++;

        return count;
    }
}