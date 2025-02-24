#pragma once
#include <stdexcept>

namespace Bkornblume {

struct ErrorBase : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

} // namespace Bkornblume
