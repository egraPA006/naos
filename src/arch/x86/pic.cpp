#include "arch/x86/pic.hpp"
#include "arch/x86/io.hpp"

namespace x86::pic {

namespace {
enum class OCW3 : uint8_t { READ_IRR = 0x0a, READ_ISR = 0x0b };
// Helper function for writing to PIC Ports
void write(Port p, uint8_t value) {
    x86::io::outb(static_cast<uint16_t>(p), value);
    if (p == Port::MASTER_COMMAND || p == Port::SLAVE_COMMAND) {
        x86::io::outb(0x80, 0); // io_wait
    }
}

uint8_t read(Port p) { return x86::io::inb(static_cast<uint16_t>(p)); }

uint16_t get_irq_reg(OCW3 ocw3) {
    write(Port::MASTER_COMMAND, static_cast<uint8_t>(ocw3));
    write(Port::SLAVE_COMMAND, static_cast<uint8_t>(ocw3));
    return (read(Port::SLAVE_COMMAND) << 8) | read(Port::MASTER_COMMAND);
}

// Helper function for reading from PIC Ports

} // anonymous namespace

void Initialize(uint8_t master_offset, uint8_t slave_offset) {
    // Save current masks
    uint8_t master_mask = read(Port::MASTER_DATA);
    uint8_t slave_mask = read(Port::SLAVE_DATA);

    // Start initialization sequence
    write(Port::MASTER_COMMAND,
          static_cast<uint8_t>(ICW1::INITIALIZATION | ICW1::ICW4_NEEDED));
    write(Port::SLAVE_COMMAND,
          static_cast<uint8_t>(ICW1::INITIALIZATION | ICW1::ICW4_NEEDED));

    // Set vector offsets
    write(Port::MASTER_DATA, master_offset);
    write(Port::SLAVE_DATA, slave_offset);

    // Tell master PIC there's a slave at IRQ2
    write(Port::MASTER_DATA, 0x04);
    // Tell slave PIC its cascade identity
    write(Port::SLAVE_DATA, 0x02);

    // Set operation modes
    write(Port::MASTER_DATA, static_cast<uint8_t>(ICW4::MODE_8086));
    write(Port::SLAVE_DATA, static_cast<uint8_t>(ICW4::MODE_8086));

    // Restore saved masks
    write(Port::MASTER_DATA, master_mask);
    write(Port::SLAVE_DATA, slave_mask);
}

void Disable() {
    // Mask all interrupts
    write(Port::MASTER_DATA, 0xFF);
    write(Port::SLAVE_DATA, 0xFF);
}

void SendEOI(uint8_t irq) {
    if (irq >= 8) {
        write(Port::SLAVE_COMMAND, PIC_EOI);
    }
    write(Port::MASTER_COMMAND, PIC_EOI);
}

void MaskIRQ(uint8_t irq, bool masked) {
    const Port p = irq < 8 ? Port::MASTER_DATA : Port::SLAVE_DATA;
    const uint8_t bit = 1 << (irq % 8);

    uint8_t current = read(p);
    if (masked) {
        current |= bit;
    } else {
        current &= ~bit;
    }
    write(p, current);
}

void SetMask(uint16_t mask) {
    write(Port::MASTER_DATA, static_cast<uint8_t>(mask & 0xFF));
    write(Port::SLAVE_DATA, static_cast<uint8_t>((mask >> 8) & 0xFF));
}

uint16_t GetMask() {
    uint16_t mask = read(Port::MASTER_DATA);
    mask |= static_cast<uint16_t>(read(Port::SLAVE_DATA)) << 8;
    return mask;
}
uint16_t GetIRR() { return get_irq_reg(OCW3::READ_IRR); }
uint16_t GetISR() { return get_irq_reg(OCW3::READ_ISR); }

} // namespace x86::pic
