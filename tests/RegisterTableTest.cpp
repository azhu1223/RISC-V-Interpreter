#include <gtest/gtest.h>

#include "RegisterTable.h"
#include "Utility.h"

class RegisterTableTest : public ::testing::Test {
protected:
    RegisterTableTest() : m_rt(u_ptr<RegisterArray>(new RegisterArray)) {};
    RegisterTable m_rt;

    virtual void SetUp() override {
        for (int i = 0; i < 31; i++) {
            EXPECT_TRUE(m_rt.set(i, i + 1));
        }
    };
};

TEST_F(RegisterTableTest, Set) {
    for (int i = 1; i < 31; i++) {
        EXPECT_TRUE(m_rt.set(i, i + 1));
    }

    for (int i = 32; i < 40; i++) {
        EXPECT_FALSE(m_rt.set(i, i + 1));
    }
}

TEST_F(RegisterTableTest, Get) {
    EXPECT_EQ(m_rt.get(0), 0);

    for (int i = 1; i < 31; i++) {
        Result<uint32_t> result = m_rt.get(i);

        EXPECT_TRUE(result.first);
        EXPECT_EQ(result.second, i + 1);
    }

    for (int i = 32; i < 40; i++) {
        EXPECT_FALSE(m_rt.get(i).first);
    }
}
