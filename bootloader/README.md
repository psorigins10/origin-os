# ORIGIN Boot

The ORIGIN bootloader is a UEFI application responsible for preparing the
system for kernel execution.

## Structure

```text
bootloader/
├── include/    # UEFI interfaces and bootloader headers
├── src/        # Bootloader source
└── build/      # Generated build artifacts
```

## Build

Build the bootloader and kernel from the repository root:

```bash
make
```

The bootloader is produced as:

```text
bootloader/build/efi/BOOTX64.EFI
```

## Run

Build the bootable ESP and run ORIGIN with QEMU and OVMF:

```bash
make run
```

## Boot Responsibilities

The bootloader currently:

- Initializes access to the UEFI System Table and Boot Services.
- Provides UEFI console output.
- Locates the filesystem through the UEFI Simple File System Protocol.
- Opens and reads `\boot\kernel.elf`.
- Loads and parses the kernel ELF image.
- Obtains the kernel entry point.
- Locates the UEFI Graphics Output Protocol.
- Obtains framebuffer information.
- Obtains the UEFI memory map.
- Constructs the `ORIGIN_BOOT_INFO` structure.
- Obtains the final memory map required for `ExitBootServices()`.
- Terminates UEFI Boot Services.
- Transfers control to the ORIGIN kernel.

## Kernel Handoff

The bootloader passes an `ORIGIN_BOOT_INFO *` pointer to the kernel entry
point.

The boot information currently contains:

- Final UEFI memory map address.
- Memory map size.
- Memory descriptor size.
- Framebuffer base address.
- Framebuffer size.
- Framebuffer width.
- Framebuffer height.
- Pixels per scanline.
- Framebuffer pixel format.

The kernel entry point is `kernel_entry`.

See:

- [Boot Process](docs/boot/architecture/boot-process.md)
- [Boot Protocol](docs/boot/architecture/boot-protocol.md)

## UEFI Interfaces

The bootloader maintains its own declarations for the UEFI interfaces it
uses.

Relevant interfaces include:

- UEFI System Table
- UEFI Boot Services
- UEFI Runtime Services
- Loaded Image Protocol
- Simple File System Protocol
- File Protocol
- Graphics Output Protocol

See:

- [UEFI](docs/boot/uefi/uefi.md)
- [Memory Map](docs/boot/uefi/memory-map.md)