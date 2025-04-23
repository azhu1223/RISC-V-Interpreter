#ifndef COMMAND_LINE_ARGUMENTS_PARSER_H
#define COMMAND_LINE_ARGUMENTS_PARSER_H

#include "Utility.h"

#include "cxxopts.hpp"

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

// Stores the results of parsing the command line arguments
struct CommandLineOptions {
    std::string exceptionMessage;
    fs::path programPath;
    bool isMachineCode;
    bool isHelp;
};

class CommandLineArgumentParser {
public:
    CommandLineArgumentParser(int argc, char** argv);
    Result<CommandLineOptions> parse();
    std::string getHelpString() const;
    
private:
    int m_argc;
    char** m_argv;
    std::string m_helpString;
    cxxopts::Options m_options;
};

#endif
