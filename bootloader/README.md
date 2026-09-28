# ORIGIN Boot

> ORIGIN OS BOOTLOADER

The ORIGIN OS bootloader is a UEFI application responsible for initializing the boot environment and preparing the system for kernel execution.

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

Build and boot the bootloader with QEMU:

```bash
make run
```

The QEMU environment uses OVMF as the UEFI firmware.

## Current Status

* UEFI application entry point
* UEFI system table access
* Console output
* UEFI protocol and service declarations
* Automated ESP preparation
* QEMU/OVMF boot workflow

## Roadmap

* UEFI Boot Services
* Filesystem access
* Kernel loading
* Memory map acquisition
* Graphics information
* ACPI discovery
* ExitBootServices()
* Kernel handoff
