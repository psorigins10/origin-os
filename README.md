# ORIGIN OS

ORIGIN OS is a private operating system project built from the ground up.

## Project Structure

```text
origin-os/
├── arch/          # Architecture-specific code
├── bootloader/    # UEFI bootloader
├── drivers/       # Device drivers
├── fs/            # Filesystem
├── kernel/        # Kernel
├── mm/            # Memory management
├── sched/         # Scheduler
├── ai/            # AI subsystem
├── docs/          # Project documentation
├── scripts/       # Build and development scripts
├── tests/         # Tests
└── tools/         # Development tools
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

QEMU uses OVMF as the UEFI firmware environment.

## Status

ORIGIN OS is under active development.

Current milestone:

- UEFI bootloader
- UEFI system table access
- Console output
- UEFI Boot Services access
- Memory map acquisition
- Automated build system
- Automated ESP preparation
- QEMU/OVMF boot workflow

## License

See [LICENSE](LICENSE).