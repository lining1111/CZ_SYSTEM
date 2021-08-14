//
// Created by lining on 8/14/21.
//

#ifndef _SERVERUDP_H
#define _SERVERUDP_H

#include "ClientInfo.h"
#include <vector>
#include <thread>
#include <queue>
#include <sys/epoll.h>

using namespace std;

class ServerUdp {
public:
    struct Msg {
        unsigned char buf[1024 * 512];
        int len = 0;
        struct sockaddr_in client_addr;
    };
public:
    const unsigned int default_port = 8313;
    unsigned int max_listen = 10;
    const long checkStatus_timeval = 5;
public:
    unsigned int port;
    int listen_sock = 0;
    //epoll
    int epoll_fd;
    struct epoll_event ev;
#define MAX_EVENTS 10
    struct epoll_event wait_events[MAX_EVENTS];
    bool isRun = false;//运行标志

    thread thread_acceptClient;
    thread thread_processRecv;
    thread thread_processSend;

    queue<Msg> queue_recv;
    pthread_mutex_t lock_recv = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond_recv = PTHREAD_COND_INITIALIZER;

    queue<Msg> queue_send;
    pthread_mutex_t lock_send = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond_send = PTHREAD_COND_INITIALIZER;

public:
    //processRecv
    typedef void (*ProcessRecv)(Msg msg,void *user);

    ProcessRecv UserProcessRecv;
    void *pUser;
public:
    ServerUdp(ProcessRecv pProcessRecv, void *pUser);

    ServerUdp(unsigned int port, ProcessRecv pProcessRecv, void *pUser);

    ~ServerUdp();

public:
    int Open();

    int Run();

    int Close();
private:
    static void ThreadAcceptClient(void *p);

    static void ThreadProcessRecv(void *p);

    static void ThreadProcessSend(void *p);

public:
    //send to client
    int Send(Msg msg);

};


#endif //_SERVERUDP_H
