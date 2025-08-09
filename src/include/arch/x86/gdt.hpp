#pragma once
#include "klib/result.hpp"
#include <stddef.h>
#include <stdint.h>
namespace x86::gdt {
enum class SegmentType : bool { Data = 0, Code = 1 };
enum class PrivilegeLevel : uint8_t {
    Ring0 = 0,
    Ring1 = 1,
    Ring2 = 2,
    Ring3 = 3
};

struct [[gnu::packed]] GdtEntry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_mid;
    union {
        uint8_t access_raw : 8;
        struct { // TODO: another union entry for tasks in the
                 // future?
            bool accessed : 1;
            bool read_write : 1; // Writable for data, Readable for code
            bool direction : 1;  // Direction/Conforming
            SegmentType type : 1;
            bool descriptor_type : 1; // 1 for code/data, 0 for system
            PrivilegeLevel dpl : 2;
            bool present : 1;
        } access_bits;
    };
    union {
        uint8_t limit_flags_raw;
        struct [[gnu::packed]] {
            uint8_t limit_high : 4;
            bool reserved : 1;
            bool long_mode : 1;
            bool size : 1;        // 0 = 16-bit, 1 = 32-bit
            bool granularity : 1; // 0 = bytes, 1 = 4KB pages
        };
    };
    uint8_t base_high;
    constexpr GdtEntry() noexcept
        : limit_low(0), base_low(0), base_mid(0), access_raw(0),
          limit_flags_raw(0), base_high(0) {}

    // Full constructor
    constexpr GdtEntry(uint32_t base, uint32_t limit, uint8_t access,
                       uint8_t flags) noexcept
        : limit_low(limit & 0xFFFF), base_low(base & 0xFFFF),
          base_mid((base >> 16) & 0xFF), access_raw(access),
          limit_flags_raw(((limit >> 16) & 0xF) | ((flags & 0xF) << 4)),
          base_high((base >> 24) & 0xFF) {}
};

class Gdt {
  public:
    static constexpr size_t MAX_ENTRIES = 8;

    Gdt() noexcept;

    // Descriptor management
    [[nodiscard]] klib::Result<size_t> add_entry(const GdtEntry &entry);
    [[nodiscard]] klib::Result<size_t>
    add_segment(uint32_t base, uint32_t limit, SegmentType type,
                PrivilegeLevel dpl, bool read_write, bool conforming,
                bool granular, bool size_32bit);

    // GDT operations
    [[nodiscard]] klib::Result<void> load() const;

    // Selector operations
    [[nodiscard]] klib::Result<uint16_t>
    create_selector(size_t index,
                    PrivilegeLevel rpl = PrivilegeLevel::Ring0) const;

    // Predefined segments
    static constexpr GdtEntry null_descriptor() noexcept {
        return GdtEntry{}; // All fields zeroed by default constructor
    }

    static constexpr GdtEntry kernel_code_segment() noexcept {
        return GdtEntry(
            0,       // Base address (0x0)
            0xFFFFF, // Limit (0xFFFFF)
            0x9A,    // Access byte:
                     //   Present (1) | DPL (00) | Descriptor Type (1) |
            //   Code (1) | Conforming (0) | Readable (1) | Accessed (0)
            0xC // Flags:
                //   Granularity (1) | 32-bit (1) | Not 64-bit (0) | Reserved
                //   (0)
        );
    }

    static constexpr GdtEntry kernel_data_segment() noexcept {
        return GdtEntry(
            0,       // Base address (0x0)
            0xFFFFF, // Limit (0xFFFFF)
            0x92,    // Access byte:
                     // Present (1) | DPL (00) | Descriptor Type (1) |
                     // Data (0) | Direction (0) | Writable (1) | Accessed (0)
            0xC      // Flags:
                //   Granularity (1) | 32-bit (1) | Not 64-bit (0) | Reserved
                //   (0)
        );
    }

    static constexpr GdtEntry user_code_segment() noexcept {
        return GdtEntry(
            0,       // Base address (0x0)
            0xFFFFF, // Limit (0xFFFFF)
            0xFA,    // Access byte:
                     // Present (1) | DPL (11) | Descriptor Type (1) |
                     // Code (1) | Conforming (0) | Readable (1) | Accessed (0)
            0xC      // Flags:
                //   Granularity (1) | 32-bit (1) | Not 64-bit (0) | Reserved
                //   (0)
        );
    }

    static constexpr GdtEntry user_data_segment() noexcept {
        return GdtEntry(
            0,       // Base address (0x0)
            0xFFFFF, // Limit (0xFFFFF)
            0xF2,    // Access byte:
                     // Present (1) | DPL (11) | Descriptor Type (1) |
                     // Data (0) | Direction (0) | Writable (1) | Accessed (0)
            0xC      // Flags:
                //   Granularity (1) | 32-bit (1) | Not 64-bit (0) | Reserved
                //   (0)
        );
    }

  private:
    GdtEntry entries_[MAX_ENTRIES];
    size_t next_free_;

    struct [[gnu::packed]] GdtPointer {
        uint16_t limit;
        const GdtEntry *base;
    };
    [[nodiscard]] klib::Result<void> validate_index(size_t index) const;
};

} // namespace x86::gdt
