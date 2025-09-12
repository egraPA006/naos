#include "arch/x86/gdt.hpp"
#include "klib/error.hpp"
#include "klib/printk.hpp"

namespace x86::gdt {
Gdt::Gdt() noexcept : entries_{}, next_free_(1) {
    entries_[0] = null_descriptor();
}
klib::Result<size_t> Gdt::add_entry(const GdtEntry &entry) {
    if (next_free_ >= MAX_ENTRIES) {
        return klib::Error::GdtFull;
    }

    const size_t index = next_free_++;
    entries_[index] = entry;
    return index;
}
klib::Result<size_t> Gdt::add_segment(uint32_t base, uint32_t limit,
                                      SegmentType type, PrivilegeLevel dpl,
                                      bool read_write, bool conforming,
                                      bool granular, bool size_32bit) {
    GdtEntry entry;

    // Access byte construction
    auto access = decltype(entry.access_bits){
        .accessed = false,
        .read_write = read_write,
        .direction = conforming,
        .type = type,
        .descriptor_type = true, // Always 1 for code/data segments
        .dpl = dpl,
        .present = true,
    };
    entry.access_bits = access;

    // Address range
    entry.limit_low = limit & 0xFFFF;
    entry.limit_high = (limit >> 16) & 0xF;
    entry.base_low = base & 0xFFFF;
    entry.base_mid = (base >> 16) & 0xFF;
    entry.base_high = (base >> 24) & 0xFF;
    entry.reserved = false;
    entry.long_mode = false;
    entry.size = size_32bit;
    entry.granularity = granular;

    return add_entry(entry);
}

klib::Result<void> Gdt::load() const {
    if (next_free_ == 0) {
        return klib::Error::InvalidGdt;
    }

    struct [[gnu::packed]] GdtPointer {
        uint16_t limit;
        const GdtEntry *base;
    };
    const GdtPointer ptr{
        .limit = static_cast<uint16_t>(next_free_ * sizeof(GdtEntry) - 1),
        .base = entries_};
    asm volatile("cli\n"
                 // Load GDT
                 "lgdt %0\n"
                 // Reload data segment registers (0x10 = kernel data selector)
                 "mov $0x10, %%ax\n"
                 "mov %%ax, %%ds\n"
                 "mov %%ax, %%es\n"
                 "mov %%ax, %%fs\n"
                 "mov %%ax, %%gs\n"
                 "mov %%ax, %%ss\n"

                 // Far jump to reload CS (0x08 = kernel code selector)
                 "ljmp $0x08, $1f\n"
                 "1:\n"
                 :
                 : [ptr] "m"(ptr)
                 : "eax", "memory");

    return {};
}

klib::Result<uint16_t> Gdt::create_selector(size_t index,
                                            PrivilegeLevel rpl) const {
    // Validate the index first
    if (auto valid = validate_index(index); valid.is_err()) {
        return valid.unwrap_err(); // Propagates the error (OutOfBounds or
                                   // InvalidDescriptor)
    }

    // Calculate selector value:
    // - Index shifted left by 3 (multiplied by 8, since each entry is 8 bytes)
    // - OR'ed with RPL (bits 0-1)
    const uint16_t selector =
        static_cast<uint16_t>((index << 3) | static_cast<uint8_t>(rpl));

    return selector;
}
klib::Result<void> Gdt::validate_index(size_t index) const {
    if (index >= MAX_ENTRIES) {
        return klib::Error::OutOfBounds;
    }
    if (index >= next_free_) {
        return klib::Error::InvalidDescriptor;
    }
    return {};
}
} // namespace x86::gdt
