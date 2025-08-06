#pragma once

namespace klib {
namespace __printk {
enum log_level {
    ERROR = 0,
    WARN = 1,
    INFO = 2,
    DEBUG = 3,
};
void set_log_level(log_level level);
}; // namespace __printk

void printk(__printk::log_level level, const char *format, ...)
    __attribute__((format(printf, 2, 3)));

#define perror(fmt, ...)                                                       \
    printk(klib::__printk::ERROR, "%s: " fmt, __func__, ##__VA_ARGS__)
#define pwarn(fmt, ...)                                                        \
    printk(klib::__printk::WARN, "%s: " fmt, __func__, ##__VA_ARGS__)
#define pinfo(fmt, ...)                                                        \
    printk(klib::__printk::INFO, "%s: " fmt, __func__, ##__VA_ARGS__)
#define pdebug(fmt, ...)                                                       \
    printk(klib::__printk::DEBUG, "%s: " fmt, __func__, ##__VA_ARGS__)
}; // namespace klib
