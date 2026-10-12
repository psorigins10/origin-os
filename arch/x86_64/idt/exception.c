/*
 * ORIGIN OS — x86-64 exception dispatcher.
 *
 * Receives the register frame prepared by the assembly exception
 * handlers, prints essential diagnostic information, and halts.
 *
 * This implementation is intentionally minimal because exceptions
 * may occur while other kernel subsystems are already failing.
 */

#include <stdint.h>

#include <origin/arch/x86_64/idt/exception.h>
#include <origin/printk/printk.h>

/*
 * Format a 64-bit value as a fixed-width hexadecimal string.
 *
 * Output format: 0x0123456789ABCDEF
 *
 * The destination must provide at least 19 bytes:
 * two prefix characters, 16 hexadecimal digits, and '\0'.
 */
static void format_hex64(char buffer[19], uint64_t value)
{
    static const char digits[] = "0123456789ABCDEF";

    buffer[0] = '0';
    buffer[1] = 'x';

    for (unsigned int i = 0; i < 16; ++i)
    {
        unsigned int shift = (15U - i) * 4U;
        unsigned int digit = (unsigned int)((value >> shift) & 0xF);

        buffer[2 + i] = digits[digit];
    }

    buffer[18] = '\0';
}

/*
 * Handle a CPU exception.
 *
 * The assembly entry stubs construct ORIGIN_EXCEPTION_FRAME and pass
 * its address according to the x86-64 calling convention.
 *
 * This dispatcher does not return: it reports the exception and
 * leaves the processor halted for debugging.
 */
void exception_dispatcher(ORIGIN_EXCEPTION_FRAME *frame)
{
    char vector_text[19];
    char error_text[19];
    char rip_text[19];

    /*
     * A null frame indicates a broken exception-entry path.
     * Avoid dereferencing it; halt without attempting further output.
     */
    if (frame == NULL)
    {
        for (;;)
            __asm__ volatile ("cli; hlt");
    }

    format_hex64(vector_text, frame->vector);
    format_hex64(error_text, frame->error_code);
    format_hex64(rip_text, frame->rip);

    printk("ORIGIN OS: FATAL EXCEPTION\n");
    printk("Vector: ");
    printk(vector_text);
    printk("\nError code: ");
    printk(error_text);
    printk("\nRIP: ");
    printk(rip_text);
    printk("\n");

    /* Fatal exceptions stop normal kernel execution. */
    for (;;)
        __asm__ volatile ("cli; hlt");
}