#include "../include/program.hpp"
#include "../include/utils/error.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>

int main() {
    srand(time(0));
    std::string path;
    Bkornblume::Program program;
    try {
        program.Parser(std::move(path));
    } catch (const Bkornblume::ErrorBase &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    return 0;
}
