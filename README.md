# ORIGIN OS

ORIGIN OS is a private operating system project being built from the
ground up.

## Project Structure

```text
origin-os/
├── arch/          # Architecture-specific kernel entry and low-level code
├── bootloader/    # UEFI bootloader
├── docs/          # Architecture, design, boot, and research documentation
├── include/       # Shared kernel and subsystem interfaces
├── kernel/        # ORIGIN kernel
├── mm/            # Memory management
├── sched/         # Scheduling and process management
├── drivers/       # Hardware drivers
├── fs/            # Filesystem and VFS
├── scripts/       # Build and development scripts
├── tests/         # Unit and integration tests
├── Makefile
├── README.md
└── LICENSE
```

## Build

Build the bootloader and kernel from the repository root:

```bash
make
```

Bootloader output:

```text
bootloader/build/efi/BOOTX64.EFI
```

Kernel output:

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
     ├── Load boot/kernel.elf
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

## Current Implementation

The current implementation includes the bootloader, kernel entry path,
boot protocol, framebuffer, and early kernel console output.

Implemented subsystem documentation is maintained under:

```text
docs/
```

### Boot

- [Boot Documentation](docs/boot/README.md)
- [Boot Process](docs/boot/architecture/boot-process.md)
- [Boot Protocol](docs/boot/architecture/boot-protocol.md)
- [UEFI](docs/boot/uefi/uefi.md)
- [Memory Map](docs/boot/uefi/memory-map.md)

### Console

- [Console Overview](docs/design/console/README.md)
- [Console Architecture](docs/design/console/architecture.md)
- [Console Rendering](docs/design/console/rendering.md)
- [Console API](docs/design/console/api.md)

## Development Status

ORIGIN OS is under active development.

Subsystems that are not yet implemented are documented separately as
design, research, or planned work and are not represented as completed
functionality.

## License

See [LICENSE](LICENSE).