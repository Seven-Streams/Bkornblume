#include "test.hpp"
#include <format>
#include <iostream>
#include <mutex>
#include <source_location>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

namespace {

struct Impl {
    using testcase = std::tuple<unit_test::Fn_t, std::string, std::source_location>;
    std::mutex mutex;
    std::vector<testcase> tests;
};

auto get_structure() -> Impl & {
    static Impl impl;
    return impl;
}

struct stream_guard {
public:
    stream_guard(std::ostream *os, std::streambuf *buf, std::ostream &err) : err{err} {
        this->ptr = os;
        if (os != nullptr) {
            this->old = os->rdbuf(buf);
            this->cur = buf;
            os->rdbuf(buf);
        }
    }
    ~stream_guard() {
        if (ptr != nullptr && cur != ptr->rdbuf(old))
            err << "\033[1;33mWarning\033[0m: streambuf mismatch!\n";
    }

private:
    std::ostream *ptr;
    std::streambuf *old;
    std::streambuf *cur;
    std::ostream &err;
};

auto global_out = std::ostream{std::cout.rdbuf()};
auto global_err = std::ostream{std::cerr.rdbuf()};

} // namespace

unit_test::unit_test(std::string name, Fn_t fn, std::source_location loc) {
    auto &impl = get_structure();
    auto guard = std::lock_guard{impl.mutex};
    impl.tests.push_back({std::move(fn), std::move(name), loc});
}

auto unit_test::run_all(test_config cfg) noexcept -> void {
    const auto tests = [] { // move tests out of the structure
        auto &impl = get_structure();
        auto guard = std::lock_guard{impl.mutex};
        return std::move(impl.tests);
    }();

    static std::mutex mutex;

    auto guard = std::lock_guard{mutex};
    using enum test_config;

    auto tout = std::ostream{NO_TEST_OUT <= cfg ? nullptr : std::cout.rdbuf()};
    auto terr = std::ostream{NO_TEST_OUT <= cfg ? nullptr : std::cerr.rdbuf()};

    auto cout_guard = stream_guard{NO_STDOUT <= cfg ? &std::cout : nullptr, {}, terr};
    auto cerr_guard = stream_guard{NO_STDERR <= cfg ? &std::cerr : nullptr, {}, terr};

    auto failing_reason = std::string{};
    for (const auto &[fn, name, loc] : tests) {
        try {
            fn();
            tout << std::format(
                "\033[1;32mPASSED\033[0m: {} ({}:{})\n", name, loc.file_name(), loc.line()
            );
            continue; // pass the test
        } catch (const std::exception &e) {
            failing_reason = e.what(); //
        } catch (...) {
            failing_reason = "unknown"; //
        }
        terr << std::format(
            "\033[1;31mFAILED\033[0m: {} ({}:{})\n", name, loc.file_name(), loc.line()
        );
        terr << std::format("  Reason: {}\n", failing_reason);
    }
}

auto unit_test::ostream() noexcept -> std::ostream & {
    return global_out;
}

auto unit_test::estream() noexcept -> std::ostream & {
    return global_err;
}
