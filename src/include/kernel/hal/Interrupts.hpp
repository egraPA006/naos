#include <stdint.h>
#pragma once

namespace kernel::HAL {
    struct InterruptHandler {
        static void handle();
    };

    class InterruptController {
    public:        
        static void init();
        static void enable_interrupt(uint32_t irq);
        static void disable_interrupt(uint32_t irq);
        static void register_handler(uint32_t irq, InterruptHandler* handler);
    };
}