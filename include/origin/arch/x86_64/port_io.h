#ifndef ORIGIN_ARCH_X86_64_PORT_IO_H
#define ORIGIN_ARCH_X86_64_PORT_IO_H

#include <stdint.h>

/*
 * Write one byte to an x86 I/O port.
 *
 * @param port  The I/O port address to write to.
 * @param value The 8-bit value to send to the port.
 */
void outb(uint16_t port, uint8_t value);

/*
 * Read one byte from an x86 I/O port.
 *
 * @param port The I/O port address to read from.
 * @return The 8-bit value read from the port.
 */
uint8_t inb(uint16_t port);

#endif