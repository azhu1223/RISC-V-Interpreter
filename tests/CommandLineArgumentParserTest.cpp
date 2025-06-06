#include <gtest/gtest.h>

#include "CommandLineArgumentParser.h"
#include "Utility.h"

#include <string>

class CommandLineArgumentParserTest : public ::testing::Test {
protected:
    std::string m_pathMarker = "-f";
    std::string m_binaryMarker = "-b";
    std::string m_helpMarker = "-h";
    std::string m_path = "path/file";
    std::string m_programName = "program";
};

TEST_F(CommandLineArgumentParserTest, ValidArguments) {
    int argc = 5;
    char* argv[5] = {m_programName.data(), m_pathMarker.data(), m_path.data(), m_binaryMarker.data(), m_helpMarker.data()};

    CommandLineArgumentParser parser(argc, argv);

    Result<CommandLineOptions> result = parser.parse();

    EXPECT_TRUE(result.first);

    CommandLineOptions options = result.second;

    EXPECT_TRUE(options.isMachineCode);
    EXPECT_TRUE(options.isHelp);
    EXPECT_EQ(options.programPath, m_path);
}

TEST_F(CommandLineArgumentParserTest, HelpString) {
    int argc = 5;
    char* argv[5] = {m_programName.data(), m_pathMarker.data(), m_path.data(), m_binaryMarker.data(), m_helpMarker.data()};

    CommandLineArgumentParser parser(argc, argv);
    std::string helpString = parser.getHelpString();

    const std::string expectedString = "\nUsage:\n  program [OPTION...]\n\n  -f, --file arg  File Path\n  -b, --binary    Is Machine Code\n  -h, --help      Help\n";

    EXPECT_EQ(helpString, expectedString);
}

TEST_F(CommandLineArgumentParserTest, MissingPath) {
    char * argv[] = {m_programName.data()};
    CommandLineArgumentParser parser(1, argv);

    Result<CommandLineOptions> result;

    result = parser.parse();

    EXPECT_FALSE(result.first);
    EXPECT_EQ(result.second.error, ParserErrorCode::MISSING_PATH_OPTION);
}

TEST_F(CommandLineArgumentParserTest, MissingPathOptionArgument) {
    char * argv[] = {m_programName.data(), m_pathMarker.data()};
    CommandLineArgumentParser parser(2, argv);

    Result<CommandLineOptions> result;

    result = parser.parse();

    EXPECT_FALSE(result.first);
    EXPECT_EQ(result.second.error, ParserErrorCode::MISSING_PATH_ARGUMENT);
}

TEST_F(CommandLineArgumentParserTest, UnrecognizedOption) {
    std::string unrecognizedArg = "fkfkfkfk";
    char * argv[] = {m_programName.data(), unrecognizedArg.data()};

    CommandLineArgumentParser parser(2, argv);

    Result<CommandLineOptions> result;

    result = parser.parse();

    EXPECT_FALSE(result.first);
    EXPECT_EQ(result.second.error, ParserErrorCode::UNRECOGNIZED_OPTION);
}
