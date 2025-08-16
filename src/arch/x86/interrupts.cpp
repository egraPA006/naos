#include "arch/x86/interrupts.hpp"
#include "arch/x86/pic.hpp"
#include "klib/panic.hpp"
#include "klib/printk.hpp"

namespace x86::interrupts {

// Register a handler for specific interrupt vector
[[nodiscard]] klib::Result<void> register_handler(idt::IDT &idt,
                                                  uint8_t interrupt,
                                                  Handler handler,
                                                  idt::PrivilegeLevel dpl) {
    uint32_t handler_address = reinterpret_cast<uint32_t>(handler);
    auto result = idt.set_entry(
        interrupt, handler_address,
        0x08, // Kernel code segment selector (assuming GDT index 1)
        idt::GateType::Interrupt32, // 32-bit interrupt gate
        dpl,                        // Privilege level from parameter
        true                        // Present
    );
    if (result.is_err())
        return result;
    if (interrupt >= 32 && interrupt < 48) {
        uint8_t irq = static_cast<uint8_t>(interrupt - 32);
        x86::pic::MaskIRQ(irq, false); // Unmask the IRQ line
    }
    return {};
}

// Default handler that halts the system
[[gnu::interrupt]] void default_handler([[gnu::unused]] InterruptFrame *frame) {
    klib::panic("default interrupt handler");
}
} // namespace x86::interrupts
