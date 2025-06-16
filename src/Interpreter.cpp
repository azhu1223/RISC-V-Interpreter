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

    std::vector<DecodedInstruction> decodedInstructions;
    std::vector<DecodedImmediate> decodedImmediates;

    processInstructions(tp, id, program, decodedInstructions, decodedImmediates);

    return std::move(rt);
}

void Interpreter::processInstructions(ThreadPool& tp, const InstructionDecoder& id, const Program& program,
    std::vector<DecodedInstruction>& decodedInstructions, std::vector<DecodedImmediate>& decodedImmediates) const {
    int programSize = program.size();
    int maxConcurrency = std::thread::hardware_concurrency();
    int nJobs = programSize > maxConcurrency ? maxConcurrency : programSize;

    int jobSize = programSize / nJobs;
    int extraJobs = programSize % nJobs;

    InstructionDecoder id;

    auto programEnd = program.end();
    auto instructionIt = decodedInstructions.begin();
    auto immediateIt = decodedImmediates.begin();

    for (auto it = program.begin(); it != programEnd;) {
        int currentJobSize = jobSize;
        if (extraJobs > 0) {
            currentJobSize++;
            extraJobs--;
        }

        auto endIt = it + currentJobSize;

        tp.queueJob([&, this] () {
            this->instructionThreadWork(id, it, endIt, instructionIt);
        });

        tp.queueJob([&, this] () {
            this->immediateThreadWork(id, it, endIt, immediateIt);
        });

        it = endIt;
        instructionIt += currentJobSize;
        immediateIt += currentJobSize;
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