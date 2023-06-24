#include "Server.h"
#include "Socket.h"
#include "Acceptor.h"
#include "Connection.h"
#include "EventLoop.h"
#include <unistd.h>
#include <functional>
#include "myThreadPool/ThreadPool.h"

Server::Server(EventLoop *_loop) : mainReactor(_loop), acceptor(nullptr)
{
    acceptor = new Acceptor(mainReactor);
    std::function<void(Socket*)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor->setNewConnectionCallback(cb);
    threadpool = new ThreadPool();
    int size = threadpool->getThreadNum();
    for (int i = 0; i < size; i++)
    {
        subReactors.push_back(new EventLoop());
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
        std::function<void(int)> cb = std::bind(&Server::deleteConnection, this, std::placeholders::_1);
        conn->setDeleteConnectionCallback(cb);
        connections[sock->getFd()] = conn;
    }
}

void Server::deleteConnection(int sockfd)
{
    if (sockfd != -1)
    {
        if (connections.find(sockfd) != connections.end())
        {
            Connection *conn = connections[sockfd];
            connections.erase(sockfd);
            delete conn;
        }
    }
}