#include "printk.h"
#include <arch/x86/serial.h>
#include <stdarg.h>

namespace klib {
namespace {

void write_serial(char c) {
    // x86::serial::Serial s;
    // s.write('c');
}

void number_to_str(__attribute__((unused)) char *buf,
                   __attribute__((unused)) int num,
                   __attribute__((unused)) int base) { /* ... */ }
} // namespace

__printk::log_level LOG_LEVEL = __printk::DEBUG;

void __printk::set_log_level(log_level level) { LOG_LEVEL = level; }

void printk(__printk::log_level level, const char *format, ...) {
    va_list args;
    va_start(args, format);

    // Log level prefix (optional)
    const char *level_str[] = {"[ERROR] ", "[WARN] ", "[INFO] ", "[DEBUG] "};
    for (const char *p = level_str[level]; *p; p++) {
        write_serial(*p);
    }
    write_serial(' ');

    // Format parsing
    for (const char *p = format; *p; p++) {
        if (*p != '%') {
            write_serial(*p);
            continue;
        }

        // Handle format specifier
        switch (*++p) {
        case 's': { // String
            const char *str = va_arg(args, const char *);
            while (*str)
                write_serial(*str++);
            break;
        }
        case 'd': { // Decimal
            char buf[16];
            number_to_str(buf, va_arg(args, int), 10);
            for (char *c = buf; *c; c++)
                write_serial(*c);
            break;
        }
        case 'x': { // Hexadecimal
            char buf[16];
            number_to_str(buf, va_arg(args, int), 16);
            for (char *c = buf; *c; c++)
                write_serial(*c);
            break;
        }
        case '%': { // Literal %
            write_serial('%');
            break;
        }
        }
    }
}
} // namespace klib
