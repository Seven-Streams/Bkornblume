#pragma once
#include <stdexcept>

namespace Bkornblume {

struct ErrorBase : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

struct UserError : public ErrorBase {
    UserError(std::string info) : ErrorBase(info) {}
};
struct DebugError : public ErrorBase {
    DebugError(std::string info) : ErrorBase(info) {}
};

} // namespace Bkornblume
