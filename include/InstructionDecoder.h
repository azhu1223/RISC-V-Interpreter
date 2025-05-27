#ifndef OPCODE_DECODER
#define OPCODE_DECODER

#include "Utility.h"

#include <bitset>

// An Opcode is 7 bits wide.
using Opcode = std::bitset<7>;

// A register address is 5 bits wide.
using RegisterAddress = std::bitset<5>;

// A Funct3 is 3 bits wide.
using Funct3 = std::bitset<3>;

// Immediates can be either 12 or 20 bits wide.
using Immediate12 = std::bitset<12>;
using Immediate20 = std::bitset<20>;

// A Funct7 is 7 bits wide.
using Funct7 = std::bitset<7>;

enum class InstructionType {
    R,
    I,
    S,
    B,
    U,
    J
};

struct DecodedInstruction {
    Opcode opcode;
    RegisterAddress rd;
    Funct3 funct3;
    RegisterAddress rs1;
    RegisterAddress rs2;
    Funct7 funct7;
    InstructionType type;
};

struct DecodedImmediate {
    Immediate12 iImmediate;
    Immediate12 sImmediate;
    Immediate12 bImmediate;
    Immediate20 uImmediate;
    Immediate20 jImmediate;
};

class InstructionDecoder {
public:
    DecodedInstruction decodeInstruction(const Instruction& instruction) const;
    DecodedImmediate generateImmediate(const Instruction& instruction) const;
private:
    InstructionType determineInstructionType(const Opcode& opcode) const;
};

#endif
