// #include "kernel/terminal/vga.hpp"
#include "arch/x86/drivers/keyboard.hpp"
#include "arch/x86/gdt.hpp"
#include "arch/x86/idt.hpp"
#include "arch/x86/interrupts.hpp"
#include "arch/x86/pic.hpp"
#include "klib/printk.hpp"
#include "klib/result.hpp"
#include "klib/utils/neofetch.hpp"

extern "C" void kernel_main(void) {
    // setup gdt
    x86::gdt::Gdt gdt;
    {
        gdt.add_entry(x86::gdt::Gdt::kernel_code_segment())
            .expect("Failed to add kernel_code entry");
        gdt.add_entry(x86::gdt::Gdt::kernel_data_segment())
            .expect("Failed to add kernel_data entry");
        gdt.load().expect("Failed to load GDT");
        kinfo("Gdt loaded successfully");
    };
    // setup interrupts
    x86::pic::Initialize(0x20, 0x28);
    kinfo("PIC remapped to 0x20-0x2F");

    x86::idt::IDT idt;
    {
        // Initialize all IDT entries with default_handler
        for (size_t i = 0; i < x86::idt::IDT::MAX_ENTRIES; ++i) {
            x86::interrupts::register_handler(idt, static_cast<uint8_t>(i),
                                              x86::interrupts::default_handler,
                                              x86::idt::PrivilegeLevel::Ring0)
                .expect("Failed to set IDT entry");
        }
        x86::drivers::keyboard::register_handler(idt).expect(
            "Failed to set keboard handler");
        idt.load();
        kinfo("IDT loaded with default handlers");
    }
    x86::pic::MaskIRQ(0, true);
    x86::pic::MaskIRQ(1, false); // Unmask keyboard interrupt (IRQ1)
    kinfo("Unmasked keyboard IRQ");
    asm volatile("sti");
    kinfo("Interrupts enabled");
    klib::utils::neofetch();
    ksuccess("Entering main loop");
    while (true) {
        asm volatile("hlt"); // Wait for interrupts
    }
}
