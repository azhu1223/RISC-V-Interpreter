#include "CommandLineArgumentParser.h"
#include "Utility.h"

#include "cxxopts.hpp"

#include <string>
#include <filesystem>

namespace fs = std::filesystem;

CommandLineArgumentParser::CommandLineArgumentParser(int argc, char** argv) : m_argc(argc), m_argv(argv), m_options(m_argv[0]) {
    m_options.add_options()
        ("f,file", "File Path", cxxopts::value<std::string>())
        ("b,binary", "Is Machine Code")
        ("h,help", "Help");

    m_helpString = m_options.help();
}

// Parses the provided command line arguments and returns a CommandLineOptions struct that contains the results of the parse.
Result<CommandLineOptions> CommandLineArgumentParser::parse() {
    cxxopts::ParseResult result;

    try {
        result = m_options.parse(m_argc, m_argv);
    }

    catch (const cxxopts::exceptions::missing_argument& error) {
        return {false, {.exceptionMessage = error.what()}};
    }

    fs::path filePath;

    try {
        filePath = result["file"].as<std::string>();
    }
    
    catch (const cxxopts::exceptions::option_has_no_value& error) {
        return {false, {.exceptionMessage = error.what()}};
    }

    bool isMachineCode = result["binary"].as<bool>();
    bool isHelp = result["help"].as<bool>();

    return {true, {.programPath = filePath, 
                    .isMachineCode = isMachineCode, 
                    .isHelp = isHelp}};
}

std::string CommandLineArgumentParser::getHelpString() const {
    return m_helpString;
}