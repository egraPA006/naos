#include <klib/printk.h>
#include <klib/utils/neofetch.h>

extern "C" void kernel_main(void) {
    klib::utils::neofetch();
    klib::perror("Something happened!");
    klib::psuccess("Everything fine!");
    klib::printk("Test");
}
