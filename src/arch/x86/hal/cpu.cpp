#include <kernel/hal/CPU.hpp>

namespace kernel::HAL {
    class x86_CPU : CPU {
    public:
        void init() {

        }
        void halt() {

        }
        void disable_interrupts() {

        }
        void enable_interrupts() {
            
        }
        static CPU& get_instance() {
            static x86_CPU instance;
            return instance;
        }
    };
}