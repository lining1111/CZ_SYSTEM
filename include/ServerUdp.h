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
        uint8_t buf[1024 * 512];
        uint32_t len = 0;
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

    vector<ClientInfo *> vector_clients;//记录的客户端信息
    pthread_mutex_t lock_vector_client = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond_vector_client = PTHREAD_COND_INITIALIZER;

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
    typedef void (*ProcessRecv)(ServerUdp &server, Msg msg, void *user);

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

    /**
     * 对比两个客户端是否一致
     * @param clientA 客户端A
     * @param clientB 客户端B
     * @return 
     */
    static bool IsSameClient(struct sockaddr_in clientA, struct sockaddr_in clientB);

    /**
     * 添加接入的客户端列表
     * @param client_addr 客户端信息
     * @brief 客户端列表数量为0,直接添加
     *          客户端列表不为0,则对比已存入的客户信息，如果相同，则不存，如果不同则存入
     */
    void AddClient(struct sockaddr_in client_addr);

    /**
     * 删除已存入的客户端
     * @param client_addr 客户端信息
     */
    void RemoveClient(struct sockaddr_in client_addr);

    /**
     * 标记客户端信息
     * @param client_addr 客户端信息
     * @param devType 客户端类型，以通讯定义为准
     */
    void MarkClient(struct sockaddr_in client_addr, DevType devType);

public:
    //send to client
    int Send(Msg msg);

};


#endif //_SERVERUDP_H
