#include <vector>
#include <bitset>
#include <memory>
#include <utility>

// Represents the Result of a function that may or may not be valid
template <typename T>
using Result = std::pair<bool, T>;

// An Instruction is 32 bits wide.
using Instruction = std::bitset<32>;

// A Program is a vector of Instructions
using Program = std::vector<Instruction>;

// Smart pointers
template <typename T>
using u_ptr = std::unique_ptr<T>;

template <typename T>
using s_ptr = std::shared_ptr<T>;