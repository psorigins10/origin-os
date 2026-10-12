#ifndef ORIGIN_ARCH_X86_64_IDT_H
#define ORIGIN_ARCH_X86_64_IDT_H

#include <stdint.h>

/* IDT configuration */
#define ORIGIN_IDT_ENTRIES 256

/* Kernel code segment selector from our GDT */
#define ORIGIN_GDT_KERNEL_CODE_SELECTOR 0x08

/* Gate types */
#define ORIGIN_IDT_INTERRUPT_GATE 0x0E
#define ORIGIN_IDT_TRAP_GATE      0x0F

/* Present bit and ring-0 privilege level */
#define ORIGIN_IDT_PRESENT        0x80
#define ORIGIN_IDT_DPL0           0x00

/* One 64-bit IDT gate descriptor: 16 bytes */
typedef struct {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t  ist;
    uint8_t  type_attributes;
    uint16_t offset_middle;
    uint32_t offset_high;
    uint32_t reserved;
} __attribute__((packed)) IDT_GATE;

/* IDT register operand for lidt: 10 bytes in 64-bit mode */
typedef struct {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) IDTR;

_Static_assert(sizeof(IDT_GATE) == 16,
               "IDT gate must be 16 bytes");

_Static_assert(sizeof(IDTR) == 10,
               "IDTR must be 10 bytes");

/* IDT initialization */
void idt_init(void);

#endif