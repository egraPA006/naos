#pragma once

#include <stdint.h>

namespace x86::pic {

// Port addresses for master and slave PICs
enum class Port : uint16_t {
    MASTER_COMMAND = 0x20,
    MASTER_DATA = 0x21,
    SLAVE_COMMAND = 0xA0,
    SLAVE_DATA = 0xA1
};
// Initialization command word master
enum class ICW1 : uint8_t {
    ICW4_NEEDED = 0x01,   // ICW4 needed
    SINGLE = 0x02,        // Single (cascade) mode
    INTERVAL4 = 0x04,     // Call address interval 4 (8)
    LEVEL_TRIGGER = 0x08, // Level triggered (edge) mode
    INITIALIZATION = 0x10 // Initialization - must be set
};
// Initialization command word slave
enum class ICW4 : uint8_t {
    MODE_8086 = 0x01,           // 8086/88 mode
    AUTO_EOI = 0x02,            // Auto EOI
    BUFFERED_SLAVE = 0x08,      // Buffered mode slave
    BUFFERED_MASTER = 0x0C,     // Buffered mode master
    SPECIAL_FULLY_NESTED = 0x10 // Special fully nested mode
};

inline ICW1 operator|(ICW1 a, ICW1 b) {
    return static_cast<ICW1>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

inline ICW4 operator|(ICW4 a, ICW4 b) {
    return static_cast<ICW4>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}
// End of Interrupt command
constexpr uint8_t PIC_EOI = 0x20;

// Initialize the PIC controllers
// Arguments:
//   offset1 - vector offset for master PIC
//   offset2 - vector offset for slave PIC
void Initialize(uint8_t offset1, uint8_t offset2);

// Disable the PIC by masking all interrupts
void Disable();

// Send End-of-Interrupt signal for the specified IRQ
void SendEOI(uint8_t irq);

// Mask or unmask an IRQ line
void MaskIRQ(uint8_t irq, bool masked);

// Set the interrupt mask (what IRQs are allowed)
void SetMask(uint16_t mask);

// Get the current interrupt mask
uint16_t GetMask();

uint16_t GetIRR();
uint16_t GetISR();

} // namespace x86::pic
