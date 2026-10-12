/*
 * ORIGIN OS — Panic formatting utilities.
 *
 * Provides low-level formatting helpers for panic and fatal-error
 * diagnostics. Output is sent directly to the serial port so it
 * remains useful when the graphical console is unavailable.
 */

#include <stdint.h>

#include "origin/kernel/panic/format.h"
#include "origin/drivers/serial/serial.h"

/*
 * Write a label and a 64-bit value in hexadecimal to the serial port.
 *
 * The value is formatted as:
 *     label0x0123456789ABCDEF
 *
 * The fixed-width representation always contains 16 hexadecimal
 * digits, including leading zeroes.
 */
void panic_write_hex64(const char *label, uint64_t value)
{
    static const char hex_digits[] = "0123456789ABCDEF";

    /* "0x" + 16 hexadecimal digits + terminating null byte. */
    char buffer[19];

    buffer[0] = '0';
    buffer[1] = 'x';

    /*
     * Extract each hexadecimal digit from most significant to least
     * significant, shifting by four bits for each digit.
     */
    for (unsigned int i = 0; i < 16; ++i)
    {
        unsigned int shift = 60 - (i * 4);
        unsigned int digit = (unsigned int)((value >> shift) & 0xF);

        buffer[i + 2] = hex_digits[digit];
    }

    buffer[18] = '\0';

    /* Keep panic output independent of the graphical console. */
    serial_write(label);
    serial_write(buffer);
    serial_write("\n");
}