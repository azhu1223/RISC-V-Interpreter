#include "Interpreter.h"
#include "FileSystemHandler.h"
#include "BinaryFileSystemHandler.h"
#include "CommandLineArgumentParser.h"

int main(int argc, char** argv) {
    CommandLineArgumentParser commandLineArgumentParser(argc, argv);
    CommandLineOptions arguments = commandLineArgumentParser.parse();

    
}
