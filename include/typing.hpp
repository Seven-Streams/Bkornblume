#pragma once
#include "utils/panic.hpp"
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <type_traits>
#include <unordered_set>
#include <variant>
#include <vector>

namespace Bkornblume {

// User defined classes
struct ClassLayout;
// A typeinfo of any class.
struct TypeInfo;

// View of a user defined class
struct ClassView;
// Storage of a user defined class
struct ClassData;

// A view of another static storage
struct StorageView;
// The real representation of the storage
struct StorageData;

using i8   = std::int8_t;
using i16  = std::int16_t;
using i32  = std::int32_t;
using i64  = std::int64_t;
using ptr  = StorageView;
using none = std::monostate;

MAKE_CHECK(CheckUseAfterFree, "Use after free detected");
MAKE_CHECK(CheckDanglingRef, "Dangling reference detected");
MAKE_CHECK(CheckOutOfBound, "Access out of bound detected");

namespace __detail {

template <typename... Args>
using variant_unqiue_aux = std::variant<none, std::unique_ptr<Args[]>...>;

using array_variant = variant_unqiue_aux<i8, i16, i32, i64, ptr, ClassView>;
using value_variant = std::variant<none, i8, i16, i32, i64, ptr, ClassView>;
using type_variant  = std::variant<none, i8, i16, i32, i64, ptr, ClassLayout *>;

template <typename T, typename... Args>
inline consteval auto which_variant_aux(std::variant<Args...> *) -> std::size_t {
    const bool same[] = {std::is_same_v<T, Args>...};
    for (std::size_t i = 0; i < sizeof...(Args); ++i)
        if (same[i])
            return i;
    throw;
}

struct BaseView;
struct BaseData;

struct BaseView {
public:
    auto is_valid() const -> bool {
        return m_source != nullptr;
    }

protected:
    BaseView();
    BaseView(BaseData &);
    ~BaseView();

    // no copy, no move
    BaseView(const BaseView &)                = delete;
    BaseView &operator=(const BaseView &)     = delete;
    BaseView(BaseView &&) noexcept            = delete;
    BaseView &operator=(BaseView &&) noexcept = delete;

    auto m_use() -> BaseData & {
        CheckUseAfterFree(this->is_valid());
        return *m_source;
    }

private:
    friend struct BaseData;
    auto m_invalidate() -> void;

    BaseData *m_source; // if null, the storage type is dead.
};

struct BaseData {
protected:
    BaseData();
    ~BaseData();

    // no copy
    BaseData(const BaseData &)            = delete;
    BaseData &operator=(const BaseData &) = delete;

    // move is unsafe
    BaseData(BaseData &&) noexcept            = delete;
    BaseData &operator=(BaseData &&) noexcept = delete;

private:
    friend struct BaseView;
    auto m_register(BaseView *view) -> void;
    std::unordered_set<BaseView *> m_views;
};

} // namespace __detail

template <typename T>
struct TypedView : __detail::BaseView {
public:
    TypedView();
    TypedView(StorageData &, std::span<T>, std::size_t = 0);

    auto operator+(std::size_t which) -> TypedView;
    auto operator-(std::size_t which) -> TypedView;
    auto operator*(void) -> T &;
    operator StorageView();

private:
    const std::span<T> m_view;
    std::size_t m_offset;
};

// global lifespan, no need to free
struct ClassLayout {
public:
    auto is(std::string_view) -> bool;

    friend auto register_class(std::string, std::vector<TypeInfo>)
        -> std::pair<ClassLayout &, bool>;

private:
    ClassLayout(std::string, std::vector<TypeInfo>);
    std::string m_name;
    std::vector<TypeInfo> m_fields;
};

struct ClassData : __detail::BaseData {
public:
    ClassData();
    ~ClassData();

    template <typename... Args>
    friend auto make_class(ClassLayout &, Args &&...) -> ClassData;

private:
    ClassLayout &m_layout;
    std::unique_ptr<StorageData[]> m_data;
};

struct StorageData : __detail::BaseData {
public:
    StorageData();
    ~StorageData();

    template <typename T, typename... Args>
    friend auto make_storage(Args &&...) -> StorageData;

private:
    template <typename T>
    auto m_get() -> T *;

    __detail::array_variant m_data; // the actual data
    std::size_t m_size;             // length of array

    template <typename T>
    struct tag {};

    template <typename T>
    StorageData(tag<T>, std::unique_ptr<std::remove_reference_t<T>[]>, std::size_t);

    template <typename T, typename... Args>
    friend auto make_storage(Args &&...) -> StorageData;
};

struct ClassView : __detail::BaseView {
    ClassView();
    ClassView(ClassData &src);

    // Return n-th member
    template <typename T>
    auto get(std::size_t) -> TypedView<T>;
};

struct StorageView : __detail::BaseView {
    StorageView();
    StorageView(StorageData &);

    template <typename T>
    auto get(std::size_t) -> TypedView<T>;
    // Return n-th class
    auto get(std::string_view, std::size_t) -> TypedView<ClassView>;
};

struct TypeInfo {
public:
    ~TypeInfo();

    template <typename T>
    friend auto make_type(std::size_t) -> TypeInfo;
    friend auto make_type(ClassLayout &, std::size_t) -> TypeInfo;

private:
    TypeInfo(__detail::type_variant, std::size_t);
    __detail::type_variant m_type;
    std::size_t m_size;
};

namespace __detail {

namespace __typing_check {

template <typename T>
inline consteval auto typeid_of_0() -> std::size_t {
    array_variant *v = nullptr;
    return which_variant_aux<std::unique_ptr<T[]>>(v);
}

template <typename T>
inline consteval auto typeid_of_1() -> std::size_t {
    type_variant *v = nullptr;
    return which_variant_aux<T>(v);
}

static_assert(typeid_of_0<i8>() == typeid_of_1<i8>());
static_assert(typeid_of_0<i16>() == typeid_of_1<i16>());
static_assert(typeid_of_0<i32>() == typeid_of_1<i32>());
static_assert(typeid_of_0<i64>() == typeid_of_1<i64>());
static_assert(typeid_of_0<ptr>() == typeid_of_1<ptr>());
static_assert(typeid_of_0<ClassView>() == typeid_of_1<ClassLayout *>());

} // namespace __typing_check

} // namespace __detail

struct ScalarValue {
public:
    auto as_int() -> std::optional<i64>;
    auto as_ptr() -> std::optional<ptr>;
    auto as_cls(std::string_view) -> std::optional<ClassView>;

private:
    __detail::value_variant m_value;
};

} // namespace Bkornblume
