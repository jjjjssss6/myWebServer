#pragma
#include <time.h>
#include <chrono>
#include <map>
#include <unordered_map>
#include <mutex>
#include <functional>
#include <vector>
#include <iostream>

struct timeNode
{
    int id;
    std::chrono::high_resolution_clock::time_point expire;
    std::function<void()> cb;
    bool operator < (const timeNode& o) const
    {
        return expire < o.expire;
    }
};
class Timer
{
public:
    Timer();
    ~Timer();

    void addExpire(int id, int time);
    void addToTimer(int id, int time, std::function<void()> cb);
    void delEvent(int id);

    void handleTimeoutEvents();


private:
    std::unordered_map<int, timeNode> mp;
    std::map<timeNode, int> eventQueue;
    std::mutex mtx;
};