# ORIGIN OS

ORIGIN OS is a private operating system project being built from the ground up.

## Project Structure

```text
origin-os/
├── arch/          # Architecture-specific kernel entry and low-level code
├── bootloader/    # UEFI bootloader
│   ├── include/   # UEFI, ELF, and bootloader interfaces
│   └── src/       # Bootloader implementation
├── docs/          # Architecture, boot, and research documentation
├── include/       # Shared boot interfaces
├── kernel/        # ORIGIN kernel
├── scripts/       # Build and development scripts
├── Makefile       # Build and run targets
├── README.md
└── LICENSE
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

The kernel is produced as:

```text
kernel/build/bin/kernel.elf
```

## Run

Build the ESP image and run ORIGIN OS with QEMU and OVMF:

```bash
make run
```

QEMU uses OVMF to provide the UEFI firmware environment.

## Boot Flow

The current boot flow is:

```text
UEFI Firmware
      │
      ▼
BOOTX64.EFI
      │
      ▼
UEFI bootloader
      │
      ├── Locate boot device
      ├── Open filesystem
      ├── Load oot\kernel.elf
      ├── Load ELF64 kernel image
      ├── Obtain framebuffer information
      ├── Obtain final UEFI memory map
      ├── Construct ORIGIN_BOOT_INFO
      ├── ExitBootServices()
      │
      ▼
kernel_entry
      │
      ▼
kernel_main()
```

The bootloader transfers an `ORIGIN_BOOT_INFO *` pointer to the kernel. The
current boot information contains the final UEFI memory map and framebuffer
information.

## Current Kernel

The kernel currently initializes its framebuffer using the information
provided by the bootloader, clears the display, renders its initial output,
and enters a halt loop.

## Documentation

Boot architecture and implementation documentation is available under:

```text
docs/boot/
```

See:

- [Boot Process](docs/boot/architecture/boot-process.md)
- [Boot Protocol](docs/boot/architecture/boot-protocol.md)
- [UEFI](docs/boot/uefi/uefi.md)
- [Memory Map](docs/boot/uefi/memory-map.md)
- [Boot Documentation](docs/boot/README.md)

## License

See [LICENSE](LICENSE).