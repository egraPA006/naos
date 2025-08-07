#include "kernel/terminal/vga.hpp"
#include "klib/printk.hpp"

namespace kernel::terminal::vga {

Terminal::Terminal()
    : row_(0), column_(0),
      color_(make_color(VGAColor::LightGrey, VGAColor::Black)),
      buffer_(reinterpret_cast<uint16_t *>(MEMORY_ADDRESS)) {
    clear();
}

uint8_t Terminal::make_color(VGAColor fg, VGAColor bg) const {
    return static_cast<uint8_t>(fg) | static_cast<uint8_t>(bg) << 4;
}

uint16_t Terminal::make_entry(char c, uint8_t color) const {
    return static_cast<uint16_t>(c) | static_cast<uint16_t>(color) << 8;
}

void Terminal::put_entry_at(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * WIDTH + x;
    buffer_[index] = make_entry(c, color);
}

void Terminal::put_char(char c) {
    if (c == '\n') {
        column_ = 0;
        if (++row_ == HEIGHT) {
            scroll();
            row_ = HEIGHT - 1;
        }
        return;
    }

    put_entry_at(c, color_, column_, row_);
    if (++column_ == WIDTH) {
        column_ = 0;
        if (++row_ == HEIGHT) {
            scroll();
            row_ = HEIGHT - 1;
        }
    }
}

void Terminal::write(const char *data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        put_char(data[i]);
    }
}

void Terminal::write_string(const char *data) {
    while (*data != '\0') {
        put_char(*data++);
    }
}

void Terminal::clear() {
    for (size_t y = 0; y < HEIGHT; y++) {
        for (size_t x = 0; x < WIDTH; x++) {
            put_entry_at(' ', color_, x, y);
        }
    }
    row_ = 0;
    column_ = 0;
}

void Terminal::scroll() {
    // Move all lines up by one
    for (size_t y = 1; y < HEIGHT; y++) {
        for (size_t x = 0; x < WIDTH; x++) {
            const size_t current = y * WIDTH + x;
            const size_t above = (y - 1) * WIDTH + x;
            buffer_[above] = buffer_[current];
        }
    }

    // Clear the bottom line
    const size_t bottom_line = HEIGHT - 1;
    for (size_t x = 0; x < WIDTH; x++) {
        put_entry_at(' ', color_, x, bottom_line);
    }
}
void Terminal::set_color(VGAColor fg, VGAColor bg) {
    color_ = make_color(fg, bg);
}

} // namespace kernel::terminal::vga
