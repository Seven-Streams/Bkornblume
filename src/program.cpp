#include "program.hpp"
#include "utils/error.hpp"
#include "utils/tokens.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <variant>

namespace Bkornblume {

void Program::Parser(std::string path) {
    // int cnt = 0;
    std::fstream file(path);
    original_code.clear();
    global_variable.clear();
    structs_member.clear();
    function_entries.clear();
    labels.clear();

    if (!file.is_open()) {
        throw UserError("File not found");
    }
    while (!file.eof()) {
        std::string line;
        std::getline(file, line);
        original_code.push_back(line);
    }
    std::string now_func = "";
    for (size_t i = 0; i < original_code.size(); i++) {
        auto tokens = Slice(original_code[i]);
        /*
            This part is to check tag the entry of the function.
        */
        if (tokens.front() == "define") {
            std::string function_name       = tokens[1].find('(') == std::string::npos
                                                ? tokens[1]
                                                : tokens[1].substr(0, tokens[1].find('('));
            function_entries[function_name] = i;
            now_func                        = function_name;
            labels[function_name]           = std::unordered_map<std::string, int>();
        }

        /*
            This part is to check the end of the function.
        */
        if (original_code[i][original_code.size() - 1] == '}') {
            now_func = "";
        }

        /*
            This part is to check the label.
        */
        if (original_code[i][original_code.size() - 1] == ':') {
            labels[now_func][tokens[0].substr(0, tokens[0].size() - 1)] = i;
        }

        /*
            This part is to check the global variable.
        */
        if (original_code[i][0] == '@') {
            for (size_t i = 0; i < tokens.size(); i++) {
                if (tokens[i].find("global") != std::string::npos) {
                    if (tokens[i + 1] == "i1") {
                        if (tokens[i + 2] == "true") {
                            global_variable[tokens[i]] = new int(1);
                        } else {
                            global_variable[tokens[i]] = new int(0);
                        }
                    } else {
                        if (tokens[i + 1] == "i32" || tokens[i + 1] == "i8") {
                            if ((i + 2) < tokens.size()) {
                                global_variable[tokens[i]] = new int(std::stoi(tokens[i + 2]));
                            } else {
                                global_variable[tokens[i]] = new int(std::rand());
                                std::cout << "Warning: the value of " << tokens[i]
                                          << " is not initialized" << std::endl;
                            }
                        } else {
                            global_variable[tokens[i]] = new ArrayNode();
                        }
                    }
                }
            }
        }
    }
    file.close();
}

} // namespace Bkornblume
