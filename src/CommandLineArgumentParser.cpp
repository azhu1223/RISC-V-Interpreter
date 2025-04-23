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
    auto result = m_options.parse(m_argc, m_argv);

    if (result.unmatched().size() != 0) {
        return {false, {}};
    }

    fs::path filePath = result["file"].as<std::string>();
    bool isMachineCode = result["binary"].as<bool>();
    bool isHelp = result["help"].as<bool>();

    return {true, {filePath, isMachineCode, isHelp}};
}

std::string CommandLineArgumentParser::getHelpString() const {
    return m_helpString;
}