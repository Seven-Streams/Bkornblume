#include "error.hpp"
#include "program.hpp"
#include <iostream>

int main() {
  Bkornblume::Program program;
  try {
    program.Parser();
  } catch (Bkornblume::MyError &e) {
    std::cout << e.GetErrorMsg() << std::endl;
    return -1;
  }
  return 0;
}