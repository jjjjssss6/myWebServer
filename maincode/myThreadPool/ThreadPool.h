#pragma once
#include <functional>
#include <vector>
#include <thread>
#include <iostream>
#include <atomic>
#include <future>
#include "SafeQueue.h"

class ThreadPool
{
private:
    std::atomic_bool done;
    int threadNum;
    std::vector<std::thread> thds;
    SafeQueue<std::function<void()>> workers;
    void work()
    {
        while (!done)
        {
            std::function<void()> task;
            if (workers.pop(task))
            {
                task();
            }
            else
            {
                std::this_thread::yield();
            }
        }
    }
public:
    ThreadPool() : done(false), threadNum(std::thread::hardware_concurrency())
    {
        for (int i = 0; i < threadNum; i++)
        {
            thds.emplace_back(std::thread(&ThreadPool::work, this));
        }
    }
    ~ThreadPool()
    {
        done = true;
        for (int i = 0; i < threadNum; i++)
        {
            if (thds[i].joinable())
                thds[i].join();
        }
    }
    // template<typename Func>
    // void submit(Func f)
    // {
    //     workers.push(std::function<void()>(f));
    // }

    template<typename Func, typename... Args>
    auto submit(Func &&f, Args &&...args) -> std::future<typename std::result_of<Func(Args...)>::type>
    {
        using resultType = typename std::result_of<Func(Args...)>::type;
        auto task = std::make_shared<std::packaged_task<resultType()>>(
            std::bind(std::forward<Func>(f), std::forward<Args>(args)...)
        );
        auto ans = task->get_future();
        workers.push([task](){(*task)();});
        return ans;
    }

    int getThreadNum()
    {
        return threadNum;
    }
};