#include "arch/x86/serial.h"
#include <arch/x86/io.h>
#include <stdint.h>

namespace x86::serial {
constexpr uint16_t COM1 = 0x3F8;
bool initialized = false;

// ANSI color escape codes (matches the enum order)
constexpr const char *color_escapes[] = {
    "\033[30m", // Black
    "\033[34m", // Blue
    "\033[32m", // Green
    "\033[36m", // Cyan
    "\033[31m", // Red
    "\033[35m", // Magenta
    "\033[33m", // Yellow
    "\033[37m"  // White
};
constexpr const char *color_reset = "\033[0m";

void initialize() {
    using namespace x86::io;
    outb(COM1 + 1, 0x00); // Disable interrupts
    outb(COM1 + 3, 0x80); // Enable DLAB
    outb(COM1 + 0, 0x03); // Baud rate divisor
    outb(COM1 + 1, 0x00);
    outb(COM1 + 3, 0x03); // 8N1
    outb(COM1 + 2, 0xC7); // FIFO
    initialized = true;
}

void write(char c) {
    if (!initialized) {
        initialize();
    }
    using namespace x86::io;
    while ((inb(COM1 + 5) & 0x20) == 0)
        ;
    outb(COM1, c);
}

void write(const char *str) {
    while (*str) {
        write(*str++);
    }
}

void write(color col, const char *str) {
    // Write color escape sequence
    write(color_escapes[static_cast<uint8_t>(col)]);
    // Write the actual string
    write(str);
    // Reset color
    write(color_reset);
}
} // namespace x86::serial
