#pragma once
#include "klib/error.hpp"
#include "klib/panic.hpp"

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
};

} // namespace klib
