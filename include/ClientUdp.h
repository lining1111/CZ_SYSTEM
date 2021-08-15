//
// Created by lining on 8/14/21.
//

#ifndef _CLIENTUDP_H
#define _CLIENTUDP_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include "RingBuffer.h"
#include <string>
#include <thread>
#include <queue>

using namespace std;

/**
 * udp client 类
 */

class ClientUdp {
public:

    struct Msg {
        uint8_t buf[1024 * 512];
        uint32_t len = 0;
    };

public:
    string server_ip;
    unsigned int server_port;
    int sockfd = 0;

    bool isRun = false;//运行标志
    timeval receive_time = {0, 0};

public:
    thread thread_recv;
    thread thread_processRecv;
    thread thread_processSend;
public:

    struct sockaddr_in server_addr;
    socklen_t len = sizeof(struct sockaddr_in);

    queue<Msg> queue_recv;
    pthread_mutex_t lock_recv = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond_recv = PTHREAD_COND_INITIALIZER;

    queue<Msg> queue_send;
    pthread_mutex_t lock_send = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond_send = PTHREAD_COND_INITIALIZER;
public:
    //usrProcessRecv
    typedef void (*ProcessRecv)(Msg msg, void *pUser);

    ProcessRecv UserProcessRecv;
    void *pUser;

    ClientUdp(string server_ip, unsigned int server_port, ProcessRecv pProcessRecv, void *pUser);

    ~ClientUdp();

public:
    int Open();

    int Run();

    int Close();

private:
    static void ThreadRecv(void *p);

    static void ThreadProcessRecv(void *p);

    static void ThreadProcessSend(void *p);

public:
    //send to server
    int Send(Msg msg);

    int SendToBase(const char *buf, int len);

};


#endif //_CLIENTUDP_H
