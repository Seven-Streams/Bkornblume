#pragma once
#include "utils/error.hpp"
#include <concepts>
#include <format>
#include <functional>
#include <source_location>
#include <string>
#include <type_traits>
#include <utility>

namespace Bkornblume {

namespace __detail {

template <typename Str>
concept string_like =
    std::same_as<Str, void> || requires(const Str &str) { static_cast<std::string>(str); };

template <typename Fn, typename... Args>
concept callable = std::invocable<Fn, Args...> && string_like<std::invoke_result_t<Fn, Args...>>;

template <typename Fn, typename... Args>
static constexpr auto invoke_callable(Fn &&fn, Args &&...args) -> std::string {
    if constexpr (std::same_as<std::invoke_result_t<Fn, Args...>, void>) {
        std::forward<Fn>(fn)(std::forward<Args>(args)...);
        return std::string{};
    } else {
        return static_cast<std::string>(std::forward<Fn>(fn)(std::forward<Args>(args)...));
    }
}

template <typename _Tp>
concept condition = requires(_Tp &&cond) { static_cast<bool>(cond); };

inline constexpr auto panic_message = "Internal Error";

} // namespace __detail

template <typename Tag>
struct assumption_message_trait {
    static constexpr auto value = __detail::panic_message;
};

template <typename Tag>
inline constexpr auto assumption_message = assumption_message_trait<Tag>::value;

struct panic_handler {
    [[noreturn]]
    // a function that will be called when the program panicks
    static auto trap(const std::string &, std::source_location, const char *) -> void;
    using Hook_t = std::function<decltype(trap)>;

    static auto add_hook(Hook_t) -> void;
    static auto pop_hook(void) -> Hook_t;
};

// An RAII guard
struct [[nodiscard]] panic_injector {
    panic_injector(panic_handler::Hook_t hook) {
        panic_handler::add_hook(std::move(hook));
    }
    ~panic_injector() {
        panic_handler::pop_hook();
    }
};

namespace __detail {

// this function is for internal use only
template <typename... Args>
[[noreturn, gnu::cold]]
inline auto panic_fmt(
    std::format_string<Args...> fmt, Args &&...args,           //
    const char *detail       = __detail::panic_message,        //
    std::source_location loc = std::source_location::current() //
) -> void {
    panic_handler::trap(std::format(fmt, std::forward<Args>(args)...), loc, detail);
}

} // namespace __detail

// this function is for public use
[[noreturn, gnu::cold]]
inline auto panic(
    const std::string &msg   = "",                             //
    const char *detail       = __detail::panic_message,        //
    std::source_location loc = std::source_location::current() //
) -> void {
    panic_handler::trap(msg, loc, detail);
}

template <typename Tag = void, typename... Args>
struct assume {
private:
    using Src_t = std::source_location;

public:
    template <__detail::condition Cond>
    [[gnu::always_inline]]
    constexpr assume(
        Cond &&cond, std::format_string<Args...> fmt, Args &&...args,
        Src_t loc = Src_t::current() // need c++20
    ) {
        if (cond) [[likely]]
            return;
        using __detail::panic_fmt;
        panic_fmt<Args...>(fmt, std::forward<Args>(args)..., assumption_message<Tag>, loc);
    }

    template <__detail::callable<Args...> Fn, __detail::condition Cond>
    [[gnu::always_inline]]
    constexpr assume(Cond &&cond, Fn &&fn, Args &&...args, Src_t loc = Src_t::current()) {
        if (cond) [[likely]]
            return;
        using __detail::invoke_callable;
        panic(
            invoke_callable(std::forward<Fn>(fn), std::forward<Args>(args)...),
            assumption_message<Tag>, loc
        );
    }

    template <__detail::condition Cond>
    [[gnu::always_inline]]
    constexpr assume(Cond &&cond, Src_t loc = Src_t::current())
        requires(sizeof...(Args) == 0)
    {
        if (cond) [[likely]]
            return;
        panic(loc.function_name(), assumption_message<Tag>, loc);
    }
};

template <typename Cond>
assume(Cond &&) -> assume<void>;

template <typename Cond, typename... Args, __detail::callable<Args...> Fn>
assume(Cond &&, Fn &&, Args &&...) -> assume<void, Args...>;

template <typename Cond, typename... Args>
assume(Cond &&, std::format_string<Args...>, Args &&...) -> assume<void, Args...>;

#define MAKE_CHECK(Typename, Message)                                                              \
    namespace __detail {                                                                           \
    struct Typename##Tag {};                                                                       \
    }                                                                                              \
    template <>                                                                                    \
    struct assumption_message_trait<__detail::Typename##Tag> {                                     \
        static constexpr auto value = Message;                                                     \
    };                                                                                             \
    template <typename... Args>                                                                    \
    struct Typename : public assume<__detail::Typename##Tag, Args...> {                            \
        using assume<__detail::Typename##Tag, Args...>::assume;                                    \
    };                                                                                             \
    template <typename Cond>                                                                       \
    Typename(Cond &&) -> Typename<>;                                                               \
    template <typename Cond, typename... Args, __detail::callable<Args...> Fn>                     \
    Typename(Cond &&, Fn &&, Args &&...) -> Typename<Args...>;                                     \
    template <typename Cond, typename... Args>                                                     \
    Typename(Cond &&, std::format_string<Args...>, Args &&...) -> Typename<Args...>;

struct PanicError : public ErrorBase {
    using ErrorBase::ErrorBase;
};

} // namespace Bkornblume
