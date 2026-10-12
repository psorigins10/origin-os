# Serial Driver Architecture

## 1. Overview

The ORIGIN OS serial driver provides serial communication through the COM1 UART interface on x86-64 systems. It enables the kernel to transmit diagnostic messages to a serial terminal, including the host terminal when running under QEMU.

The driver uses x86 port-mapped I/O to configure the UART and transmit data.

## 2. Architecture

The serial subsystem consists of three layers:

```text
Kernel
  |
  v
Serial Driver
  |
  v
x86 Port I/O
  |
  v
COM1 UART
  |
  v
Serial Output
```

### Kernel

The kernel invokes the serial driver's public interface to initialize serial communication and transmit messages.

### Serial Driver

The serial driver configures the UART, checks transmitter readiness, and sends characters. It abstracts UART register operations from the rest of the kernel.

### x86 Port I/O

The port I/O layer provides low-level access to x86 I/O ports through the `inb()` and `outb()` functions, implemented in assembly.

### COM1 UART

COM1 is the conventional first serial port, using the base I/O address `0x3F8`. The UART exposes control, status, and data registers through consecutive I/O port addresses.

## 3. Source Organization

| File | Responsibility |
|---|---|
| `drivers/serial/serial.c` | UART initialization and serial transmission |
| `include/origin/drivers/serial/serial.h` | Public serial driver interface |
| `arch/x86_64/drivers/port_io.asm` | Assembly implementation of port I/O operations |
| `include/origin/arch/x86_64/port_io.h` | Declarations for port I/O functions |

## 4. UART Initialization

The driver initializes COM1 through the following operations:

1. Disable UART interrupts.
2. Enable the Divisor Latch Access Bit (DLAB).
3. Configure the baud-rate divisor.
4. Disable DLAB and configure 8 data bits, no parity, and one stop bit (8-N-1).
5. Enable and configure the FIFO buffers.
6. Configure the modem control register.

The current implementation uses a divisor of `1`, corresponding to 115200 baud on a standard 16550-compatible UART with a 1.8432 MHz input clock.

The driver configures the UART for polling-based transmission rather than interrupt-driven communication.

## 5. Data Transmission

The driver transmits data through the UART's transmit holding register.

For each character:

1. Read the Line Status Register (LSR) at `COM1 + 5`.
2. Check bit 5 (`THRE`), which indicates that the transmitter holding register is empty.
3. Wait until the register is available.
4. Write the character to the data register at `COM1`.

String transmission repeats this process until the null terminator (`'\0'`) is reached.

The current implementation uses a busy-wait loop while checking transmitter readiness. It does not use interrupts, DMA, or transmit buffering managed by the driver.

## 6. Port I/O Interface

The serial driver uses two architecture-specific functions:

- `outb(uint16_t port, uint8_t value)`: writes one byte to an I/O port.
- `inb(uint16_t port)`: reads one byte from an I/O port.

These functions execute the x86 `out` and `in` instructions, respectively. They operate on the x86 I/O address space, which is separate from memory-mapped device registers.

Because these instructions require sufficient processor privilege, the operations must execute in an appropriate privileged context.

## 7. QEMU Integration

QEMU can redirect the emulated serial port to the host terminal using:

```bash
-serial stdio
```

This allows serial messages produced by the kernel to appear in the terminal running QEMU. The serial output path is independent of the framebuffer and graphical console.

The current implementation supports basic kernel diagnostic output through COM1. It does not yet provide receive handling, interrupt-driven I/O, multiple serial ports, or a general terminal subsystem.