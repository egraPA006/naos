#include "arch/x86/drivers/keyboard.hpp"
#include "arch/x86/interrupts.hpp"
#include "arch/x86/io.hpp"
#include "arch/x86/pic.hpp"
#include "klib/printk.hpp"

namespace x86::drivers::keyboard {
namespace {
char scancode_to_ascii(uint8_t scancode) {
    static const char *layout =
        "??1234567890-=??qwertyuiop[]\n?asdfghjkl;'`??\\zxcvbnm,./?*? ?";
    if (scancode >= sizeof(layout))
        return 0;
    return layout[scancode];
}
[[gnu::interrupt]] void
handler([[gnu::unused]] interrupts::InterruptFrame *frame) {
    uint8_t scancode = io::inb(0x60);
    kdebug("Numerical input: 0x%x", scancode);
    if (scancode < 0x80) {
        kdebug("Maybe: %c", '1' + scancode);  // Ignore key-up scancodes (0x80+)
        char c = scancode_to_ascii(scancode); // Implement this function
        if (c != 0) {
            kinfo("Got input %c", c);
        }
    }
    x86::pic::SendEOI(1);
}
} // namespace
klib::Result<void> register_handler(x86::idt::IDT &idt) {
    auto result = x86::interrupts::register_handler(
        idt, static_cast<uint8_t>(0x21), handler,
        x86::idt::PrivilegeLevel::Ring0);
    if (result.is_err())
        return result;
    return {};
}
} // namespace x86::drivers::keyboard
