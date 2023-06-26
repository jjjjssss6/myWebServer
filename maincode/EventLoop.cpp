#include "EventLoop.h"
#include "Epoll.h"
#include "Channel.h"
#include "myThreadPool/ThreadPool.h"
#include "Timer/Timer.h"
#include <vector>

EventLoop::EventLoop(Timer *_timer) : timer(_timer), ep(nullptr), quit(false), timeout(5000)
{
    ep = new Epoll();
    threadPool = new ThreadPool();
}

EventLoop::~EventLoop()
{
    quit = 1;
    delete ep;
    delete threadPool;
}

int EventLoop::getEpfd()
{
    return ep->getEpfd();
}

Timer* EventLoop::getTimer()
{
    return timer;
}

void EventLoop::loop()
{
    while (!quit)
    {
        timer->handleTimeoutEvents();
        std::vector<Channel *> chs;
        chs = ep->poll(timeout);
        for (auto it = chs.begin(); it != chs.end(); ++it)
        {
            // (*it)->handleEvent();
            addThread(std::bind(&Channel::handleEvent, (*it)));
        }
    }
}

void EventLoop::updateChannel(Channel *ch)
{
    ep->updateChannel(ch);
}

void EventLoop::addThread(std::function<void()> func)
{
    threadPool->submit(func);
}