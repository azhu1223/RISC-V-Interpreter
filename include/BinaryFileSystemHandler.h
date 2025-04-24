#ifndef BINARY_FILE_SYSTEM_HANDLER_H
#define BINARY_FILE_SYSTEM_HANDLER_H

#include "FileSystemHandler.h"
#include "Utility.h"

class BinaryFileSystemHandler : public FileSystemHandler {
public:
    virtual Result<u_ptr<Program>> readProgram() const;
private:
};

#endif