# OriginBoot

OriginBoot is the UEFI bootloader for Origin OS.

It is responsible for loading the Origin OS kernel from the EFI filesystem,
parsing its ELF image, loading its segments into memory, and transferring
control to the kernel.

## Current Status

OriginBoot currently operates as a UEFI application using gnu-efi.

It can:

- Initialize the gnu-efi environment
- Locate the EFI filesystem
- Open and read `kernel.elf`
- Validate the kernel ELF image
- Parse ELF program headers
- Load `PT_LOAD` segments into memory
- Obtain the kernel entry point
- Transfer execution to the Origin OS kernel

The current kernel is a minimal x86-64 freestanding kernel that prints:

```text
Welcome to ORIGIN OS
```

through the serial port.

## Current Boot Flow

```text
UEFI Firmware
     ↓
EFI/BOOT/BOOTX64.EFI
     ↓
efi_main()
     ↓
Initialize gnu-efi
     ↓
Locate EFI filesystem
     ↓
Open kernel.elf
     ↓
Read kernel into memory
     ↓
Validate ELF header
     ↓
Parse ELF program headers
     ↓
Load PT_LOAD segments
     ↓
Obtain kernel entry point
     ↓
Jump to kernel
     ↓
ORIGIN OS kernel
```

## Project Structure

```text
bootloader/
├── include/
│   ├── console.h
│   ├── elf.h
│   ├── file.h
│   └── memory.h
├── scripts/
│   ├── mkimage.sh
│   └── run-qemu.sh
├── src/
│   ├── console.c
│   ├── elf.c
│   ├── file.c
│   ├── main.c
│   └── memory.c
├── Makefile
└── README.md
```

## Building

From the `bootloader` directory:

```bash
make
```

For a clean rebuild:

```bash
make clean && make
```

The complete Origin OS system can also be built from the repository root:

```bash
make
```

## Running

From the `bootloader` directory:

```bash
./scripts/run-qemu.sh
```

From the repository root:

```bash
make run
```

QEMU uses OVMF for UEFI firmware and the serial console for kernel output.

## Kernel Loading

OriginBoot expects the kernel to be available on the EFI filesystem as:

```text
/kernel.elf
```

The kernel is currently loaded as an ELF64 executable.

For each `PT_LOAD` segment, OriginBoot:

1. Locates the segment in the loaded ELF image.
2. Copies it to its physical address.
3. Clears the remaining memory described by `p_memsz`.
4. Processes all loadable segments.
5. Uses the ELF entry point as the kernel entry address.

## Current Limitations

The bootloader is intentionally minimal and is still under active
development.

Current limitations include:

- Limited ELF validation
- No complete UEFI memory-map handling
- No robust memory-management strategy
- No page-table setup
- No dedicated kernel stack setup
- No boot information structure passed to the kernel
- No `ExitBootServices()` transition yet
- Limited error handling
- Kernel and bootloader memory safety still needs to be improved

This implementation is intended for learning and research, not production
use.

## Roadmap

The long-term goal for OriginBoot is to:

- Access the UEFI filesystem
- Locate the Origin OS kernel
- Read the kernel from disk
- Validate and parse the kernel ELF
- Allocate and prepare memory
- Load the kernel into memory
- Collect the UEFI memory map
- Create boot information for the kernel
- Set up the initial kernel execution environment
- Exit UEFI Boot Services
- Transfer control to the Origin OS kernel

## Development Philosophy

OriginBoot is being developed incrementally with an emphasis on understanding
the boot process rather than relying on a pre-built bootloader.

Each stage is implemented and tested before moving deeper into the boot
process.

The goal is to understand what happens between firmware startup and kernel
execution and eventually implement those stages directly in OriginBoot.

## Version

Current development stage: **Kernel ELF Loading and Handoff**

Target milestone: **OriginBoot v0.1**