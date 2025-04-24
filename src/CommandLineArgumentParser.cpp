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
    m_options.allow_unrecognised_options();
    m_helpString = m_options.help();
}

// Parses the provided command line arguments and returns a CommandLineOptions struct that contains the results of the parse.
Result<CommandLineOptions> CommandLineArgumentParser::parse() {
    cxxopts::ParseResult results;

    try {
        results = m_options.parse(m_argc, m_argv);
    }

    // If this exception was thrown, that means the -f/file option was provided, but an argument was not
    catch (const cxxopts::exceptions::missing_argument& error) {
        return {false, {.error = ParserErrorCode::MISSING_PATH_ARGUMENT}};
    } 

    if (results.unmatched().size() != 0) {
        return {false, {.error = ParserErrorCode::UNRECOGNIZED_OPTION}};
    }

    // If this is true, that means the -f/file option was not provided
    if (results.count("file") == 0) {
        return {false, {.error = ParserErrorCode::MISSING_PATH_OPTION}};
    }

    std::string filePath = results["file"].as<std::string>();

    bool isMachineCode = results["binary"].as<bool>();
    bool isHelp = results["help"].as<bool>();

    return {true, {.programPath = filePath, 
                    .isMachineCode = isMachineCode, 
                    .isHelp = isHelp}};
}

std::string CommandLineArgumentParser::getHelpString() const {
    return m_helpString;
}