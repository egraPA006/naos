#include "kernel/hal/Serial.hpp"
#include "kernel/hal/IO.hpp"

namespace kernel::HAL {
    class Serial {
    static constexpr uint16_t COM1 = 0x3F8;
    static constexpr const char *color_escapes[] = {
        "\033[30m", // Black
        "\033[34m", // Blue
        "\033[32m", // Green
        "\033[36m", // Cyan
        "\033[31m", // Red
        "\033[35m", // Magenta
        "\033[33m", // Yellow
        "\033[37m"  // White
    };
    static constexpr const char *color_reset = "\033[0m";
    public:
        // static void initialize() {
        //     IO::outb(COM1 + 1, 0x00); // Disable interrupts
        //     IO::outb(COM1 + 3, 0x80); // Enable DLAB
        //     IO::outb(COM1 + 0, 0x03); // Baud rate divisor
        //     IO::outb(COM1 + 1, 0x00);
        //     IO::outb(COM1 + 3, 0x03); // 8N1
        //     IO::outb(COM1 + 2, 0xC7); // FIFO
        // }
        // static void write(char c) {
        //     while ((IO::inb(COM1 + 5) & 0x20) == 0);
        //     IO::outb(COM1, c);
        // }

        // static void write(const char *str) {
        //     while (*str) {
        //         write(*str++);
        //     }
        // }

        // static void write(kernel::HAL::Serial::Color col, char c) {
        //     // Write color escape sequence
        //     write(color_escapes[static_cast<uint8_t>(col)]);
        //     // Write the actual char
        //     write(c);
        //     // Reset color
        //     write(color_reset);
        // }

        // void write(kernel::HAL::Serial::Color col, const char *str) {
        //     // Write color escape sequence
        //     write(color_escapes[static_cast<uint8_t>(col)]);
        //     // Write the actual string
        //     write(str);
        //     // Reset color
        //     write(color_reset);
        // }
    };
}