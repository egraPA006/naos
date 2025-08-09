#pragma once
#include "klib/result.hpp"
#include <stddef.h>
#include <stdint.h>

namespace x86::idt {

enum class GateType : uint8_t {
    Task = 0x5,
    Interrupt16 = 0x6,
    Trap16 = 0x7,
    Interrupt32 = 0xE,
    Trap32 = 0xF
};

enum class PrivilegeLevel : uint8_t {
    Ring0 = 0,
    Ring1 = 1,
    Ring2 = 2,
    Ring3 = 3
};

struct [[gnu::packed]] GateDescriptor {
    uint16_t offset_low;  // Lower 16 bits of handler address
    uint16_t segment_sel; // Code segment selector
    uint8_t reserved = 0; // Always 0

    union {
        struct [[gnu::packed]] {
            GateType type : 4;      // Gate type (Interrupt/Trap)
            bool zero : 1;          // Must be 0
            PrivilegeLevel dpl : 2; // Descriptor privilege level
            bool present : 1;       // Is entry present?
        };
        uint8_t attributes; // Raw byte representation
    };

    uint16_t offset_high; // Upper 16 bits of handler address

    // Constructors
    constexpr GateDescriptor() noexcept
        : offset_low(0), segment_sel(0), reserved(0), attributes(0),
          offset_high(0) {}

    constexpr GateDescriptor(uint32_t offset, uint16_t selector,
                             GateType gate_type, PrivilegeLevel privilege,
                             bool is_present = true) noexcept
        : offset_low(static_cast<uint16_t>(offset & 0xFFFF)),
          segment_sel(selector),
          attributes(
              static_cast<uint8_t>(static_cast<uint8_t>(gate_type) & 0x0F) |
              ((static_cast<uint8_t>(privilege) & 0x03) << 5 |
               (is_present ? 0x80 : 0x00))),
          offset_high(static_cast<uint16_t>(offset >> 16)) {}

    constexpr GateDescriptor(uint16_t low, uint16_t sel, uint8_t attr,
                             uint16_t high) noexcept
        : offset_low(low), segment_sel(sel), attributes(attr),
          offset_high(high) {}
};
class IDT {
  public:
    static constexpr size_t MAX_ENTRIES = 256;

    [[nodiscard]] klib::Result<void> set_entry(size_t index, uint32_t offset,
                                               uint16_t selector, GateType type,
                                               PrivilegeLevel dpl,
                                               bool present = true);

    [[nodiscard]] klib::Result<void> clear_entry(size_t index);

    // Accessors with bounds checking
    [[nodiscard]] klib::Result<const GateDescriptor &>
    get_entry(size_t index) const;

    [[nodiscard]] klib::Result<GateDescriptor &> get_entry_mut(size_t index);

    // Keep operator[] for unchecked access when you're certain of bounds
    const GateDescriptor &operator[](size_t index) const {
        return entries_[index];
    }
    GateDescriptor &operator[](size_t index) { return entries_[index]; }

    void load() const; // lidt doesn't typically fail

  private:
    GateDescriptor entries_[MAX_ENTRIES]{};
};

} // namespace x86::idt
