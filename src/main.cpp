#include "Interpreter.h"
#include "FileSystemHandler.h"
#include "BinaryFileSystemHandler.h"
#include "CommandLineArgumentParser.h"

#include <iostream>

int main(int argc, char** argv) {
    CommandLineArgumentParser commandLineArgumentParser(argc, argv);
    Result<CommandLineOptions> argumentResults = commandLineArgumentParser.parse();

    CommandLineOptions options = argumentResults.second;

    if (options.isHelp) {
        std::cout << commandLineArgumentParser.getHelpString();

        exit(0);
    }

    if (!argumentResults.first) {
        std::cout << CommandLineOptions::parserErrorCodeToString(options.error) << '\n';

        exit(1);
    }
}
