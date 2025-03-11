#include "Utility.h"

#include "cxxopts.hpp"

#include <filesystem>

namespace fs = std::filesystem;

class CommandLineArgumentParser {
public:
    CommandLineArgumentParser(int argc, char** argv);
    CommandLineOptions parse();
    
private:
    int m_argc;
    char** m_argv;
    cxxopts::Options m_options;
};

struct CommandLineOptions {
    fs::path programPath;
    bool isMachineCode;
};
