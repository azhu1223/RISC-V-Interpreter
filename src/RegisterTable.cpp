#include "RegisterTable.h"
#include "Utility.h"

#include "Utility.h"

#include <memory>
#include <array>
#include <cstdint>

RegisterTable::RegisterTable(u_ptr<RegisterArray> regArray) : m_regArray(std::move(regArray)), m_numRegisters(m_regArray->size()) {
    (*m_regArray)[0] = 0;
}

bool RegisterTable::set(uint8_t reg, uint32_t val) {
    if (reg >= m_numRegisters) {
        return false;
    }

    // x0 should stay 0
    else if (reg != 0) {
        (*m_regArray)[reg] = val;
    }

    return true;
}

uint32_t RegisterTable::get(uint8_t reg) const {
    return (*m_regArray)[reg];
}
