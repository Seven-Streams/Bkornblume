#ifndef ERROR_HPP
#define ERROR_HPP
#include <string>
namespace Bkornblume {
class MyError {
private:
  std::string *error_msg = nullptr;

public:
  MyError(std::string _msg) : error_msg(new std::string(_msg)) {}
  ~MyError() { delete error_msg; }
  std::string GetErrorMsg() { return *error_msg; }
};
} // namespace Bkornblume

#endif