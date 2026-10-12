#ifndef ORIGIN_ARCH_X86_64_TSS_H
#define ORIGIN_ARCH_X86_64_TSS_H

#include <stddef.h>
#include <stdint.h>

/*
 * ORIGIN OS — x86-64 Task State Segment (TSS).
 *
 * In 64-bit mode, the TSS provides:
 *   - RSP0-RSP2: stack pointers for privilege-level transitions.
 *   - IST1-IST7: dedicated stacks for selected interrupt handlers.
 *   - I/O map base: offset of the optional I/O permission bitmap.
 *
 * The processor expects this structure to be exactly 104 bytes.
 * The packed attribute prevents the compiler from inserting padding
 * that would change the hardware-defined layout.
 */
typedef struct {
    uint32_t reserved0;   /* Offset 0x00: reserved. */
    uint64_t rsp0;        /* Offset 0x04: Ring 0 stack pointer. */
    uint64_t rsp1;        /* Offset 0x0C: Ring 1 stack pointer. */
    uint64_t rsp2;        /* Offset 0x14: Ring 2 stack pointer. */
    uint64_t reserved1;   /* Offset 0x1C: reserved. */

    uint64_t ist1;        /* Offset 0x24: Interrupt Stack Table entry 1. */
    uint64_t ist2;        /* Offset 0x2C: Interrupt Stack Table entry 2. */
    uint64_t ist3;        /* Offset 0x34: Interrupt Stack Table entry 3. */
    uint64_t ist4;        /* Offset 0x3C: Interrupt Stack Table entry 4. */
    uint64_t ist5;        /* Offset 0x44: Interrupt Stack Table entry 5. */
    uint64_t ist6;        /* Offset 0x4C: Interrupt Stack Table entry 6. */
    uint64_t ist7;        /* Offset 0x54: Interrupt Stack Table entry 7. */

    uint64_t reserved2;   /* Offset 0x5C: reserved. */
    uint16_t reserved3;   /* Offset 0x64: reserved. */
    uint16_t iomap_base;  /* Offset 0x66: I/O permission bitmap offset. */
} __attribute__((packed)) ORIGIN_TSS;

/* Compile-time checks for the hardware-defined TSS layout. */
#if defined(__cplusplus)
#define ORIGIN_STATIC_ASSERT(expr, msg) static_assert((expr), msg)
#else
#define ORIGIN_STATIC_ASSERT(expr, msg) _Static_assert((expr), msg)
#endif

ORIGIN_STATIC_ASSERT(sizeof(ORIGIN_TSS) == 104,
    "64-bit TSS must be 104 bytes");

ORIGIN_STATIC_ASSERT(offsetof(ORIGIN_TSS, rsp0) == 4,
    "TSS rsp0 offset is incorrect");

ORIGIN_STATIC_ASSERT(offsetof(ORIGIN_TSS, ist1) == 36,
    "TSS ist1 offset is incorrect");

ORIGIN_STATIC_ASSERT(offsetof(ORIGIN_TSS, iomap_base) == 102,
    "TSS iomap_base offset is incorrect");

#endif /* ORIGIN_ARCH_X86_64_TSS_H */