#include "Timer.h"

Timer::Timer()
{

}

Timer::~Timer()
{

}

void Timer::addExpire(int id, int time)
{
    if (mp.count(id) == 0)
    {
        printf("event doesn't exist\n");
    }
    else
    {
        timeNode event = mp[id];
        mp.erase(id);
        eventQueue.erase(event);
        timeNode newEvent = {id, std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(time), event.cb};
        mp[id] = newEvent;
        eventQueue[newEvent] = 1;
    }
}

void Timer::addToTimer(int id, int time, std::function<void()> cb)
{
    timeNode newEvent = {id, std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(time), cb};
    if (mp.count(id) != 0)
    {
        timeNode event = mp[id];
        mp.erase(id);
        eventQueue.erase(event);
        mp[id] = newEvent;
        eventQueue[newEvent] = 1;
    }
    else
    {
        mp[id] = newEvent;
        eventQueue[newEvent] = 1;
    }
}

void Timer::delEvent(int id)
{
    if (mp.count(id) == 0)
    {
        printf("event doesn't exist\n");
    }
    else
    {
        timeNode event = mp[id];
        mp.erase(id);
        eventQueue.erase(event);
    }
}

void Timer::handleTimeoutEvents()
{
    int deletedNum = 0;
    for (auto &[event, filler] : eventQueue)
    {
        if (std::chrono::duration_cast<std::chrono::milliseconds>(event.expire - std::chrono::high_resolution_clock::now()).count() > 0)
            break;
        event.cb();
        deletedNum++;
    }

}

