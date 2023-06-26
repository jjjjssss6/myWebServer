#include "Server.h"
#include "Socket.h"
#include "Acceptor.h"
#include "Connection.h"
#include "util.h"
#include "EventLoop.h"
#include <unistd.h>
#include "Epoll.h"
#include <functional>
#include "myThreadPool/ThreadPool.h"
#include "Timer/Timer.h"

Server::Server(EventLoop *_loop) : mainReactor(_loop), acceptor(nullptr), timer(mainReactor->getTimer()), timeout(5000)
{
    acceptor = new Acceptor(mainReactor);
    std::function<void(Socket*)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor->setNewConnectionCallback(cb);
    threadpool = new ThreadPool();
    int size = threadpool->getThreadNum();
    for (int i = 0; i < size; i++)
    {
        subReactors.push_back(new EventLoop(timer));
    }
    for (int i = 0; i < size; i++)
    {
        std::function<void()> sub_loop = std::bind(&EventLoop::loop, subReactors[i]);
        threadpool->submit(sub_loop);
    }
}

Server::~Server()
{
    delete acceptor;
    delete threadpool;
}

void Server::newConnection(Socket *sock)
{
    if (sock->getFd() != -1)
    {
        int idx = sock->getFd() % subReactors.size();
        Connection *conn = new Connection(subReactors[idx], sock);
        std::function<void(int)> cb = std::bind(&Server::deleteConnection, this, std::placeholders::_1, subReactors[idx]->getEpfd());
        conn->setDeleteConnectionCallback(cb);
        connections[sock->getFd()] = conn;
        timer->addToTimer(sock->getFd(), timeout, std::bind(&Server::deleteConnection, this, sock->getFd(), subReactors[idx]->getEpfd()));
    }
}

void Server::deleteConnection(int sockfd, int epfd)
{
    if (sockfd != -1)
    {
        if (connections.find(sockfd) != connections.end())
        {
            printf("EOF, client fd %d disconnected\n", sockfd);
            Connection *conn = connections[sockfd];
            connections.erase(sockfd);
            errif(epoll_ctl(epfd, EPOLL_CTL_DEL, sockfd, NULL) == -1, "epoll delete error");
            delete conn;
        }
    }
}