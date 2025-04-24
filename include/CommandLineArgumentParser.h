#ifndef COMMAND_LINE_ARGUMENTS_PARSER_H
#define COMMAND_LINE_ARGUMENTS_PARSER_H

#include "Utility.h"

#include "cxxopts.hpp"

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

// Represents possible errors encountered by the parser 
enum class ParserErrorCode {
    MISSING_PATH_OPTION,
    MISSING_PATH_ARGUMENT,
    UNRECOGNIZED_OPTION
};

// Stores the results of parsing the command line arguments
struct CommandLineOptions {
    ParserErrorCode error;
    fs::path programPath;
    bool isMachineCode;
    bool isHelp;

    static std::string parserErrorCodeToString(ParserErrorCode error);
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

inline std::string CommandLineOptions::parserErrorCodeToString(ParserErrorCode error) {
    std::string errorString;

    switch (error) {
    case ParserErrorCode::MISSING_PATH_OPTION:
        errorString = "File option not provided";
        break;
    case ParserErrorCode::MISSING_PATH_ARGUMENT:
        errorString = "File path not provided";
        break;
    case ParserErrorCode::UNRECOGNIZED_OPTION:
        errorString = "Detected extra unrecognized arguments";
        break;
    }

    return errorString;
}

#endif
