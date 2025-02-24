#pragma once
#include "variable.hpp"
#include <stack>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
namespace Bkornblume {
class Program {
private:
    std::stack<std::unordered_map<std::string, std::variant<int *, ArrayNode *>>> variable_stack;
    std::stack<std::string> function_stack;
    std::unordered_map<std::string, std::variant<int *, ArrayNode *>> global_variable;
    std::unordered_map<std::string, std::vector<std::string>> structs_member;
    std::vector<std::string> original_code;
    std::unordered_map<std::string, std::unordered_map<std::string, int>> labels;
    std::unordered_map<std::string, int> function_entries;

public:
    Program()  = default;
    ~Program(){
        for(auto i: global_variable) {
            if(std::holds_alternative<ArrayNode *>(i.second)) {
                delete std::get<ArrayNode *>(i.second);
            } else {
                delete std::get<int *>(i.second);
            }
        }
        while(!variable_stack.empty()) {
            for(auto i: variable_stack.top()) {
                if(std::holds_alternative<ArrayNode *>(i.second)) {
                    delete std::get<ArrayNode *>(i.second);
                } else {
                    delete std::get<int *>(i.second);
                }
            }
            variable_stack.pop();
        }
    }
    void Parser(std::string path);
    void Run();
};
} // namespace Bkornblume
