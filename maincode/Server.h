#pragma once
#include <map>
#include <vector>
class EventLoop;
class Socket;
class Acceptor;
class Connection;
class ThreadPool;
class Timer;
class Server
{
public:
    Server(EventLoop *_loop);
    ~Server();

    // void handleReadEvent(int);
    void newConnection(Socket *sock);
    void deleteConnection(int sockfd, int epfd);

private:
    EventLoop *mainReactor;
    Acceptor *acceptor;
    std::map<int, Connection*> connections;
    std::vector<EventLoop*> subReactors;
    ThreadPool *threadpool;
    Timer *timer;
};