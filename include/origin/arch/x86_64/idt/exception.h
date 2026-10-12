#ifndef ORIGIN_ARCH_X86_64_IDT_EXCEPTION_H
#define ORIGIN_ARCH_X86_64_IDT_EXCEPTION_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
#define ORIGIN_STATIC_ASSERT(cond, msg) static_assert((cond), msg)
#else
#define ORIGIN_STATIC_ASSERT(cond, msg) _Static_assert((cond), msg)
#endif

typedef struct {
    /* General-purpose registers: reverse push order */
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t rbp;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;

    /* Added by the assembly stub */
    uint64_t vector;
    uint64_t error_code;

    /* Saved by the CPU */
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;

    uint64_t cr2;
} ORIGIN_EXCEPTION_FRAME;

ORIGIN_STATIC_ASSERT(offsetof(ORIGIN_EXCEPTION_FRAME, r15) == 0,
                    "r15 offset incorrect");

ORIGIN_STATIC_ASSERT(offsetof(ORIGIN_EXCEPTION_FRAME, rax) == 14 * 8,
                    "rax offset incorrect");

ORIGIN_STATIC_ASSERT(offsetof(ORIGIN_EXCEPTION_FRAME, vector) == 15 * 8,
                    "vector offset incorrect");

ORIGIN_STATIC_ASSERT(offsetof(ORIGIN_EXCEPTION_FRAME, error_code) == 16 * 8,
                    "error_code offset incorrect");

ORIGIN_STATIC_ASSERT(offsetof(ORIGIN_EXCEPTION_FRAME, rip) == 17 * 8,
                    "rip offset incorrect");
                
ORIGIN_STATIC_ASSERT(offsetof(ORIGIN_EXCEPTION_FRAME, cs) == 18 * 8,
                    "cs offset incorrect");

ORIGIN_STATIC_ASSERT(offsetof(ORIGIN_EXCEPTION_FRAME, rflags) == 19 * 8,
                    "rflags offset incorrect");

ORIGIN_STATIC_ASSERT(offsetof(ORIGIN_EXCEPTION_FRAME, cr2) == 20 * 8,
                    "cr2 offset incorrect");

ORIGIN_STATIC_ASSERT(sizeof(ORIGIN_EXCEPTION_FRAME) == 21 * 8,
                    "exception frame size incorrect");
#endif