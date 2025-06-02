#ifndef REGISTER_TABLE_H
#define REGISTER_TABLE_H

#include "Utility.h"

#include <array>
#include <cstdint>

using RegisterArray = std::array<uint32_t, 32>;

class RegisterTable {
public:
    RegisterTable(u_ptr<RegisterArray> regArray);
    bool set(int reg, uint32_t val);
    int get(int reg) const;
private:
    u_ptr<RegisterArray> m_regArray;
};

#endif
