#include "CommandLineArgumentParser.h"
#include "Utility.h"

#include "cxxopts.hpp"

#include <string>
#include <filesystem>
#

namespace fs = std::filesystem;

CommandLineArgumentParser::CommandLineArgumentParser(int argc, char** argv) : m_argc(argc), m_argv(argv), m_options(m_argv[0]) {
    m_options.add_options()
        ("f,file", "File", cxxopts::value<std::string>())
        ("b,binary", "Machine Code", cxxopts::value<bool>()->default_value(false))
        ("h,help", "Help", cxxopts::value<bool>()->default_value(false));
}

// Parses the provided command line arguments and returns a CommandLineOptions struct that contains the results of the parse.
CommandLineOptions CommandLineArgumentParser::parse() {
    auto result = m_options.parse(m_argc, m_argv);

    fs::path filePath = result["file"].as<std::string>();
    bool isMachineCode = result["binary"].as<bool>();
    bool isHelp = result["help"].as<bool>();

    return {filePath, isMachineCode, isHelp};
}

std::string CommandLineArgumentParser::getHelpString() const {
    return m_options.help();
}