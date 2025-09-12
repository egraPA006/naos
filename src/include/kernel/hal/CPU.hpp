#include <stdint.h>
#pragma once

namespace kernel::HAL {
    class CPU {
    public:
        ~CPU() = default;
        CPU(const CPU&) = delete;
        CPU& operator=(const CPU&) = delete;
        virtual void init() = 0;
        virtual void halt() = 0;
        virtual void disable_interrupts()= 0;
        virtual void enable_interrupts() = 0;
        static CPU& get_instance();
    protected:
        CPU() = default;
    };
}