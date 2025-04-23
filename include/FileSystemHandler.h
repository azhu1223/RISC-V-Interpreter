#ifndef FILE_SYSTEM_HANDLER
#define FILE_SYSTEM_HANDLER

#include "Utility.h"

#include <filesystem>
#include <bitset>

namespace fs = std::filesystem;

class FileSystemHandler {
public:
    virtual Result<u_ptr<Program>> readProgram() const = 0;
    bool setPath(const fs::path& path);
    fs::path getPath() const;
private:
    fs::path m_path;
};

inline
fs::path FileSystemHandler::getPath() const {
    return m_path;
}

// Sets m_path to path if a file exists at path and it is not a directory. 
// Returns whether m_path set.
inline
bool FileSystemHandler::setPath(const fs::path& path) {
    bool validFile = false;

    if (fs::exists(path) && !fs::is_directory(path)) {
        m_path = path;
        validFile = true;
    }

    return validFile;
}

#endif
