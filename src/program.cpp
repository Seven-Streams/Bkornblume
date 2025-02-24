#include "../include/program.hpp"
#include "../include/utils/error.hpp"
#include <fstream>
void Bkornblume::Program::Parser(std::string path) {
    int cnt = 0;
    std::fstream file(path);
    original_code.clear();
    global_variable.clear();
    structs_member.clear();
    function_args.clear();
    labels.clear();

    if (!file.is_open()) {
        throw UserError("File not found");
    }
    while(!file.eof()) {
        std::string line;
        std::getline(file, line);
        original_code.push_back(line);
    }
}