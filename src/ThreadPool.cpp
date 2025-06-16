#include "ThreadPool.h"
#include "Utility.h"

#include <thread>
#include <mutex>
#include <functional>

bool ThreadPool::start(int nThreads) {
    for (int i = 0; i < nThreads; i++) {
        m_threads.emplace_back(&ThreadPool::threadLoop, this);
    }

    return true;
}

bool ThreadPool::queueJob(std::function<void()> job) {
    {
        u_lock lock(m_queueMutex);
        m_jobs.push(job);
    }

    m_mutexCondition.notify_one();

    return true;
}

bool ThreadPool::stop() {
    {
        u_lock lock(m_queueMutex);
        m_shouldTerminate = true;
    }

    m_mutexCondition.notify_all();
    for (std::thread& thread : m_threads) {
        thread.join();
    }

    m_threads.clear();

    return true;
}

bool ThreadPool::isBusy() {
    bool isBusy;
    {
        u_lock lock(m_queueMutex);
        isBusy = !(m_jobs.empty() && m_nProcessesRunning > 0);
    }

    return isBusy;
}

bool ThreadPool::wait() {
    u_lock lock(m_mainThreadMutex);

    if (isBusy()) {
        m_mainThreadCondition.wait(lock, [this] () {
            return !this->isBusy();
        });
    }
}

void ThreadPool::threadLoop() {
    while (true) {
        std::function<void()> job;
        {
            u_lock lock(m_queueMutex);

            if (m_jobs.empty()) {
                m_mutexCondition.wait(lock, [this] {
                    return !m_jobs.empty() || m_shouldTerminate;
                });
            }

            if (m_shouldTerminate) {
                return;
            }

            m_nProcessesRunning++;

            job = m_jobs.front();
            m_jobs.pop();
        }

        job();

        u_lock queueLock(m_queueMutex);

        if (--m_nProcessesRunning == 0 && m_jobs.empty()) {
            queueLock.unlock();
            
            u_lock mainLock(m_mainThreadMutex);

            m_mainThreadCondition.notify_one();
        }
    }
}

ThreadPool::~ThreadPool() {
    stop();
}
