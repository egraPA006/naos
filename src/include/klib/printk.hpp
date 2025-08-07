#pragma once
namespace klib {
namespace __printk {
enum log_level {
    ERROR = 0,
    WARN = 1,
    SUCCESS = 2,
    INFO = 3,
    DEBUG = 4,
};
void set_log_level(log_level level);
}; // namespace __printk

void printk(__printk::log_level level, const char *format, ...)
    __attribute__((format(printf, 2, 3)));

void printk(const char *format, ...) __attribute__((format(printf, 1, 2)));

#define kerror(fmt, ...)                                                       \
    klib::printk(klib::__printk::ERROR, "%s: " fmt, __func__, ##__VA_ARGS__)
#define kwarn(fmt, ...)                                                        \
    klib::printk(klib::__printk::WARN, "%s: " fmt, __func__, ##__VA_ARGS__)
#define ksuccess(fmt, ...)                                                     \
    klib::printk(klib::__printk::SUCCESS, "%s: " fmt, __func__, ##__VA_ARGS__)
#define kinfo(fmt, ...)                                                        \
    klib::printk(klib::__printk::INFO, "%s: " fmt, __func__, ##__VA_ARGS__)
#define kdebug(fmt, ...)                                                       \
    klib::printk(klib::__printk::DEBUG, "%s: " fmt, __func__, ##__VA_ARGS__)
}; // namespace klib
