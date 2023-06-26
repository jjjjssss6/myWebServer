#pragma once
#include <functional>

class Epoll;
class Channel;
class ThreadPool;
class Timer;
class EventLoop
{
private:
    Epoll *ep;
    ThreadPool *threadPool;
    Timer *timer;
    int timeout;
    bool quit;
public:
    EventLoop(Timer *_timer);
    ~EventLoop();
    int getEpfd();
    Timer* getTimer();


    void loop();
    void updateChannel(Channel*);

    void addThread(std::function<void()>);
};