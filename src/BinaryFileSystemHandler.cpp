#include "BinaryFileSystemHandler.h"
#include "Utility.h"

#include <fstream>
#include <memory>

Result<u_ptr<Program>> BinaryFileSystemHandler::readProgram() const {
    std::ifstream binaryFile(getPath(), std::ios::binary);

    // File failed to open or some other error occurred.
    if (!binaryFile.good()) {
        return {false, nullptr};
    }

    u_ptr<Program> program(new Program);
    while(!binaryFile.eof()) {
        char buf[4];

        // Read 4 bytes from binaryFile into buf
        binaryFile.read(buf, 4);

        program->emplace_back(buf);
    }

    // The last instruction read is not a valid length (i.e. too short).
    // Therefore, the file does not contain a valid program.
    // Empty program is a valid program.
    if (binaryFile.gcount() != 4 && !program->empty()) {
        return {false, nullptr};
    }

    return {true, std::move(program)};
}
