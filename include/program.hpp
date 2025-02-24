#pragma once
#include "instructions.hpp"
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>
#include <variant>
namespace Bkornblume {
class Program {
private:
    std::stack<std::unordered_map<std::string, std::variant<int, void*>>> variable_stack;
    std::stack<std::string> function_stack;
    std::unordered_map<std::string, std::variant<int, void*>> global_variable;
    std::unordered_map<std::string, std::vector<std::string>> structs_member;
    std::unordered_map<std::string, std::vector<std::string>> function_args;
    std::vector<std::string> original_code;
    std::vector<Instruction> instructions;
    std::unordered_map<std::string, int> labels;
public:
    Program() = default;
    ~Program() = default;
    void Parser(std::string path);
    void Run();
};
}
