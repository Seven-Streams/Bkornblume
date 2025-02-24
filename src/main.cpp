#include "error.hpp"
#include <iostream>

int main() {
  std::string res;
  try {
    while (std::cin >> res) {
      //TODO:Finish the parser.
    }
  } catch (Bkornblume::MyError &e) {
    std::cout << e.GetErrorMsg() << std::endl;
    return -1;
  }
  return 0;
}