#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "Utility.h"

#include <thread>
#include <queue>
#include <vector>
#include <mutex>
#include <functional>
#include <condition_variable>
#include <atomic>

class ThreadPool {
public:
    bool start(int nThreads);
    bool stop();
    bool isBusy();
    bool queueJob(std::function<void()> job);
    bool wait();
    ~ThreadPool();
private:
    void threadLoop();

    bool m_shouldTerminate;
    std::mutex m_queueMutex;
    std::condition_variable m_mutexCondition;
    std::vector<std::thread> m_threads;
    std::queue<std::function<void()>> m_jobs;

    std::mutex m_mainThreadMutex;
    std::condition_variable m_mainThreadCondition;
    std::atomic<int> m_nProcessesRunning;
};

#endif
