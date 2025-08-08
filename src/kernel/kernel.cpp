// #include "kernel/terminal/vga.hpp"
#include "arch/x86/gdt.hpp"
#include "klib/error.hpp"
#include "klib/panic.hpp"
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
    klib::utils::neofetch();
    // setup gdt
    x86::gdt::Gdt gdt;
    gdt.add_entry(x86::gdt::Gdt::kernel_code_segment())
        .expect("Failed to add kernel_code entry");
    gdt.add_entry(x86::gdt::Gdt::kernel_data_segment())
        .expect("Failed to add kernel_data entry");
    gdt.load().expect("Failed to load GDT");
    kinfo("Gdt loaded successfully");
}
