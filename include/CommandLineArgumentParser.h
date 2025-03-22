#include "Utility.h"

#include "cxxopts.hpp"

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

// Stores the results of parsing the command line arguments
struct CommandLineOptions {
    fs::path programPath;
    bool isMachineCode;
    bool isHelp;
};

class CommandLineArgumentParser {
public:
    CommandLineArgumentParser(int argc, char** argv);
    CommandLineOptions parse();
    std::string getHelpString() const;
    
private:
    int m_argc;
    char** m_argv;
    cxxopts::Options m_options;
};
