#include "Utility.h"

#include "cxxopts.hpp"

#include <filesystem>

namespace fs = std::filesystem;

// Stores the results of parsing the command line arguments
struct CommandLineOptions {
    fs::path programPath;
    bool isMachineCode;
};

class CommandLineArgumentParser {
public:
    CommandLineArgumentParser(int argc, char** argv);
    CommandLineOptions parse();
    
private:
    int m_argc;
    char** m_argv;
    cxxopts::Options m_options;
};
