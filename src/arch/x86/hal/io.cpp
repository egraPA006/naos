#include "kernel/hal/IO.hpp"

namespace kernel::HAL {
    // class IO {
    //     static void outb(uint16_t port, uint8_t value) {
    //         asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
    //     }

    //     static uint8_t inb(uint16_t port) {
    //         uint8_t ret;
    //         asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    //         return ret;
    //     }
    // };
}