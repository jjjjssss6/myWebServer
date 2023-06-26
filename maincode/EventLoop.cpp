#include "EventLoop.h"
#include "Epoll.h"
#include "Channel.h"
#include "myThreadPool/ThreadPool.h"
#include <vector>

EventLoop::EventLoop() : ep(nullptr), quit(false)
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

void EventLoop::loop()
{
    while (!quit)
    {
        std::vector<Channel *> chs;
        chs = ep->poll();
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