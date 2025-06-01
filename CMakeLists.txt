cmake_minimum_required(VERSION 3.30.3)
project(risc5interpreter)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)

# Make sure developers do not run cmake in the main project directory, to keep
# build artifacts from becoming clutter
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
message(FATAL_ERROR "In-source builds not allowed.
Please make a new directory (called a build directory) and run
CMake from there.You may need to remove CMakeCache.txt." )
endif()

# Check if the build type is set for coverage
if(CMAKE_BUILD_TYPE STREQUAL "Coverage")
    message(STATUS "Configuring build for coverage")
    # Add flags for GCC to generate coverage data
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

include(GoogleTest)

FetchContent_Declare(
  cxxopts
  GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
  GIT_TAG v3.2.1
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest cxxopts)

enable_testing()

include_directories(include)
include_directories(libraries)

# Libraries

# add_library(Interpreter_lib src/Interpreter.cpp)
add_library(BinaryFileSystemHandler_lib src/BinaryFileSystemHandler.cpp)
add_library(CommandLineArgumentParser_lib src/CommandLineArgumentParser.cpp)
add_library(ThreadPool_lib src/ThreadPool.cpp)
add_library(InstructionDecoder_lib src/InstructionDecoder.cpp)

add_executable(interpreter src/main.cpp)
target_link_libraries(
  interpreter
  BinaryFileSystemHandler_lib
  CommandLineArgumentParser_lib
)

add_executable(CommandLineArgumentParser_test tests/CommandLineArgumentParserTest.cpp)
target_link_libraries(
  CommandLineArgumentParser_test
  CommandLineArgumentParser_lib
  GTest::gtest_main
)

add_executable(ThreadPool_test tests/ThreadPoolTest.cpp)
target_link_libraries(
  ThreadPool_test
  ThreadPool_lib
  GTest::gtest_main
)

add_executable(InstructionDecoder_test tests/InstructionDecoderTest.cpp)
target_link_libraries(
  InstructionDecoder_test
  InstructionDecoder_lib
  gtest_main
)

gtest_discover_tests(CommandLineArgumentParser_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(ThreadPool_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(InstructionDecoder_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
