#include "origin/drivers/serial/serial.h"
#include "origin/arch/x86_64/port_io.h"

// COM1's base I/O port address.
#define COM1_BASE 0x3F8

void serial_init(void) {
    // Disable all UART interrupts during initialization.
    outb(COM1_BASE + 1, 0);

    // Set DLAB (Divisor Latch Access Bit) to access the baud-rate divisor registers.
    outb(COM1_BASE + 3, 0x80);

    // Set the baud-rate divisor to 1 (115200 baud with a conventional UART clock).
    outb(COM1_BASE + 0, 1); // Divisor low byte.
    outb(COM1_BASE + 1, 0); // Divisor high byte.

    // Configure 8 data bits, no parity, and 1 stop bit (8-N-1).
    // This also clears DLAB, restoring normal register access.
    outb(COM1_BASE + 3, 0x03);

    // Enable the FIFO and clear its transmit and receive buffers.
    outb(COM1_BASE + 2, 0xC7);

    // Enable DTR, RTS, and OUT2 in the modem-control register.
    outb(COM1_BASE + 4, 0x0B);
}

void serial_write_char(const char character) {
    while((inb(COM1_BASE + 5) & 0x20) == 0) {
        // Wait until the UART can accept another character.
    }
    outb(COM1_BASE, (uint8_t)character);
}

void serial_write(const char *message) {
    while(*message != '\0') {
        serial_write_char(*message);
        message++;
    }
}