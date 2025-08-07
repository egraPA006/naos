#include <kernel/terminal/vga.hpp>
#include <klib/printk.hpp>
#include <klib/utils/neofetch.hpp>

extern "C" void kernel_main(void) {
    klib::utils::neofetch();
    kernel::terminal::vga::Terminal terminal;
    {
        using kernel::terminal::vga::VGAColor;
        terminal.set_color(VGAColor::Black, VGAColor::Blue);
    }
    terminal.write_string("test test test");
}
