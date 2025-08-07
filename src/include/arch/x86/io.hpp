#pragma once
#include <stdint.h>
namespace x86::io {
void outb(uint16_t port, uint8_t value);

uint8_t inb(uint16_t port);
} // namespace x86::io
