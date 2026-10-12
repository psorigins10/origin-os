#ifndef SERIAL_H
#define SERIAL_H

/*
 * Initialize the serial port (COM1).
 *
 * Configures the UART so the kernel can transmit
 * characters through the serial port.
 */
void serial_init(void);

/*
 * Transmit a single character through the serial port.
 *
 * Waits until the UART is ready to accept a character,
 * then writes the character to its transmit register.
 *
 * @param character The character to transmit.
 */
void serial_write_char(const char character);

/*
 * Transmit a null-terminated string through the serial port.
 *
 * Sends each character in the message until the
 * null terminator ('\0') is reached.
 *
 * @param message Pointer to the string to transmit.
 */
void serial_write(const char *message);

#endif
