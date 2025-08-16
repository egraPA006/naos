// interrupts.h
#pragma once
#include "arch/x86/idt.hpp"
#include "klib/result.hpp"
#include <stdint.h>

namespace x86::drivers::keyboard {
[[nodiscard]] klib::Result<void> register_handler(x86::idt::IDT &idt);
} // namespace x86::drivers::keyboard
