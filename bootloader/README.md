# OriginBoot

OriginBoot is the UEFI bootloader for Origin OS.

## Current Status

OriginBoot currently boots as a UEFI application.

At the moment, it initializes the gnu-efi environment, displays a
startup message, waits for keyboard input, and then returns control
to the UEFI firmware.

### Current Boot Flow

```text
UEFI Firmware
     ↓
EFI/BOOT/BOOTX64.EFI
     ↓
efi_main()
     ↓
Initialize gnu-efi
     ↓
Display startup message
     ↓
Wait for keyboard input
     ↓
Return to UEFI
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

This will:

1. Compile the bootloader source code.
2. Link the UEFI application.
3. Convert it into `BOOTX64.EFI`.
4. Update the FAT32 disk image.

For a completely clean rebuild:

```bash
make clean && make
```

## Running

Run OriginBoot in QEMU using OVMF:

```bash
./scripts/run-qemu.sh
```

## Requirements

OriginBoot currently requires:

- GCC
- GNU binutils
- gnu-efi
- mtools
- dosfstools
- QEMU
- OVMF

## Roadmap

The long-term goal is for OriginBoot to:

- Access the UEFI filesystem
- Locate the Origin OS kernel
- Read the kernel from disk
- Parse the kernel's ELF format
- Allocate and prepare memory
- Load the kernel into memory
- Create boot information for the kernel
- Exit UEFI boot services
- Transfer control to the Origin OS kernel

## Development Philosophy

OriginBoot is being developed step by step with a focus on understanding
how the boot process works rather than relying on a pre-built bootloader.

The goal is to understand and implement each stage of the boot process
before moving to the next one.

## Version

Current development stage: **UEFI Bootloader Skeleton**

Target milestone: **OriginBoot v0.1**