#include "klib/utils/neofetch.hpp"
#include <arch/x86/serial.hpp>
#include <cstddef>
#include <stddef.h>

namespace klib::utils {

void neofetch() {
    using x86::serial::color;
    using x86::serial::write;

    // ASCII Art - schreck
    const char *ascii_art[] = {
        "        .-.__      \\ .-.  ___  __",
        "       |_|  '--.-.-(   \\/\\;;\\_\\.-._______.-.",
        "      (-)___     \\ \\ .-\\ \\;;\\(   \\       \\ \\",
        "       Y    '---._\\_((Q)) \\;;\\\\ .-\\     __(_)",
        "       I           __'-' / .--.((Q))---'    \\,",
        "       I     ___.-:    \\|  |   \\'-'_          \\",
        "       A  .-'      \\ .-.\\   \\   \\ \\ '--.__     '\\",
        "       |  |____.----((Q))\\   \\__|--\\_      \\     '",
        "          ( )        '-'  \\_  :  \\-' '--.___\\",
        "           Y                \\  \\  \\       \\(_)",
        "           I                 \\  \\  \\         \\,",
        "           I                  \\  \\  \\          \\",
        "           A                   \\  \\  \\          '\\",
        "           |                    \\  \\__|           '",
        "                                \\_:.  \\",
        "                                  \\ \\  \\",
        "                                   \\ \\  \\",
        "                                    \\_\\_|"};

    // Calculate array sizes without library functions
    const size_t art_lines = sizeof(ascii_art) / sizeof(ascii_art[0]);

    // Print side by side
    for (size_t i = 0; i < art_lines; i++) {
        // Print ASCII art line
        write(color::White, ascii_art[i]);

        // Add appropriate spacing (you might need to adjust this)
        write("   ");

        // Print info line with colors
        if (i == 2) {
            write(color::Magenta, "    NAOS");
            write(color::White, " @ ");
            write(color::Cyan, "x86\n");
        } else if (i == 4) {
            write(color::Green, "  OS: ");
            write(color::White, "NAOS 0.0.1\n");
        } else if (i == 5) {
            write(color::Green, " CPU: ");
            write(color::White, "i386\n");
        } else {
            write('\n');
        }
    }
    for (size_t i = 0; i < 4; i++) {
        write('\n');
    }
}

} // namespace klib::utils
