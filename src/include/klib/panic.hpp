#pragma once
namespace klib {
[[noreturn]] void panic(const char *message);
[[noreturn]] void panic();
} // namespace klib
