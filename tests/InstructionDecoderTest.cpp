#include <gtest/gtest.h>

#include "InstructionDecoder.h"
#include "Utility.h"

#include <vector>

class InstructionDecoderTest : public ::testing::Test {
protected:
    Program program;
    InstructionDecoder id;
    virtual void SetUp() override {
        program.emplace_back(0x00100093); // addi x1, x0, 1
        program.emplace_back(0x00416333); // or x6, x2, x4
        program.emplace_back(0xff608113); // addi x2, x1, -10
        program.emplace_back(0x474ff0ef); // jal x1, 1045620
        program.emplace_back(0x7aa39563); // bne x7, x10, 1962
    }
};

#include <sstream>
#include <iostream>

TEST_F(InstructionDecoderTest, DecodeInstruction) {
    std::vector<DecodedInstruction> decodedInstructions;

    std::stringstream ss;
    ss << std::hex;

    for (const Instruction& i : program) {
        ss << i.to_ulong() << '\n';
        decodedInstructions.push_back(id.decodeInstruction(i));
    }

    ss << '\n';

    std::cout << ss.str();
    
    // First instruction
    DecodedInstruction& di = decodedInstructions[0];

    // Check correct instruction
    EXPECT_EQ(di.opcode, 0b0010011);
    EXPECT_EQ(di.funct3, 0b000);

    // Check correct registers
    EXPECT_EQ(di.rd, 0b00001);
    EXPECT_EQ(di.rs1, 0b00000);

    // Check type
    EXPECT_EQ(di.type, InstructionType::I);

    // Second instruction
    di = decodedInstructions[1];

    // Check correct instruction
    EXPECT_EQ(di.opcode, 0b0110011);
    EXPECT_EQ(di.funct3, 0b110);

    // Check correct registers
    EXPECT_EQ(di.rd, 0b00110);
    EXPECT_EQ(di.rs1, 0b00010);
    EXPECT_EQ(di.rs2, 0b00100);

    // Check type
    EXPECT_EQ(di.type, InstructionType::R);

    // Third instruction
    di = decodedInstructions[2];

    // Check correct instruction
    EXPECT_EQ(di.opcode, 0b0010011);
    EXPECT_EQ(di.funct3, 0b000);

    // Check correct registers
    EXPECT_EQ(di.rd, 0b00010);
    EXPECT_EQ(di.rs1, 0b00001);

    // Check type
    EXPECT_EQ(di.type, InstructionType::I);

    // Fourth instruction
    di = decodedInstructions[3];

    // Check correct instruction
    EXPECT_EQ(di.opcode, 0b1101111);

    // Check correct register
    EXPECT_EQ(di.rd, 0b00001);

    // Check type
    EXPECT_EQ(di.type, InstructionType::J);

    // Fifth instruction
    di = decodedInstructions[4];

    // Check correct instruction
    EXPECT_EQ(di.opcode, 0b1100011);
    EXPECT_EQ(di.funct3, 0b001);

    // Check correct registers
    EXPECT_EQ(di.rs1, 0b00111);
    EXPECT_EQ(di.rs2, 0b01010);

    // Check type
    EXPECT_EQ(di.type, InstructionType::B);
}

TEST_F(InstructionDecoderTest, GenerateImmediate) {
    std::vector<DecodedImmediate> extendedImmediates;
    for (const Instruction& i : program) {
        extendedImmediates.push_back(id.generateImmediate(i));
    }

    // First instruction
    DecodedImmediate& di = extendedImmediates[0];
    EXPECT_EQ(di.iImmediate, 1);

    // Third instruction
    di = extendedImmediates[2];
    EXPECT_EQ(di.iImmediate, -10);

    // Fourth instruction
    di = extendedImmediates[3];
    EXPECT_EQ(di.jImmediate, 1045620);

    // Fifth instruction
    // Fourth instruction
    di = extendedImmediates[4];
    EXPECT_EQ(di.bImmediate, 1962);
}
