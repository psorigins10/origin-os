/*
 * ORIGIN OS — Kernel panic handler.
 *
 * Reports a fatal kernel error through the serial port, captures
 * the processor's register context, and halts the system.
 *
 * This path avoids relying on the graphical console because a panic
 * may occur before the console is initialized or when it is unusable.
 */

#include <stdint.h>

#include "origin/kernel/panic/panic.h"
#include "origin/kernel/panic/format.h"
#include "origin/kernel/panic/register_context.h"
#include "origin/drivers/serial/serial.h"

/* Implemented in assembly; saves the processor context into the buffer. */
extern void register_capture(ORIGIN_REGISTER_CONTEXT *context);

/*
 * Stop the processor permanently after reporting a fatal error.
 * Interrupts are disabled before this function is entered.
 */
static ORIGIN_NORETURN void panic_halt(void)
{
    for (;;)
        __asm__ volatile ("cli; hlt");
}

/*
 * Report a fatal kernel error.
 *
 * message may be NULL. Register capture and diagnostic output are
 * performed before the processor is halted.
 */
ORIGIN_NORETURN void panic(const char *message)
{
    /* Prevent maskable interrupts from interrupting panic reporting. */
    __asm__ volatile ("cli" ::: "memory");

    serial_write("\n");
    serial_write("========================================\n");
    serial_write("       ORIGIN OS - KERNEL PANIC\n");
    serial_write("========================================\n\n");

    serial_write("Reason: ");

    if (message != NULL)
    {
        serial_write(message);
    }
    else
    {
        serial_write("(no reason provided)");
    }

    serial_write("\n\n");

    /*
     * Capture the CPU state before performing further diagnostic work.
     * The assembly routine must match the context structure's layout.
     */
    ORIGIN_REGISTER_CONTEXT context;
    register_capture(&context);

    serial_write("Register snapshot:\n");

    /* General-purpose registers. */
    panic_write_hex64("RAX:    ", context.general_purpose_registers.RAX);
    panic_write_hex64("RBX:    ", context.general_purpose_registers.RBX);
    panic_write_hex64("RCX:    ", context.general_purpose_registers.RCX);
    panic_write_hex64("RDX:    ", context.general_purpose_registers.RDX);
    panic_write_hex64("RSI:    ", context.general_purpose_registers.RSI);
    panic_write_hex64("RDI:    ", context.general_purpose_registers.RDI);
    panic_write_hex64("RBP:    ", context.general_purpose_registers.RBP);
    panic_write_hex64("R8:     ", context.general_purpose_registers.R8);
    panic_write_hex64("R9:     ", context.general_purpose_registers.R9);
    panic_write_hex64("R10:    ", context.general_purpose_registers.R10);
    panic_write_hex64("R11:    ", context.general_purpose_registers.R11);
    panic_write_hex64("R12:    ", context.general_purpose_registers.R12);
    panic_write_hex64("R13:    ", context.general_purpose_registers.R13);
    panic_write_hex64("R14:    ", context.general_purpose_registers.R14);
    panic_write_hex64("R15:    ", context.general_purpose_registers.R15);

    /* Stack and execution state supplied by the capture routine. */
    panic_write_hex64("RSP:    ", context.stack_and_execution.RSP);
    panic_write_hex64("RIP:    ", context.stack_and_execution.RIP);
    panic_write_hex64("RFLAGS: ", context.stack_and_execution.RFLAGS);

    /*
     * UINT64_MAX indicates that exception metadata is unavailable.
     * Otherwise, report the vector and associated error code.
     */
    if (context.exception_vector != UINT64_MAX)
    {
        panic_write_hex64(
            "Exception vector: ",
            context.exception_vector
        );

        panic_write_hex64(
            "Error code:       ",
            context.error_code
        );
    }
    else
    {
        serial_write("Exception metadata: unavailable\n");
    }

    serial_write("\nSystem halted.\n");

    panic_halt();
}