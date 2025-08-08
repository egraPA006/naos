#pragma once
#include "klib/error.hpp"
#include "klib/panic.hpp"
#include "klib/printk.hpp"

namespace klib {

template <typename T> class Result {
    union {
        T value;
        Error error;
    };
    bool is_ok_;

  public:
    // Success case
    constexpr Result(const T &val) : value(val), is_ok_(true) {}

    // Error case
    constexpr Result(Error::Type err) : error(err), is_ok_(false) {}
    constexpr Result(Error err) : error(err), is_ok_(false) {}

    constexpr bool is_ok() const noexcept { return is_ok_; }
    constexpr bool is_err() const noexcept { return !is_ok_; }

    constexpr T unwrap() const {
        if (!is_ok_)
            panic("Attempted to unwrap an error");
        return value;
    }

    constexpr Error unwrap_err() const {
        if (is_ok_)
            panic("Attempted to unwrap_err on ok result");
        return error;
    }
    T expect(const char *message) {
        if (is_ok_)
            return value;
        kerror("%s, with error: %s", message, error.to_string());
        panic();
    }
};

template <> class Result<void> {
    Error error;
    bool is_ok_;

  public:
    // Success case
    constexpr Result() : is_ok_(true) {}

    // Error case
    constexpr Result(Error::Type err) : error(err), is_ok_(false) {}
    constexpr Result(Error err) : error(err), is_ok_(false) {}

    constexpr bool is_ok() const noexcept { return is_ok_; }
    constexpr bool is_err() const noexcept { return !is_ok_; }

    void unwrap() const { panic("Attempted to void Result"); }

    constexpr Error unwrap_err() const {
        if (is_ok_)
            panic("Attempted to unwrap_err on ok result");
        return error;
    }
    void expect(const char *message) {
        if (is_ok_)
            return;
        kerror("Expected %s, but got error: %s", message, error.to_string());
        panic();
    }
};

} // namespace klib
