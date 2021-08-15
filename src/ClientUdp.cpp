//
// Created by lining on 8/14/21.
//

#include "ClientUdp.h"
#include <iostream>
#include <cstring>
#include <sys/time.h>
#include <unistd.h>

using namespace std;

ClientUdp::ClientUdp(string server_ip, unsigned int server_port, ProcessRecv pProcessRecv, void *pUser) {
    this->server_port = server_port;
    this->server_ip = server_ip;
    this->UserProcessRecv = pProcessRecv;
    this->pUser = pUser;
    isRun = false;
}

ClientUdp::~ClientUdp() {
    this->Close();
}

int ClientUdp::Open() {
    if (this->server_ip.empty()) {
        cout << "server ip empty" << endl;
        return -1;
    }

    int sockfd = 0;
    sockfd = socket(AF_INET, SOCK_DGRAM/*SOCK_STREAM*/, 0/*IPPROTO_TCP*/);
    if (sockfd == -1) {
        cout << "create socket failed" << endl;
        return -1;
    } else {
        int opt = 1;
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        timeval timeout;
        timeout.tv_sec = 3;
        timeout.tv_usec = 0;
        setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *) &timeout, sizeof(struct timeval));
        setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof(struct timeval));

        int recvBufSize = 32 * 1024;
        int sendBufSize = 32 * 1024;
        setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, (char *) &recvBufSize, sizeof(int));
        setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, (char *) &sendBufSize, sizeof(int));
    }

    int ret = 0;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(this->server_port);

    server_addr.sin_addr.s_addr = inet_addr(this->server_ip.c_str());
    ret = connect(sockfd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr));

    timeval tv_now;
    gettimeofday(&tv_now, nullptr);

    if (ret == -1) {
        cout << "connect server:" << this->server_ip << " port: " << to_string(this->server_port) << " failed"
             << ",at " << ctime((time_t *) &tv_now.tv_sec) << endl;
        close(sockfd);
        return -1;
    }

    cout << "connect server:" << this->server_ip << " port: " << to_string(this->server_port) << " success"
         << ",at " << ctime((time_t *) &tv_now.tv_sec) << endl;

    this->sockfd = sockfd;
    isRun = true;

    return 0;
}

int ClientUdp::Run() {
    if (!this->isRun) {
        cout << "server not connect" << endl;
        return -1;
    }

    //start pthread
    thread_recv = thread(ThreadRecv, this);
    thread_recv.detach();

    thread_processRecv = thread(ThreadProcessRecv, this);
    thread_processRecv.detach();

    thread_processSend = thread(ThreadProcessSend, this);
    thread_processSend.detach();

    return 0;
}

int ClientUdp::Close() {
    isRun = false;

    if (sockfd > 0) {
        close(sockfd);
    }

    return 0;
}

void ClientUdp::ThreadRecv(void *p) {
    if (p == nullptr) {
        return;
    }
    auto client = (ClientUdp *) p;

    char buf[1024 * 512] = {0};
    int nread = 0;

    cout << "ClientUdp " << __FUNCTION__ << " run" << endl;
    while (client->isRun) {
        usleep(1000 * 10);
        memset(buf, 0, sizeof(buf));
        nread = recvfrom(client->sockfd, buf, sizeof(buf), 0, (struct sockaddr *) &client->server_addr,
                         &client->len);
        if (nread < 0) {
            if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN) {
                continue;
            }
            cout << "recv info error,errno:" << to_string(errno) << endl;
            close(client->sockfd);
            timeval tv_now;
            gettimeofday(&tv_now, nullptr);
            cout << "close sock:" << to_string(client->sockfd) << ",at " << ctime((time_t *) &tv_now.tv_sec)
                 << endl;

            client->isRun = false;
        } else if (nread > 0) {
            cout << "recv info" << endl;
            //update receive_time
            gettimeofday(&client->receive_time, nullptr);
            //把接受数据放到队列
            Msg msg;
            bzero(msg.buf, sizeof(msg.buf));
            memcpy(msg.buf, buf, nread);
            msg.len = nread;

            //try lock_recv
            pthread_mutex_lock(&client->lock_recv);
            client->queue_recv.push(msg);
            pthread_cond_broadcast(&client->cond_recv);
            pthread_mutex_unlock(&client->lock_recv);
        }
    }
    cout << "ClientUdp " << __FUNCTION__ << " exit" << endl;
}

void ClientUdp::ThreadProcessRecv(void *p) {
    if (p == nullptr) {
        return;
    }
    auto client = (ClientUdp *) p;

    cout << "ClientUdp " << __FUNCTION__ << " run" << endl;
    while (client->isRun) {
        //获取接口数据
        pthread_mutex_lock(&client->lock_recv);
        while (client->queue_recv.empty()) {
            pthread_cond_wait(&client->cond_recv, &client->lock_recv);
        }
        //task
        //pop all msg process
        while (!client->queue_recv.empty()) {
            Msg msg = client->queue_recv.front();
            client->queue_recv.pop();

            //record receive time
            gettimeofday(&client->receive_time, nullptr);

            //将buf内容以16进制形式打印出来
            string buf_str;
            buf_str.clear();
            for (int i = 0; i < msg.len; i++) {
                char tmp[4];
                bzero(tmp, sizeof(tmp));
                sprintf(tmp,"%02x ",msg.buf[i]);
                buf_str.append(tmp);
            }


            //这是一个单纯打印接收的流程，可以根据实际(比如json中code，来进行区分)
            cout << "recv client info,len:" << msg.len << ",msg:" << buf_str << endl;
            //回环 heartbeat opened loop must close
            bool enableLoop = false;
            if (enableLoop) {
                client->Send(msg);
                continue;
            }

            //获取数据，依次处理
            if (client->UserProcessRecv) {
                client->UserProcessRecv(msg, client->pUser);
            }

        }
        pthread_mutex_unlock(&client->lock_recv);

    }
    cout << "ClientUdp " << __FUNCTION__ << " exit" << endl;
}

void ClientUdp::ThreadProcessSend(void *p) {
    if (p == nullptr) {
        return;
    }
    auto client = (ClientUdp *) p;

    cout << "ClientUdp " << __FUNCTION__ << " run" << endl;
    while (client->isRun) {
        //try lock_recv
        pthread_mutex_lock(&client->lock_send);
        while (client->queue_send.empty()) {
            pthread_cond_wait(&client->cond_send, &client->lock_send);
        }
        //task pop all msg
        while (!client->queue_send.empty()) {
            Msg msg = client->queue_send.front();
            client->queue_send.pop();

            //将buf内容以16进制形式打印出来
            string buf_str;
            buf_str.clear();
            for (int i = 0; i < msg.len; i++) {
                char tmp[4];
                bzero(tmp, sizeof(tmp));
                sprintf(tmp,"%02x ",msg.buf[i]);
                buf_str.append(tmp);
            }


            int ret = sendto(client->sockfd, msg.buf, msg.len, 0, (struct sockaddr *) &client->server_addr,
                             client->len);
            if (ret != msg.len) {
                if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN) {
                    continue;
                }
                cout << "msg:" << buf_str<< "send fail,errno:" << to_string((errno)) << endl;
                close(client->sockfd);
                timeval tv_now;
                gettimeofday(&tv_now, nullptr);
                cout << "close sock:" << to_string(client->sockfd) << ",at " << ctime((time_t *) &tv_now.tv_sec)
                     << endl;
                client->isRun = false;
            } else {
                cout << "msg:" << buf_str << "send ok" << endl;
            }
        }
        pthread_mutex_unlock(&client->lock_send);
    }
    cout << "ClientUdp " << __FUNCTION__ << " exit" << endl;
}

int ClientUdp::Send(ClientUdp::Msg msg) {
    if (msg.len <= 0) {
        return -1;
    }
    //try send lock_queue_recv
    pthread_mutex_lock(&this->lock_send);
    this->queue_send.push(msg);
    pthread_cond_broadcast(&this->cond_send);
    pthread_mutex_unlock(&this->lock_send);

    return msg.len;
}

int ClientUdp::SendToBase(const char *buf, int len) {
    ClientUdp::Msg msg;
    bzero(msg.buf, sizeof(msg.buf));
    memcpy(msg.buf, buf, len);
    msg.len = len;
    return Send(msg);
}
