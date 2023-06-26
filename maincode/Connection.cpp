#include "Connection.h"
#include "EventLoop.h"
#include "Socket.h"
#include "Channel.h"
#include "util.h"
#include "Buffer.h"
#include "Timer/Timer.h"
#include <unistd.h>
#include <string.h>
#include <iostream>



Connection::Connection(EventLoop *_loop, Socket *_sock) : loop(_loop), sock(_sock), channel(nullptr), readBuffer(nullptr), timer(loop->getTimer())
{
    channel = new Channel(loop, sock->getFd());
    channel->enableRead();
    channel->useET();
    std::function<void()> cb = std::bind(&Connection::echo, this, sock->getFd());
    channel->setReadCallback(cb);
    readBuffer = new Buffer();
}

Connection::~Connection()
{
    delete channel;
    delete sock;
    delete readBuffer;
}

void Connection::echo(int sockfd)
{
    char buf[1024]; 
    while (true)
    { 
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if (bytes_read > 0)
        {
            timer->addExpire(sockfd, 5000);
            readBuffer->append(buf, bytes_read);
        }
        else if (bytes_read == -1 && errno == EINTR)
        { 
            timer->addExpire(sockfd, 5000);
            printf("continue reading\n");
            continue;
        }
        else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
        {   // 非阻塞IO，这个条件表示数据全部读取完毕
            timer->addExpire(sockfd, 5000);
            printf("message from client fd %d: %s\n", sockfd, readBuffer->c_str());
            send(sockfd);
            readBuffer->clear();
            break;
        }
        else if (bytes_read == 0)
        {   // EOF，客户端断开连接
            deleteConnectionCallback(sockfd);
            break;
        }
        else
        {
            printf("Connection reset by peer\n");
            deleteConnectionCallback(sockfd);
            break;
        }
    }
}

void Connection::send(int sockfd)
{
    char buf[readBuffer->size()];
    strcpy(buf, readBuffer->c_str());
    int data_size = readBuffer->size();
    int data_left = data_size;
    while (data_left > 0)
    {
        ssize_t bytes_write = write(sockfd, buf + data_size - data_left, data_left);
        if (bytes_write == -1 && errno == EAGAIN)
        {
            break;
        }
        data_left -= bytes_write;
    }
}

void Connection::setDeleteConnectionCallback(std::function<void(int)> _cb)
{
    deleteConnectionCallback = _cb;
}