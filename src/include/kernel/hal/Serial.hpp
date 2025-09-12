#include <stdint.h>
#pragma once

namespace kernel::HAL {
    class Serial {
    public:
        enum Color : uint8_t {
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
            Debug = White
        };
        static void initialize();
        static void write(char c);
        static void write(const char *str);
        static void write(Color col, char c);
        static void write(Color col, const char *str);
    };
}