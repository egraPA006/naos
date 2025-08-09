#include "arch/x86/idt.hpp"
namespace x86::idt {
klib::Result<void> IDT::set_entry(size_t index, uint32_t offset,
                                  uint16_t selector, GateType type,
                                  PrivilegeLevel dpl, bool present) {
    if (index >= MAX_ENTRIES) {
        return klib::Error::OutOfBounds;
    }
    if (selector == 0) {
        return klib::Error::NullSelector;
    }

    entries_[index] = GateDescriptor(offset, selector, type, dpl, present);
    return {};
}
klib::Result<void> IDT::clear_entry(size_t index) {
    if (index >= MAX_ENTRIES) {
        return klib::Error::OutOfBounds;
    }
    entries_[index].present = false;
    return {};
}
klib::Result<const GateDescriptor &> IDT::get_entry(size_t index) const {
    if (index >= MAX_ENTRIES) {
        return klib::Error::OutOfBounds;
    }
    return entries_[index];
}
klib::Result<GateDescriptor &> IDT::get_entry_mut(size_t index) {
    if (index >= MAX_ENTRIES) {
        return klib::Error::OutOfBounds;
    }
    return entries_[index];
}
void IDT::load() const {
    struct [[gnu::packed]] IdtPtr {
        uint16_t limit;
        uint32_t base;
    };

    IdtPtr ptr = {.limit = sizeof(entries_) - 1,
                  .base = reinterpret_cast<uint32_t>(&entries_[0])};

    // Inline assembly for protected mode lidt
    asm volatile("lidt %0" : : "m"(ptr) : "memory");
}
} // namespace x86::idt
