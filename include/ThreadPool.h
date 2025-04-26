#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "Utility.h"

#include <thread>
#include <queue>
#include <vector>
#include <mutex>
#include <functional>

class ThreadPool {
public:
    void start();
    void queueJob(std::function<void()>);
    void stop();
    bool isBusy();
    ~ThreadPool();
private:
    void threadLoop();

    bool m_shouldTerminate;
    std::mutex m_queueMutex;
    std::condition_variable m_mutexCondition;
    std::vector<std::thread> m_threads;
    std::queue<std::function<void()>> m_jobs;
};

#endif
