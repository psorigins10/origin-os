# Kernel Debugging

## 1. Overview

ORIGIN OS supports source-level kernel debugging using GNU Debugger (GDB) and QEMU's GDB remote debugging stub.

The debugging workflow allows developers to set breakpoints, inspect CPU registers and memory, examine stack frames, and step through kernel instructions.

## 2. Requirements

- QEMU with x86-64 system emulation
- GNU Debugger (GDB)
- GCC with debug information support
- A successfully built ORIGIN OS kernel and bootable ESP image

## 3. Debug Build

The kernel is compiled with the GCC `-g` option to generate debugging information. This allows GDB to associate machine instructions with source files, source lines, and function names.

The kernel ELF file is located at:

```text
kernel/build/bin/kernel.elf
```

Use the ELF file when loading kernel symbols into GDB. The ESP image contains the bootable files and is not a replacement for the kernel ELF's debugging information.

## 4. QEMU Debug Configuration

The debug launcher is:

```text
scripts/debug-qemu.sh
```

It uses the existing UEFI firmware, bootable ESP image, VGA configuration, and serial output configuration, with these additional QEMU options:

- `-s`: Opens the GDB remote debugging stub on TCP port `1234`.
- `-S`: Pauses the virtual CPU at startup until GDB resumes execution.

The CPU initially stops before the firmware and bootloader have completed execution. GDB can follow execution into the kernel after the bootloader transfers control to it.

## 5. Debugging Workflow

### Terminal 1: Start QEMU

From the repository root:

```bash
make debug
```

This builds the bootloader and kernel, creates the ESP image, and launches QEMU with the CPU paused.

### Terminal 2: Start GDB

```bash
make debug-gdb
```

The Makefile target runs:

```bash
gdb -x scripts/kernel.gdb kernel/build/bin/kernel.elf
```

The GDB command file contains:

```gdb
target remote :1234
break kernel_main
continue
```

GDB connects to QEMU, sets a breakpoint at `kernel_main`, and resumes execution. When the breakpoint is reached, execution stops at the kernel entry point.

## 6. Common GDB Commands

| Command | Description |
|---|---|
| `break kernel_main` | Set a breakpoint at `kernel_main` |
| `continue` | Resume execution until the next stop |
| `next` | Execute the next source line, stepping over function calls |
| `step` | Execute the next source line, entering function calls |
| `si` | Execute one machine instruction |
| `info registers` | Display CPU registers |
| `print variable` | Evaluate and display a variable |
| `x/8gx $rsp` | Examine eight 64-bit values starting at the stack pointer |
| `x/8i $rip` | Disassemble eight instructions starting at the instruction pointer |
| `bt` | Display the current backtrace |
| `quit` | Exit GDB |

## 7. Debugging Limitations

- The initial QEMU pause occurs in firmware execution, not at the kernel entry point.
- Kernel source-level debugging requires a matching ELF file containing debug information.
- The current setup loads kernel symbols but does not automatically load UEFI firmware or bootloader symbols.
- Source-level variable values can depend on compiler output and the current instruction location.
- A useful stack trace depends on valid stack state and suitable unwinding information.
- QEMU's GDB stub listens on TCP port `1234` by default when using `-s`. The port must be available, and the debug endpoint should not be exposed to untrusted networks.

