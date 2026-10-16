# ORIGIN OS

ORIGIN OS is an open source operating system project being developed from the ground up.

## Project Structure

```text
origin-os/
├── arch/          # Architecture-specific kernel entry and low-level code
├── bootloader/    # UEFI bootloader
├── docs/          # Architecture, design, boot, and research documentation
├── include/       # Shared headers and subsystem interfaces
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

The kernel ELF contains debugging information when built with the `-g` compiler flag.

## Run

Build the EFI System Partition (ESP) image and run ORIGIN OS using QEMU and OVMF:

```bash
make run
```

QEMU uses OVMF to provide the UEFI firmware environment. Serial output is redirected to the host terminal.

## Kernel Debugging

ORIGIN OS supports source-level kernel debugging with GDB and QEMU.

Start QEMU in debugging mode:

```bash
make debug
```

In a second terminal, connect GDB to QEMU:

```bash
make debug-gdb
```

The debug configuration pauses the virtual CPU at startup, exposes the GDB remote stub on TCP port `1234`, and automatically sets a breakpoint at `kernel_main`.

See [Kernel Debugging](docs/boot/development/debugging.md) for setup details and GDB commands.

## Boot Flow

```text
UEFI Firmware
    |
    v
BOOTX64.EFI
    |
    v
UEFI Bootloader
    |
    ├── Locate boot device
    ├── Open filesystem
    ├── Load boot/kernel.elf
    ├── Load ELF64 kernel image
    ├── Obtain framebuffer information
    ├── Obtain final UEFI memory map
    ├── Construct ORIGIN_BOOT_INFO
    ├── ExitBootServices()
    |
    v
kernel_entry
    |
    v
kernel_main()
```

## Current Implementation

Implemented components include:

- UEFI bootloader and kernel loading
- Boot protocol and boot information transfer
- Framebuffer initialization and early kernel console output
- x86-64 port-mapped I/O primitives
- COM1 serial driver with polling-based transmission
- QEMU and GDB kernel debugging workflow

## Documentation

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

### Serial Driver

- [Serial Driver Overview](docs/architecture/drivers/serial/README.md)
- [Serial Driver Architecture](docs/architecture/drivers/serial/architecture.md)
- [Serial Driver API](docs/architecture/drivers/serial/api.md)
- [Serial Driver Testing](docs/architecture/drivers/serial/testing.md)

### Development and Debugging

- [Build System](docs/boot/development/build-system.md)
- [Testing](docs/boot/development/testing.md)
- [Kernel Debugging](docs/boot/development/debugging.md)

## Development Status

ORIGIN OS is under active development. Implemented functionality, architectural design, research, and planned work are documented separately. Components listed as planned or under research should not be considered implemented.

## License

See [LICENSE](LICENSE).
