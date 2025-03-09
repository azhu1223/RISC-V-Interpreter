#include "Utility.h"

#include "cxxopts.hpp"

class CommandLineArgumentParser {
public:
    CommandLineArgumentParser(int argc, char** argv);

private:
    int m_argc;
    char** m_argv;
};
