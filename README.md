# ORIGIN OS

ORIGIN OS is a private operating system project being built from the ground up.

## Project Structure

```text
origin-os/
├── bootloader/    # UEFI bootloader
│   ├── include/   # UEFI and bootloader interfaces
│   └── src/       # Bootloader implementation
├── docs/          # Architecture, boot, and research documentation
├── scripts/       # Build and development scripts
├── Makefile       # Build and run targets
├── README.md
└── LICENSE
```

## Build

From the repository root:

```bash
make
```

## Run

Build and run ORIGIN OS with QEMU:

```bash
make run
```

QEMU uses OVMF to provide the UEFI firmware environment.

## Current Status

ORIGIN is currently in the UEFI bootloader stage.

Implemented and verified:

- UEFI bootloader entry point
- UEFI system table access
- Console output
- UEFI Boot Services access
- UEFI memory map acquisition
- Memory map dumping
- Automated build system
- ESP image preparation
- QEMU/OVMF boot workflow

The current bootloader can start under UEFI, acquire the memory map, and dump its descriptors successfully.

## Roadmap

The next major milestone is implementing a correct `ExitBootServices()` flow, followed by the boot protocol and kernel-loading stages.

## License

See [LICENSE](LICENSE).
