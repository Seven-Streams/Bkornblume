#include "../include/program.hpp"
#include "../include/utils/error.hpp"
#include <iostream>

int main() {
    Bkornblume::Program program;
    try {
        program.Parser("");
    } catch (const Bkornblume::ErrorBase &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    return 0;
}
