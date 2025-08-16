#include "klib/panic.hpp"
#include "klib/printk.hpp"
namespace klib {
[[noreturn]] void panic(const char *message) {
    asm volatile("cli");

    kerror("KERNEL PANIC: %s", message);

    while (1) {
        asm volatile("hlt");
    }
}
[[noreturn, gnu::no_caller_saved_registers]] void panic() {
    asm volatile("cli");
    kerror("KERNEL PANIC!");
    while (1) {
        asm volatile("hlt");
    }
}
} // namespace klib
