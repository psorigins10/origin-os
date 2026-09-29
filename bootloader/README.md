# ORIGIN Boot

ORIGIN OS bootloader — a UEFI application responsible for preparing the system for kernel execution.

## Structure

```text
bootloader/
├── include/    # UEFI interfaces and bootloader headers
├── src/        # Bootloader source
└── build/      # Generated build artifacts
```

## Build

From the repository root:

```bash
make
```

## Run

Boot with QEMU and OVMF:

```bash
make run
```

## Current Status

- UEFI application entry point
- UEFI system table access
- Console output
- UEFI Boot Services access
- Memory map acquisition
- Automated ESP preparation
- QEMU/OVMF boot workflow

## Roadmap

- Memory map validation
- Filesystem access
- Kernel loading
- Graphics information
- ACPI discovery
- ExitBootServices()
- Kernel handoff