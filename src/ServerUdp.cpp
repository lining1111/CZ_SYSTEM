//
// Created by lining on 8/14/21.
//

#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/time.h>
#include "include/ServerUdp.h"

using namespace std;


ServerUdp::ServerUdp(ProcessRecv pProcessRecv, void *pUser) {
    this->port = default_port;
    this->UserProcessRecv = pProcessRecv;
    this->pUser = pUser;
    isRun = false;
    vector_clients.clear();
}

ServerUdp::ServerUdp(unsigned int port, ProcessRecv pProcessRecv, void *pUser) {
    this->port = port;
    this->UserProcessRecv = pProcessRecv;
    this->pUser = pUser;
    isRun = false;
    vector_clients.clear();
}

ServerUdp::~ServerUdp() {
    this->Close();
}

int setnonblocking(int sockfd) {
    if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0) | O_NONBLOCK) == -1) {
        return -1;
    }
    return 0;
}

int ServerUdp::Open() {
    //bind
    int sockfd;
    sockfd = socket(AF_INET, SOCK_DGRAM/*SOCK_STREAM| SOCK_NONBLOCK*/, 0);
    cout << "server sock:" << to_string(sockfd) << endl;
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(this->port);
    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    struct timeval timeout;
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *) &timeout, sizeof(struct timeval));
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof(struct timeval));

    int recvBufSize = 32 * 1024;
    int sendBufSize = 32 * 1024;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, (char *) &recvBufSize, sizeof(int));
    setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, (char *) &sendBufSize, sizeof(int));

    //将套接字绑定到服务器的网络地址上
    if (bind(sockfd, (struct sockaddr *) &server_addr, (socklen_t) sizeof(server_addr)) < 0) {
        cout << "bind error" << endl;
        return -1;
    }
    //listen
    listen(sockfd, max_listen);
    //创建一个epoll句柄

    epoll_fd = epoll_create(MAX_EVENTS);
    if (epoll_fd == -1) {
        cout << "epoll create fail" << endl;
        return -1;
    }
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = sockfd;
    // 向epoll注册server_sockfd监听事件
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sockfd, &ev) == -1) {
        cout << "epoll_ctl server fd failed" << endl;
        close(sockfd);
        return -1;
    }
    this->listen_sock = sockfd;
    return 0;
}

int ServerUdp::Run() {
    if (listen_sock <= 0) {
        return -1;
    }

    isRun = true;
    thread_acceptClient = thread(ThreadAcceptClient, this);
    thread_acceptClient.detach();

    if (UserProcessRecv) {
        thread_processRecv = thread(ThreadProcessRecv, this);
        thread_processRecv.detach();
    }

    thread_processSend = thread(ThreadProcessSend, this);
    thread_processSend.detach();

    return 0;
}

int ServerUdp::Close() {
    isRun = false;
    //close server fd
    if (listen_sock > 0) {
        close(listen_sock);
    }
    vector_clients.clear();
    return 0;
}

void ServerUdp::ThreadAcceptClient(void *p) {
    if (p == nullptr) {
        return;
    }
    auto server = (ServerUdp *) p;
    int nfds = 0;// epoll监听事件发生的个数

    cout << "ServerUdp " << __FUNCTION__ << " run" << endl;
    while (server->isRun) {
        // 等待事件发生
        nfds = epoll_wait(server->epoll_fd, server->wait_events, MAX_EVENTS, -1);

        if (nfds == -1) {
            //            cout << "start epoll wait fail" << endl;
            continue;
        }
        for (int i = 0; i < nfds; i++) {

            if (server->wait_events[i].data.fd == server->listen_sock) {
                // 客户端有数据发送过来
                struct sockaddr_in client_addr;
                socklen_t cli_len = sizeof(client_addr);

                char buf[1024 * 32] = {0};
                int len = 0;
                bzero(buf, sizeof(buf));
                len = recvfrom(server->listen_sock, buf, 1024 * 32, 0, (struct sockaddr *) &client_addr, &cli_len);

                if (len < 0) {
                    cout << "recv fail" << endl;
                    //刪除客戶端信息
                    server->RemoveClient(client_addr);
                } else if (len > 0) {
                    //记录客户端信息
                    server->AddClient(client_addr);

                    //记录信息
                    Msg msg;
                    memcpy(&msg.client_addr, &client_addr, cli_len);
                    bzero(msg.buf, sizeof(msg.buf));
                    memcpy(msg.buf, buf, len);
                    msg.len = len;

                    //try lock_recv
                    pthread_mutex_lock(&server->lock_recv);
                    server->queue_recv.push(msg);
                    pthread_cond_broadcast(&server->cond_recv);
                    pthread_mutex_unlock(&server->lock_recv);
                }
            }
        }
    }
    close(server->epoll_fd);
    close(server->listen_sock);
    cout << "ServerUdp " << __FUNCTION__ << " exit" << endl;
}

void ServerUdp::ThreadProcessRecv(void *p) {
    if (p == nullptr) {
        return;
    }
    auto server = (ServerUdp *) p;

    cout << "ServerUdp " << __FUNCTION__ << " run" << endl;
    while (server->isRun) {

        usleep(1000 * 10);
        if (!server->isRun) {
            continue;
        }
        //获取接口数据
        pthread_mutex_lock(&server->lock_recv);
        while (server->queue_recv.empty()) {
            pthread_cond_wait(&server->cond_recv, &server->lock_recv);
        }
        //task
        //pop all msg process
        while (!server->queue_recv.empty()) {
            ServerUdp::Msg msg = server->queue_recv.front();
            server->queue_recv.pop();

            //将buf内容以16进制形式打印出来
            string buf_str;
            buf_str.clear();
            for (int i = 0; i < msg.len; i++) {
                char tmp[4];
                bzero(tmp, sizeof(tmp));
                sprintf(tmp, "%02x ", msg.buf[i]);
                buf_str.append(tmp);
            }

            //这是一个单纯打印接收的流程，可以根据实际(比如json中code，来进行区分)
            cout << "recv client info,len:" << msg.len << ",info:" << buf_str << endl;
            //回环 heartbeat opened loop must close
            bool enableLoop = false;
            if (enableLoop) {
                server->Send(msg);
                continue;
            }

            //获取数据，依次处理
            if (server->UserProcessRecv) {
                server->UserProcessRecv(*server, msg, server->pUser);
            }

        }
        pthread_mutex_unlock(&server->lock_recv);

    }
    cout << "ServerUdp " << __FUNCTION__ << " exit" << endl;
}

void ServerUdp::ThreadProcessSend(void *p) {
    if (p == nullptr) {
        return;
    }
    auto server = (ServerUdp *) p;

    cout << "ServerUdp " << __FUNCTION__ << " run" << endl;
    while (server->isRun) {
        //try lock_recv
        pthread_mutex_lock(&server->lock_send);
        while (server->queue_send.empty()) {
            pthread_cond_wait(&server->cond_send, &server->lock_send);
        }
        //task pop all msg
        while (!server->queue_send.empty()) {
            Msg msg;
            msg = server->queue_send.front();
            server->queue_send.pop();

            //将buf内容以16进制形式打印出来
            string buf_str;
            buf_str.clear();
            for (int i = 0; i < msg.len; i++) {
                char tmp[4];
                bzero(tmp, sizeof(tmp));
                sprintf(tmp, "%02x ", msg.buf[i]);
                buf_str.append(tmp);
            }

            socklen_t cli_len = sizeof(struct sockaddr_in);
            //服务端发送数据，要把数据发送给正确的客户端
            int ret = sendto(server->listen_sock, msg.buf, msg.len, 0, (struct sockaddr *) &msg.client_addr, cli_len);
            if (ret != msg.len) {
                if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN) {
                    continue;
                }
                cout << "msg:" << buf_str << ",send fail,errno:" << to_string((errno)) << ",client:"
                     << inet_ntoa(msg.client_addr.sin_addr) << endl;
                //移除记录客户端信息
                server->RemoveClient(msg.client_addr);
            } else {
                cout << "msg:" << buf_str << ",send ok" << ",client:"
                     << inet_ntoa(msg.client_addr.sin_addr) << endl;
            }
        }
        pthread_mutex_unlock(&server->lock_send);
    }
    cout << "ServerUdp " << __FUNCTION__ << " exit" << endl;
}

bool ServerUdp::IsSameClient(struct sockaddr_in clientA, struct sockaddr_in clientB) {
    int same = memcmp(&clientA, &clientB, sizeof(struct sockaddr_in));

    if (same == 0) {
        return true;
    } else {
        return false;
    }

}

void ServerUdp::AddClient(struct sockaddr_in client_addr) {
    if (vector_clients.empty()) {
        ClientInfo *clientInfo = new ClientInfo(client_addr, Common::UnknownDev);
        //add vector
        pthread_mutex_lock(&this->lock_vector_client);
        vector_clients.push_back(clientInfo);
        pthread_cond_broadcast(&this->cond_vector_client);
        pthread_mutex_unlock(&this->lock_vector_client);

        timeval tv_now;
        gettimeofday(&tv_now, nullptr);
        cout << "add client ip:" << inet_ntoa(client_addr.sin_addr) << " port:"
        << to_string(client_addr.sin_port) << ",at " << ctime((time_t *) &tv_now.tv_sec) << endl;
    } else {
        bool isRecord = false;
        for (auto iter:vector_clients) {
            if (IsSameClient(client_addr, iter->clientAddr)) {
                isRecord = true;
                break;
            }
        }
        if (!isRecord) {
            ClientInfo *clientInfo = new ClientInfo(client_addr, Common::UnknownDev);
            //add vector
            pthread_mutex_lock(&this->lock_vector_client);
            vector_clients.push_back(clientInfo);
            pthread_cond_broadcast(&this->cond_vector_client);
            pthread_mutex_unlock(&this->lock_vector_client);

            timeval tv_now;
            gettimeofday(&tv_now, nullptr);
            cout << "add client ip:" << inet_ntoa(client_addr.sin_addr) << " port:"
                 << to_string(client_addr.sin_port) << ",at " << ctime((time_t *) &tv_now.tv_sec) << endl;
        }
    }
}

void ServerUdp::RemoveClient(struct sockaddr_in client_addr) {
    if (vector_clients.empty()) {
        return;
    }
    pthread_mutex_lock(&this->lock_vector_client);
    //vector erase
    for (int i = 0; i < vector_clients.size(); i++) {
        auto iter = vector_clients.at(i);
        if (IsSameClient(client_addr, iter->clientAddr)) {
            //delete client class release clientInfo->rb
            delete iter;

            //erase vector
            vector_clients.erase(vector_clients.begin() + i);
            vector_clients.shrink_to_fit();

            timeval tv_now;
            gettimeofday(&tv_now, nullptr);
            cout << "remove client ip:" << inet_ntoa(client_addr.sin_addr) << " port:"
                 << to_string(client_addr.sin_port) << ",at " << ctime((time_t *) &tv_now.tv_sec) << endl;
            break;
        }
    }
    pthread_cond_broadcast(&this->cond_vector_client);
    pthread_mutex_unlock(&this->lock_vector_client);
}

void ServerUdp::MarkClient(struct sockaddr_in client_addr, DevType devType) {
    if (vector_clients.empty()) {
        return;
    }

    for (auto iter:vector_clients) {
        if (iter->devType == Common::UnknownDev) {
            if (IsSameClient(client_addr,iter->clientAddr)){
                iter->devType = devType;
            }
        }
    }
}

int ServerUdp::Send(ServerUdp::Msg msg) {
    if (msg.len <= 0) {
        return -1;
    }
    //try lock_send
    pthread_mutex_lock(&this->lock_send);
    queue_send.push(msg);
    pthread_cond_broadcast(&this->cond_send);
    pthread_mutex_unlock(&this->lock_send);

    return msg.len;
}
