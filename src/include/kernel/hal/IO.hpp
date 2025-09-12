#pragma once
#include <stdint.h>
namespace kernel::HAL {
    class IO {
    public:
        static void outb(uint16_t port, uint8_t value);

        static uint8_t inb(uint16_t port);
    };

}