# ORIGIN OS

> An experimental x86-64 operating system exploring computer architecture,
> kernel design, security, and adaptive systems.

ORIGIN OS is a long-term systems research and learning project focused on
understanding how computers work from the lowest practical level upward.

The project begins with firmware, bootloaders, CPU architecture, memory,
and kernel development, then expands toward filesystems, drivers,
networking, security, distributed systems, and eventually AI/ML as a
potential active component of the operating system itself.

---

## What Exists Today

### OriginBoot

`bootloader/` contains the custom UEFI bootloader for ORIGIN OS.

OriginBoot currently:

- Runs as a UEFI application using gnu-efi
- Locates the filesystem containing the bootloader
- Opens `kernel.elf`
- Reads the kernel into memory
- Validates the ELF image
- Parses ELF program headers
- Loads `PT_LOAD` segments
- Obtains the kernel entry point
- Transfers execution to the kernel

See [`bootloader/README.md`](bootloader/README.md) for bootloader-specific
documentation.

### Initial Kernel

`kernel/` contains the first ORIGIN OS kernel implementation.

The current kernel is intentionally minimal. It is a freestanding x86-64
kernel with an explicit linker script and entry point.

Its current job is simple:

```text
Welcome to ORIGIN OS
```

The message is written through the serial interface so the kernel can be
observed while running under QEMU.

### Build System

The repository has a top-level build system that coordinates the kernel,
bootloader, disk image, and QEMU workflow.

From the repository root:

```bash
make
```

builds the system.

```bash
make run
```

builds the required components and launches QEMU.

For a clean rebuild:

```bash
make clean
make
```

Generated build artifacts are excluded from version control.

---

## Building

### Requirements

The current development environment requires tools including:

- GCC
- GNU binutils
- gnu-efi
- mtools
- dosfstools
- QEMU
- OVMF

### Build

From the repository root:

```bash
make
```

### Run

```bash
make run
```

### Clean

```bash
make clean
```.