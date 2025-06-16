#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Utility.h"
#include "RegisterTable.h"
#include "ThreadPool.h"
#include "InstructionDecoder.h"

#include <vector>

class Interpreter {
public:
    u_ptr<RegisterTable> interpret(const Program& program) const;
private:
    void processInstructions(ThreadPool& tp, const InstructionDecoder& id, const Program& program,
        std::vector<DecodedInstruction>& decodedInstructions, std::vector<DecodedImmediate>& decodedImmediates) const;
    void instructionThreadWork(const InstructionDecoder& id, 
        const Iterator<Program>& start, const Iterator<Program>& end, 
        Iterator<std::vector<DecodedInstruction>>& diStart) const;
    void immediateThreadWork(const InstructionDecoder& id, 
        const Iterator<Program>& start, const Iterator<Program>& end, 
        Iterator<std::vector<DecodedImmediate>>& diStart) const;
};

#endif