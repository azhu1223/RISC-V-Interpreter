#include "FileSystemHandler.h"
#include "Utility.h"

class BinaryFileSystemHandler : public FileSystemHandler {
public:
    virtual Result<u_ptr<Program>> readProgram() const;
private:
};