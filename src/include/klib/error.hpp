#pragma once
#include <stdint.h>

namespace klib {

class Error {
  public:
    enum Type : uint8_t {
        None = 0,
        InvalidArgument,
        OutOfMemory,
        DeviceError,
        FileNotFound,
        GdtFull,
        InvalidGdt,
        // Add new errors here...
        _Count
    };

    // Direct construction
    constexpr Error() = default;
    constexpr Error(Type t) : type_(t) {}

    // Enable direct switching
    constexpr operator Type() const noexcept { return type_; }

    // String conversion
    const char *to_string() const noexcept {
        static const char *names[] = {
            "None",         "InvalidArgument", "OutOfMemory", "DeviceError",
            "FileNotFound", "GdtFull",         "InvalidGdt"
            // Must match enum order
        };
        return type_ < _Count ? names[type_] : "Unknown";
    }

  private:
    Type type_ = None;
};
} // namespace klib
