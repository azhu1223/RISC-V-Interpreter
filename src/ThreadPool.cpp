#include "ThreadPool.h"
#include "Utility.h"

#include <thread>
#include <queue>
#include <vector>
#include <mutex>
#include <functional>

void ThreadPool::start() {
    int nThreads = std::thread::hardware_concurrency();
    for (int i = 0; i < nThreads; i++) {
        m_threads.emplace_back(&ThreadPool::threadLoop, this);
    }

}

void ThreadPool::queueJob(std::function<void()> job) {
    {
        u_lock lock(m_queueMutex);
        m_jobs.push(job);
    }

    m_mutexCondition.notify_one();
}

void ThreadPool::stop() {
    {
        u_lock lock(m_queueMutex);
        m_shouldTerminate = true;
    }

    m_mutexCondition.notify_all();
    for (std::thread& thread : m_threads) {
        thread.join();
    }

    m_threads.clear();
}

bool ThreadPool::isBusy() {
    bool isBusy;
    {
        u_lock lock(m_queueMutex);
        isBusy = !m_jobs.empty();
    }

    return isBusy;
}

void ThreadPool::threadLoop() {
    while (true) {
        std::function<void()> job;
        {
            u_lock lock(m_queueMutex);
            m_mutexCondition.wait(lock, [this] {
                return !m_jobs.empty() || m_shouldTerminate;
            });

            if (m_shouldTerminate) {
                return;
            }

            job = m_jobs.front();
            m_jobs.pop();
        }

        job();
    }
}

ThreadPool::~ThreadPool() {
    stop();
}
