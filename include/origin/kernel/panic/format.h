#ifndef ORIGIN_KERNEL_PANIC_FORMAT_H
#define ORIGIN_KERNEL_PANIC_FORMAT_H

#include <stdint.h>

void panic_write_hex64(const char *label, uint64_t value);

#endif