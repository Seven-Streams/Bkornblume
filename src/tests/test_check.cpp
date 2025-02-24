#include "test.hpp"
#include "utils/check.hpp"
#include "utils/error.hpp"
#include "utils/panic.hpp"
#include <iostream>
#include <stdexcept>
#include <string_view>

namespace Bkornblume {

namespace {

struct UserError : ErrorBase {
    using ErrorBase::ErrorBase;
};

REGISTER_TEST(UserCheckTest) {
    UserCheck(true);
    const auto guard =
        panic_injector{[](const std::string &, std::source_location, const char *detail) {
            if (detail == std::string_view{"User Check Failed"})
                throw UserError{"User Check Failed"};
        }};

    try {
        UserCheck(false);
    } catch (const UserError &e) { //
        std::cerr << "As expected: " << e.what() << std::endl;
        return;
    }

    throw std::runtime_error{"Unexpected"};
}

} // namespace

} // namespace Bkornblume
