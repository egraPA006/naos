#include "arch/x86/pit.hpp"
#include "arch/x86/io.hpp"

namespace x86::pit {
void initialize(uint32_t frequency_hz) {
    // Avoid division by zero and too high frequencies
    if (frequency_hz == 0)
        frequency_hz = 1;
    if (frequency_hz > PIT_BASE_FREQUENCY)
        frequency_hz = PIT_BASE_FREQUENCY;

    uint16_t divisor = static_cast<uint16_t>(PIT_BASE_FREQUENCY / frequency_hz);

    Mode_Cmd cmd;
    cmd.bcd_bin = false;        // Binary mode
    cmd.op_mode = Op_mode::M_3; // Square wave generator
    cmd.access_mode = Access_mode::ACESS_LOBYTE_HIBYTE;
    cmd.sel_ch = Channel::CH0;

    write_mode_command(cmd);
    write_count(Channel::CH0, divisor);
}

void set_frequency(Channel channel, uint32_t frequency_hz) {
    if (frequency_hz == 0)
        frequency_hz = 1;
    if (frequency_hz > PIT_BASE_FREQUENCY)
        frequency_hz = PIT_BASE_FREQUENCY;

    uint16_t divisor = static_cast<uint16_t>(PIT_BASE_FREQUENCY / frequency_hz);
    write_count(channel, divisor);
}

void write_mode_command(Mode_Cmd cmd) {
    uint8_t command_byte = (static_cast<uint8_t>(cmd.bcd_bin) << 0) |
                           (static_cast<uint8_t>(cmd.op_mode) << 1) |
                           (static_cast<uint8_t>(cmd.access_mode) << 4) |
                           (static_cast<uint8_t>(cmd.sel_ch) << 6);

    io::outb(static_cast<uint16_t>(Ports::MODE_CMD), command_byte);
}
void write_count(Channel channel, uint16_t count) {
    const uint16_t port =
        static_cast<uint16_t>(Ports::CH0) + static_cast<uint16_t>(channel);

    // Write LSB then MSB
    io::outb(port, static_cast<uint8_t>(count & 0xFF));
    io::outb(port, static_cast<uint8_t>((count >> 8) & 0xFF));
}

uint16_t read_count(Channel channel) {
    // Send latch command
    Mode_Cmd latch_cmd;
    latch_cmd.bcd_bin = false;
    latch_cmd.op_mode = Op_mode::M_0; // Doesn't matter for latch
    latch_cmd.access_mode = Access_mode::LATCH_COUNT_VALUE;
    latch_cmd.sel_ch = channel;
    write_mode_command(latch_cmd);

    const uint16_t port =
        static_cast<uint16_t>(Ports::CH0) + static_cast<uint16_t>(channel);

    // Read LSB then MSB
    uint8_t lo = io::inb(port);
    uint8_t hi = io::inb(port);

    return (static_cast<uint16_t>(hi) << 8) | lo;
}

void wait_ticks(uint16_t ticks) {
    write_count(Channel::CH0, ticks);

    // Wait for the count to reach 0
    while ((read_count(Channel::CH0) & 0xFFFF) != 0) {
        asm volatile("pause");
    }
}

} // namespace x86::pit
