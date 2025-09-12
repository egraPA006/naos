#pragma once
namespace klib {
[[noreturn]] void panic(const char *message);
[[noreturn, gnu::no_caller_saved_registers]] void panic();
} // namespace klib
