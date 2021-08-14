//
// Created by lining on 8/15/21.
//

#include <cstring>
#include "Common.h"

namespace Common {


    int SetInfo(InfoBase infoBase, uint8_t *out, uint32_t *lenOfOut) {
        if (out == nullptr || lenOfOut == nullptr) {
            return -1;
        }
        int index = 0;

        CommonHead head;
        CommonTail tail;

        //head
        head.dev = infoBase.dev;
        head.cmd = infoBase.cmd;
        head.frameLen = infoBase.len;
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
        CommonHead head;
        CommonTail tail;

        //head
        memcpy(&head, &in[index], sizeof(head));
        index += sizeof(head);
        infoBase.dev = head.dev;
        infoBase.cmd = head.cmd;

        //frame
        bzero(infoBase.data, sizeof(infoBase.data));
        memcpy(infoBase.data, &in[index], head.frameLen);
        index += head.frameLen;
        infoBase.len = head.frameLen;
        //tail
        memcpy(&tail, &in[index], sizeof(tail));
        index += sizeof(tail);


        return 0;
    }
}