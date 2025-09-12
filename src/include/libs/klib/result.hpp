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

template <typename T> class Result<T &> {
    union {
        T *ptr_;      // Pointer to referenced object
        Error error_; // Error storage
    };
    bool is_ok_;

  public:
    // Success case - stores pointer to referenced object
    constexpr Result(T &val) noexcept : ptr_(&val), is_ok_(true) {}

    // Error cases
    constexpr Result(Error::Type err) noexcept : error_(err), is_ok_(false) {}
    constexpr Result(Error err) noexcept : error_(err), is_ok_(false) {}

    // Check status
    constexpr bool is_ok() const noexcept { return is_ok_; }
    constexpr bool is_err() const noexcept { return !is_ok_; }

    // Accessors
    constexpr T &unwrap() const {
        if (!is_ok_) {
            panic("Attempted to unwrap an error");
        }
        return *ptr_;
    }

    constexpr Error unwrap_err() const {
        if (is_ok_) {
            panic("Attempted to unwrap_err on ok result");
        }
        return error_;
    }

    T &expect(const char *msg) const {
        if (!is_ok_) {
            kerror("%s: %s", msg, error_.to_string());
            panic();
        }
        return *ptr_;
    }

    // Disallow copying (references are inherently non-copyable)
    Result(const Result &) = delete;
    Result &operator=(const Result &) = delete;

    // Allow moving
    constexpr Result(Result &&other) noexcept : is_ok_(other.is_ok_) {
        if (is_ok_) {
            ptr_ = other.ptr_;
        } else {
            error_ = other.error_;
        }
    }

    constexpr Result &operator=(Result &&other) noexcept {
        if (this != &other) {
            is_ok_ = other.is_ok_;
            if (is_ok_) {
                ptr_ = other.ptr_;
            } else {
                error_ = other.error_;
            }
        }
        return *this;
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
