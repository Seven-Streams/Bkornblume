#include "test.hpp"
#include "utils/panic.hpp"
#include <format>
#include <iostream>
#include <stdexcept>
#include <string_view>

namespace Bkornblume {

namespace {

REGISTER_TEST(test_panic) {
    const auto guard =
        panic_injector{[](const std::string &, std::source_location, const char *detail) {
            if (std::string_view{detail} != "Internal Error")
                throw std::runtime_error{"Unexpected panic"};
        }};

    auto panic_test = [&](auto &&f) {
        try {
            f();
        } catch (PanicError &e) {
            std::cerr << "- As expected, exception caught: " << e.what() << std::endl;
            return;
        }
        throw std::runtime_error{"Expected error not thrown"};
    };

    panic_test([] { panic("This is a panic message"); });
    panic_test([] { assume(false, "This is an assumption"); });
    panic_test([] { assume(1 + 2 == 4, "{} + {} != {}", 1, 2, 4); });
    panic_test([] {
        int x = 10;
        assume(x < 0, [](int x) { return std::format("x = {}", x); }, x);
    });
}

} // namespace

} // namespace Bkornblume
