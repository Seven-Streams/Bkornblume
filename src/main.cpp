#include "utils/error.hpp"
#include <iostream>

int main() {
    std::string res;
    try {
        while (std::cin >> res) {
            // TODO:Finish the parser.
        }
    } catch (Bkornblume::ErrorBase &e) {
        std::cout << e.what() << std::endl;
        return -1;
    }
    return 0;
}
