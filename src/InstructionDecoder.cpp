#include "InstructionDecoder.h"
#include "Utility.h"

#include <cstdint>

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

DecodedImmediate InstructionDecoder::generateImmediate(const Instruction& instruction) const {
    int32_t iImmediate;
    int32_t sImmediate;
    int32_t bImmediate;
    int32_t uImmediate;
    int32_t jImmediate;

    unsigned long long instructionLong = instruction.to_ullong();
    // Get rid of the opcode, which is not relevant
    instructionLong >>= 7;

    int32_t sbFirstPart = instructionLong & 0x1f;
    instructionLong >>= 5;

    int32_t ujRawImmediate = instructionLong;
    instructionLong >>= 8;

    int32_t iRawImmediate = instructionLong & 0xfff;
    instructionLong >>= 5;

    int32_t sbSecondPart = instructionLong;

    // Sign extend
    iImmediate = (iRawImmediate << 20) >> 20;
    sImmediate = ((sbSecondPart << 25) >> 20) + sbFirstPart;

    int32_t bSignExtention = (((sbSecondPart & 0x40) << 25) >> 19);
    bImmediate = bSignExtention + ((sbFirstPart & 0x1) << 11) + ((sbSecondPart & 0x3f) << 5) + (sbFirstPart & 0x1e);

    uImmediate = ujRawImmediate << 12;

    int32_t jSignExtention = (((ujRawImmediate & 0x80000) << 12) >> 11);
    jImmediate = jSignExtention + ((ujRawImmediate & 0xff) << 12) + ((ujRawImmediate & 0x100) << 3) + ((ujRawImmediate & 0x7fe00) >> 8);

    return {iImmediate, sImmediate, bImmediate, uImmediate, jImmediate};
}