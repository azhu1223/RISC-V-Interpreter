#include "InstructionDecoder.h"
#include "Utility.h"

InstructionType InstructionDecoder::determineInstructionType(const Opcode& opcode) const {
    // Tests if it's a branch or jump type instruction
    if (opcode.test(6)) {
        if (opcode.test(2)) {
            // Test if a J-type instruction
            if (opcode.test(3)) {
                return InstructionType::J;
            }

            // Else is an I-type instruction
            else {
                return InstructionType::I;
            }
        }

        // If not set, then it's a B-type instruction
        else {
            return InstructionType::B;
        }
    }

    else {
        // Test if a U-type instruction
        if (opcode.test(2)) {
            return InstructionType::U;
        }

        else {
            if (opcode.test(5)) {
                // Test if a R-type instruction
                if (opcode.test(4)) {
                    return InstructionType::R;
                }

                // Else is an S-type instruction
                else {
                    return InstructionType::S;
                }
            }

            // If not set, then it's an I-type instruction.
            else {
                return InstructionType::I;
            }
        }
    }
}

// Decodes the instruction provided.
DecodedInstruction InstructionDecoder::decodeInstruction(const Instruction& instruction) const {
    Opcode opcode;
    RegisterAddress rd;
    Funct3 funct3;
    RegisterAddress rs1;
    RegisterAddress rs2;
    Funct7 funct7;

    unsigned long long instructionLong = instruction.to_ullong();

    opcode = instructionLong & 0x7f;
    instructionLong >>= 7;
    rd = instructionLong & 0x1f;
    instructionLong >>= 5;
    funct3 = instructionLong & 0x7;
    instructionLong >>= 3;
    rs1 = instructionLong & 0x1f;
    instructionLong >>= 5;
    rs2 = instructionLong & 0x1f;
    instructionLong >>= 5;
    funct7 = instructionLong;

    return {opcode, rd, funct3, rs1, rs2, funct7, determineInstructionType(opcode)};
}