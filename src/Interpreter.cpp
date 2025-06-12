#include "Interpreter.h"
#include "Utility.h"
#include "RegisterTable.h"
#include "ThreadPool.h"
#include "InstructionDecoder.h"

#include <memory>
#include <thread>
#include <vector>

u_ptr<RegisterTable> Interpreter::interpret(const Program& program) const {
    u_ptr<RegisterArray> registerTableArray(new RegisterArray());
    u_ptr<RegisterTable> rt(new RegisterTable(std::move(registerTableArray)));

    ThreadPool tp;
    tp.start(std::thread::hardware_concurrency());

    InstructionDecoder id;

    processInstructions(tp, id, program);

    return std::move(rt);
}

void Interpreter::processInstructions(ThreadPool& tp, const InstructionDecoder& id, const Program& program) const {
    int programSize = program.size();
    int maxConcurrency = std::thread::hardware_concurrency();
    int nJobs = programSize > maxConcurrency ? maxConcurrency : programSize;

    int jobSize = programSize / nJobs;
    int extraJobs = programSize % nJobs;

    InstructionDecoder id;

    auto programEnd = program.end();
    for (auto it = program.begin(); it != programEnd;) {
        int currentJobSize = jobSize;
        if (extraJobs > 0) {
            currentJobSize++;
            extraJobs--;
        } 
    }
};

#include <algorithm>

void Interpreter::instructionThreadWork(const InstructionDecoder& id, 
    const Iterator<Program>& start, const Iterator<Program>& end, 
    Iterator<std::vector<DecodedInstruction>>& diStart) const {

    auto diIterator = diStart;

    std::for_each(start, end, [&] (const Instruction& i) -> void {
        *(diIterator++) = id.decodeInstruction(i);
    });
}

void Interpreter::immediateThreadWork(const InstructionDecoder& id, 
    const Iterator<Program>& start, const Iterator<Program>& end, 
    Iterator<std::vector<DecodedImmediate>>& diStart) const {

    auto diIterator = diStart;

    std::for_each(start, end, [&] (const Instruction& i) -> void {
        *(diIterator++) = id.generateImmediate(i);
    });
}