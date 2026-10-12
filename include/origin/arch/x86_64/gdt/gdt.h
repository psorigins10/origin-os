#ifndef ORIGIN_ARCH_X86_64_GDT_H
#define ORIGIN_ARCH_X86_64_GDT_H

#include <stdint.h>

#define ORIGIN_GDT_KERNEL_CODE_SELECTOR 0x08
#define ORIGIN_GDT_KERNEL_DATA_SELECTOR 0x10

/* Initialize and load ORIGIN OS's GDT. */
void gdt_init(void);

#endif