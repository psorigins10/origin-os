# Serial Driver

## Overview

The ORIGIN OS serial driver provides polling-based serial transmission through the COM1 UART interface on x86-64 systems. It uses x86 port-mapped I/O to configure the UART and transmit characters.

The driver currently supports initialization and transmission of null-terminated strings or individual characters. Serial output can be redirected to the host terminal when running under QEMU.

## Documentation

- [Architecture](architecture.md) — Driver layers, UART configuration, port I/O, and transmission flow.
- [API Reference](api.md) — Public functions, parameters, behavior, and limitations.
- [Testing](testing.md) — Build instructions, QEMU configuration, verification procedures, and troubleshooting.

## Implementation

| File | Description |
|---|---|
| `drivers/serial/serial.c` | COM1 initialization and transmission |
| `include/origin/drivers/serial/serial.h` | Public serial driver declarations |
| `arch/x86_64/drivers/port_io.asm` | x86 port I/O instruction implementations |
| `include/origin/arch/x86_64/port_io.h` | Port I/O function declarations |

## Configuration

| Property | Value |
|---|---|
| Architecture | x86-64 |
| UART | COM1 |
| Base I/O address | `0x3F8` |
| Baud rate | 115200 |
| Format | 8-N-1 |
| Transmission | Polling |
| QEMU output | `-serial stdio` |

## Limitations

- COM1 only.
- Transmission only; reception is not implemented.
- Busy-wait polling without a timeout.
- No software transmit queue or interrupt-driven operation.

