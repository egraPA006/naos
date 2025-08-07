#pragma once
#include <stddef.h>
#include <stdint.h>

namespace kernel::terminal::vga {

enum class VGAColor : uint8_t {
    Black = 0,
    Blue = 1,
    Green = 2,
    Cyan = 3,
    Red = 4,
    Magenta = 5,
    Brown = 6,
    LightGrey = 7,
    DarkGrey = 8,
    LightBlue = 9,
    LightGreen = 10,
    LightCyan = 11,
    LightRed = 12,
    LightMagenta = 13,
    LightBrown = 14,
    White = 15
};

class Terminal {
  public:
    static constexpr size_t WIDTH = 80;
    static constexpr size_t HEIGHT = 25;
    static constexpr uintptr_t MEMORY_ADDRESS = 0xB8000;

    Terminal();

    void set_color(VGAColor fg, VGAColor bg);
    void put_char(char c);
    void write(const char *data, size_t size);
    void write_string(const char *data);
    void clear();

  private:
    size_t row_;
    size_t column_;
    uint8_t color_;
    uint16_t *buffer_;

    uint8_t make_color(VGAColor fg, VGAColor bg) const;
    uint16_t make_entry(char c, uint8_t color) const;
    void put_entry_at(char c, uint8_t color, size_t x, size_t y);
    void scroll();
};

} // namespace kernel::terminal::vga
