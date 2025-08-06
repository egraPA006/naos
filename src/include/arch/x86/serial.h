#pragma once

#include <stdint.h>

namespace x86::serial {
enum color : uint8_t {
    Black = 0,
    Blue = 1,
    Green = 2,
    Cyan = 3,
    Red = 4,
    Magenta = 5,
    Yellow = 6,
    White = 7,

    // Aliases
    Error = Red,
    Warning = Yellow,
    Success = Green,
    Info = Cyan,
    Debug = Magenta
};
void write(char c);
void write(const char *str);
void write(color col, char c);
void write(color col, const char *str);
} // namespace x86::serial
