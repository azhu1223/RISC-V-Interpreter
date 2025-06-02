#ifndef REGISTER_TABLE_H
#define REGISTER_TABLE_H

#include "Utility.h"

#include <array>
#include <cstdint>

using RegisterArray = std::array<uint32_t, 32>;

class RegisterTable {
public:
    RegisterTable(u_ptr<RegisterArray> regArray);
    bool set(uint8_t reg, uint32_t val);
    uint32_t get(uint8_t reg) const;
private:
    u_ptr<RegisterArray> m_regArray;
    int m_numRegisters;
};

#endif
