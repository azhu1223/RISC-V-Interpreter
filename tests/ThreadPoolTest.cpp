#include <gtest/gtest.h>

#include "ThreadPool.h"

#include <chrono>
#include <thread>
#include <functional>

class ThreadPoolTest : public ::testing::Test {
protected:
    ThreadPool m_tp;
    int nThreads = 4;
};

TEST_F(ThreadPoolTest, StartTest) {
    EXPECT_TRUE(m_tp.start(nThreads));
}

TEST_F(ThreadPoolTest, StopTest) {
    EXPECT_TRUE(m_tp.stop());
}

TEST_F(ThreadPoolTest, QueueJobTest) {
    m_tp.start(nThreads);

    EXPECT_TRUE(m_tp.queueJob([] () {}));
}

TEST_F(ThreadPoolTest, IsBusyTest) {
    m_tp.start(1);

    // Define lambda to simulate work
    std::function<void()> job = [] () {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    };

    EXPECT_FALSE(m_tp.isBusy());

    m_tp.queueJob(job);
    m_tp.queueJob(job);

    EXPECT_TRUE(m_tp.isBusy());
}
