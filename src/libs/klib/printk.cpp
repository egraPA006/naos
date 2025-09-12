#include "klib/printk.hpp"
#include "arch/x86/serial.hpp"
#include <stdarg.h>

namespace klib {
namespace {

// Map log levels to colors
constexpr x86::serial::color level_colors[] = {
    x86::serial::Error,   // ERROR
    x86::serial::Warning, // WARN
    x86::serial::Success, // SUCCESS
    x86::serial::Info,    // INFO
    x86::serial::Debug    // DEBUG
};

void write_char(x86::serial::color col, char c) { x86::serial::write(col, c); }

void write_string(x86::serial::color col, const char *str) {
    x86::serial::write(col, str);
}

void number_to_str(char *buf, int num, int base) {
    static const char digits[] = "0123456789abcdef";
    char *ptr = buf;
    bool negative = false;

    if (num < 0 && base == 10) {
        negative = true;
        num = -num;
    }

    // Handle 0 explicitly
    if (num == 0) {
        *ptr++ = '0';
        *ptr = '\0';
        return;
    }

    // Convert number to string (reverse order)
    while (num != 0) {
        *ptr++ = digits[num % base];
        num /= base;
    }

    if (negative) {
        *ptr++ = '-';
    }

    *ptr = '\0';

    // Reverse the string
    char *start = buf;
    char *end = ptr - 1;
    while (start < end) {
        char tmp = *start;
        *start = *end;
        *end = tmp;
        start++;
        end--;
    }
}

void print(const x86::serial::color color, const char *format, va_list args) {
    // Format parsing
    for (const char *p = format; *p; p++) {
        if (*p != '%') {
            write_char(color, *p);
            continue;
        }

        // Handle format specifier
        switch (*++p) {
        case 's': { // String
            const char *str = va_arg(args, const char *);
            write_string(color, str);
            break;
        }
        case 'd': { // Decimal
            char buf[16];
            number_to_str(buf, va_arg(args, int), 10);
            write_string(color, buf);
            break;
        }
        case 'x': { // Hexadecimal
            char buf[16];
            number_to_str(buf, va_arg(args, int), 16);
            write_string(color, buf);
            break;
        }
        case 'c': { // Character
            char c = static_cast<char>(va_arg(args, int));
            write_char(color, c);
            break;
        }
        case '%': { // Literal %
            write_char(color, '%');
            break;
        }
        default: { // Unknown format - just print it
            write_char(color, '%');
            write_char(color, *p);
            break;
        }
        }
    }
    x86::serial::write('\n');
}
} // namespace

__printk::log_level LOG_LEVEL = __printk::DEBUG;

void __printk::set_log_level(log_level level) { LOG_LEVEL = level; }

void printk(const char *format, ...) {
    va_list args;
    va_start(args, format);

    print(x86::serial::color::White, format, args);
    va_end(args);
}

void printk(__printk::log_level level, const char *format, ...) {
    if (level > LOG_LEVEL) {
        return;
    }

    va_list args;
    va_start(args, format);

    const x86::serial::color color = level_colors[level];

    // Log level prefix
    const char *level_str[] = {"[ERROR] ", "[WARN]  ", "[SUCCESS]  ",
                               "[INFO]  ", "[DEBUG] "};
    write_string(color, level_str[level]);

    print(color, format, args);
    va_end(args);
}
} // namespace klib
