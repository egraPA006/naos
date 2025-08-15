// #include "kernel/terminal/vga.hpp"
#include "arch/x86/gdt.hpp"
#include "arch/x86/idt.hpp"
#include "arch/x86/pic.hpp"
#include "klib/error.hpp"
#include "klib/printk.hpp"
#include "klib/result.hpp"
#include "klib/utils/neofetch.hpp"

using klib::Error;
using klib::Result;

Result<int> test(int a) {
    if (a > 3)
        return Error::InvalidArgument;
    return 4;
}

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
    x86::idt::IDT idt;
    idt.load();
    kinfo("Idt loaded successfully");
    x86::pic::Initialize(0x20, 0x28);

    klib::utils::neofetch();
}
