#pragma once
#include <cstddef>
#include <functional>
#include <source_location>
#include <stdexcept>
#include <string>

enum class test_config : std::size_t {
    NO_STDOUT   = 1 << 0,
    NO_STDERR   = 1 << 1,
    NO_TEST_OUT = 1 << 2,
    NO_TEST_ERR = 1 << 3,

    // some aliases
    NO_CONFIG  = 0,
    NO_OUTPUT  = NO_STDOUT | NO_STDERR,
    ERROR_ONLY = NO_STDOUT | NO_TEST_OUT,
};

// union
inline constexpr auto operator|(test_config x, test_config y) noexcept -> test_config {
    return static_cast<test_config>(static_cast<std::size_t>(x) | static_cast<std::size_t>(y));
}

// intersection
inline constexpr auto operator&(test_config x, test_config y) noexcept -> test_config {
    return static_cast<test_config>(static_cast<std::size_t>(x) & static_cast<std::size_t>(y));
}

// symmetric difference
inline constexpr auto operator^(test_config x, test_config y) noexcept -> test_config {
    return static_cast<test_config>(static_cast<std::size_t>(x) ^ static_cast<std::size_t>(y));
}

// complement
inline constexpr auto operator~(test_config x) noexcept -> test_config {
    return static_cast<test_config>(~static_cast<std::size_t>(x));
}

// difference
inline constexpr auto operator/(test_config x, test_config y) noexcept -> test_config {
    return x & ~y;
}

// addition
inline constexpr auto operator+(test_config x, test_config y) -> test_config {
    if ((x & y) != test_config{})
        throw std::invalid_argument{"Cannot add configurations with overlapping flags"};
    return x | y;
}

// subtraction
inline constexpr auto operator-(test_config x, test_config y) -> test_config {
    if ((x & y) != y)
        throw std::invalid_argument{"Cannot subtract configurations with non-overlapping flags"};
    return x / y;
}

// equality
inline constexpr auto operator==(test_config x, test_config y) noexcept -> bool {
    return static_cast<std::size_t>(x) == static_cast<std::size_t>(y);
}

// comparison (order)
inline constexpr auto operator<=>(test_config x, test_config y) noexcept -> std::partial_ordering {
    if (x == y)
        return std::partial_ordering::equivalent;
    if ((x & y) == x) // x is a subset of y
        return std::partial_ordering::less;
    if ((x & y) == y) // y is a subset of x
        return std::partial_ordering::greater;
    // otherwise, they are unordered
    return std::partial_ordering::unordered;
}

struct unit_test {
    using Fn_t  = std::function<void()>;
    using Loc_t = std::source_location;

    explicit unit_test(std::string, Fn_t, Loc_t = Loc_t::current());
    static auto run_all(test_config = test_config::NO_STDOUT) noexcept -> void;

    static auto ostream() noexcept -> std::ostream &;
    static auto estream() noexcept -> std::ostream &;
};

#define REGISTER_TEST(name)                                                                        \
    static auto _test_func_##name()->void;                                                         \
    static auto _test_##name = unit_test{#name, _test_func_##name};                                \
    static auto _test_func_##name()->void
