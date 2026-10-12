#ifndef REGISTER_CONTEXT_H
#define REGISTER_CONTEXT_H

#include <stdint.h>
#include <stddef.h>

/*
 * ORIGIN OS — CPU Register Context
 *
 * Defines the data structure used to represent a snapshot of CPU state.
 * Assembly code will eventually populate this structure, and the C panic
 * reporter will use it to display diagnostic information.
 *
 * IMPORTANT:
 * A software panic snapshot is not necessarily the same as the CPU state
 * at the original fault. CPU exception handlers must save the interrupted
 * state at exception entry.
 */

/*
 * General-purpose registers.
 *
 * Each field represents a 64-bit x86-64 general-purpose register.
 * The field order is intentional: assembly code must use matching offsets
 * when saving register values into this structure.
 */
typedef struct origin_general_purpose_registers {
    uint64_t RAX;   /* Accumulator; commonly used for results and arguments. */
    uint64_t RBX;   /* General-purpose register; callee-saved in the System V ABI. */
    uint64_t RCX;   /* General-purpose register; also used by some instructions. */
    uint64_t RDX;   /* General-purpose register; also used for data and results. */
    uint64_t RSI;   /* Source index; also used for function arguments. */
    uint64_t RDI;   /* Destination index; also used for function arguments. */
    uint64_t RBP;   /* Commonly used as a stack-frame base pointer. */
    uint64_t R8;    /* Additional x86-64 general-purpose register. */
    uint64_t R9;    /* Additional x86-64 general-purpose register. */
    uint64_t R10;   /* Additional x86-64 general-purpose register. */
    uint64_t R11;   /* Additional x86-64 general-purpose register. */
    uint64_t R12;   /* Additional x86-64 general-purpose register. */
    uint64_t R13;   /* Additional x86-64 general-purpose register. */
    uint64_t R14;   /* Additional x86-64 general-purpose register. */
    uint64_t R15;   /* Additional x86-64 general-purpose register. */
} ORIGIN_GENERAL_PURPOSE_REGISTERS;

/*
 * Stack and execution state.
 *
 * RSP    — stack pointer value being reported.
 * RIP    — instruction pointer value being reported.
 * RFLAGS — CPU flags register value being reported.
 *
 * The capture mechanism must define exactly which moment each value
 * represents. Reading these values inside ordinary C code may be too late
 * to recover the state from before a fault.
 */
typedef struct origin_stack_and_execution {
    uint64_t RSP;
    uint64_t RIP;
    uint64_t RFLAGS;
} ORIGIN_STACK_AND_EXECUTION;

/*
 * Complete register context passed to the panic-reporting code.
 *
 * exception_vector identifies the CPU exception when this context came
 * from an exception handler.
 *
 * error_code stores the exception error code when applicable. Not every
 * CPU exception supplies one, so the assembly entry path must define a
 * consistent representation for exceptions without an error code.
 *
 * For a software-triggered panic, exception metadata must not be presented
 * as valid exception information unless it was explicitly populated.
 */
typedef struct {
    ORIGIN_GENERAL_PURPOSE_REGISTERS general_purpose_registers;
    ORIGIN_STACK_AND_EXECUTION stack_and_execution;

    uint64_t exception_vector;
    uint64_t error_code;
} ORIGIN_REGISTER_CONTEXT;

/*
 * Compile-time layout verification.
 *
 * These checks protect the interface between C and assembly. If a field
 * moves or the structure size changes, the compiler reports an error
 * instead of allowing assembly to silently write to incorrect offsets.
 *
 * The expected layout assumes the x86-64 target and the field definitions
 * above. Keep these checks synchronized with any future structure changes.
 */
#if defined(__cplusplus)

static_assert(
    sizeof(ORIGIN_REGISTER_CONTEXT) == 160,
    "Unexpected ORIGIN_REGISTER_CONTEXT size"
);

static_assert(
    offsetof(ORIGIN_REGISTER_CONTEXT, general_purpose_registers) == 0,
    "Unexpected general-purpose register offset"
);

static_assert(
    offsetof(ORIGIN_REGISTER_CONTEXT, stack_and_execution) == 120,
    "Unexpected stack/execution offset"
);

static_assert(
    offsetof(ORIGIN_REGISTER_CONTEXT, exception_vector) == 144,
    "Unexpected exception_vector offset"
);

static_assert(
    offsetof(ORIGIN_REGISTER_CONTEXT, error_code) == 152,
    "Unexpected error_code offset"
);

#else

_Static_assert(
    sizeof(ORIGIN_REGISTER_CONTEXT) == 160,
    "Unexpected ORIGIN_REGISTER_CONTEXT size"
);

_Static_assert(
    offsetof(ORIGIN_REGISTER_CONTEXT, general_purpose_registers) == 0,
    "Unexpected general-purpose register offset"
);

_Static_assert(
    offsetof(ORIGIN_REGISTER_CONTEXT, stack_and_execution) == 120,
    "Unexpected stack/execution offset"
);

_Static_assert(
    offsetof(ORIGIN_REGISTER_CONTEXT, exception_vector) == 144,
    "Unexpected exception_vector offset"
);

_Static_assert(
    offsetof(ORIGIN_REGISTER_CONTEXT, error_code) == 152,
    "Unexpected error_code offset"
);

#endif

void register_capture(ORIGIN_REGISTER_CONTEXT *context);

#endif