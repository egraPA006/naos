#pragma once
#include <stdint.h>

namespace x86::pit {
constexpr uint32_t PIT_BASE_FREQUENCY = 1193182; // 1.193182 MHz
constexpr uint16_t MAX_COUNT = 0xFFFF;
enum class Ports : uint16_t {
    CH0 = 0x40,
    CH1 = 0x41,
    CH2 = 0x42,
    MODE_CMD = 0x43
};
enum class Op_mode : uint8_t {
    M_0 = 0, // interrupt on terminal count
    M_1 = 1, // hardware re-triggerable one-shot
    M_2 = 2, // rate generator
    M_3 = 3, // square wave generator
    M_4 = 4, // software triggered strobe
    M_5 = 5  // hardware triggered strobe
};
enum class Access_mode : uint8_t {
    LATCH_COUNT_VALUE = 0,  // Latch count value command
    ACCESS_LOBYTE = 1,      // Access mode: lobyte only
    ACCESS_HIBYTE = 2,      // Access mode: hibyte only
    ACESS_LOBYTE_HIBYTE = 3 // Access mode: lobyte/hibyte
};
enum class Channel : uint8_t {
    CH0 = 0,
    CH1 = 1,
    CH2 = 2,
    READ_BACK = 3 // Read-back command (8254 only)
};
struct [[gnu::aligned]] Mode_Cmd {
    bool bcd_bin : 1; // 0 = 16-bit binary, 1 = four-digit BCD
    Op_mode op_mode : 3;
    Access_mode access_mode : 2;
    Channel sel_ch : 2;
};

// Initialize PIT with given frequency (Hz)
void initialize(uint32_t frequency_hz);

// Set frequency for a specific channel
void set_frequency(Channel channel, uint32_t frequency_hz);

// Read current count from a channel
uint16_t read_count(Channel channel);

// Send mode command to PIT
void write_mode_command(Mode_Cmd cmd);

// Write count value to a channel
void write_count(Channel channel, uint16_t count);

// Wait for a specific number of ticks
void wait_ticks(uint16_t ticks);

// Short delay in milliseconds (using PIT)
void sleep_ms(uint32_t milliseconds);

// For channel 0 interrupt handling:
void set_interrupt_handler(void (*handler)());
void acknowledge_interrupt();

// Read-back command (8254 specific)
void send_read_back_command(bool read_count, bool read_status, Channel channel);

} // namespace x86::pit
