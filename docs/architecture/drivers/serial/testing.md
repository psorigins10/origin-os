# Serial Driver Testing

## 1. Test Environment

The serial driver is tested using QEMU with OVMF firmware and an emulated x86-64 system.

QEMU is configured to redirect the emulated serial port to the host terminal using `-serial stdio`.

## 2. Build and Run

From the repository root, execute:

```bash
make clean
make run
```

The build compiles the serial driver, assembles the x86 port I/O routines, links the kernel, and prepares the bootable image.

The `make run` target launches QEMU through `scripts/run-qemu.sh`.

The QEMU command must include:

```bash
-serial stdio
```

## 3. Transmission Test

Temporarily add the following code to the kernel initialization path after boot information validation:

```c
serial_init();
serial_write("ORIGIN SERIAL TEST\n");
serial_write_char('A');
```

Ensure the serial header is included:

```c
#include "origin/drivers/serial/serial.h"
```

Rebuild and launch the system:

```bash
make clean
make run
```

### Expected Result

The terminal running QEMU should display:

```text
ORIGIN SERIAL TEST
A
```

The output confirms that the kernel initialized the serial driver and transmitted characters through the emulated COM1 UART.

Remove the temporary test code or replace it with the intended kernel diagnostic output after verification.

## 4. Port I/O Symbol Verification

Verify that the kernel ELF contains the port I/O and serial driver symbols:

```bash
nm kernel/build/bin/kernel.elf | grep -E ' (outb|inb|serial_init|serial_write_char|serial_write)$'
```

The expected symbols are:

- `outb`
- `inb`
- `serial_init`
- `serial_write_char`
- `serial_write`

A `T` symbol type indicates that the symbol is defined in the executable's text section.

## 5. Failure Diagnosis

| Symptom | Check |
|---|---|
| No serial output | Confirm `-serial stdio` is present in the QEMU command |
| Output is missing or incomplete | Confirm `serial_init()` executes before transmission |
| Serial symbols are absent | Verify the source files are included in the Makefile and linked into the kernel |
| Kernel does not boot | Inspect the build output and QEMU terminal for boot errors |
| Output appears only in the host terminal | Expected behavior; serial output and framebuffer output are separate paths |

## 6. Test Scope and Limitations

These tests verify that the driver builds, links, initializes under QEMU, and transmits characters through the emulated serial port.

They do not verify physical UART hardware, serial reception, interrupt-driven operation, timeout handling, or transmission reliability under sustained load.