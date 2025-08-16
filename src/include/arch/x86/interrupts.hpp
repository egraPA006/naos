#pragma once
#include "arch/x86/idt.hpp"
#include "klib/result.hpp"
#include <stdint.h>

namespace x86::interrupts {

// Interrupt frame pushed by CPU when an interrupt occurs
struct [[gnu::packed]] InterruptFrame {
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    // Only present if interrupt pushes error code
    // uint32_t error_code;
};

// Type for interrupt handler functions using GCC interrupt attribute
using Handler = __attribute__((interrupt)) void (*)(InterruptFrame *frame);

// Register a handler for specific interrupt vector
[[nodiscard]] klib::Result<void>
register_handler(idt::IDT &idt, uint8_t interrupt, Handler handler,
                 idt::PrivilegeLevel dpl = idt::PrivilegeLevel::Ring0);

// Default handler that halts the system
[[gnu::interrupt]] void default_handler(InterruptFrame *frame);

} // namespace x86::interrupts
