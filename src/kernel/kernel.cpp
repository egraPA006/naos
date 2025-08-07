// #include "kernel/terminal/vga.hpp"
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
    klib::utils::neofetch();
    kdebug("%s", test(4).unwrap_err().to_string());
    switch (test(4).unwrap_err()) {
    case Error::InvalidArgument:
        kinfo("Lol kek");
        break;
    default:
        kinfo("Wtf");
    };
    kwarn("%d", test(2).unwrap());
}
