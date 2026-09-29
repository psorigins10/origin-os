# Boot Build System

The ORIGIN bootloader is built from the repository root using GNU Make and the MinGW-w64 x86-64 cross compiler.

## Build Pipeline

```text
bootloader/src/main.c
        │
        ▼
bootloader/build/obj/main.o
        │
        ├──────────────┐
        │              │
bootloader/src/       │
console.c             │
        │              │
        ▼              │
bootloader/build/     │
obj/console.o ────────┘
        │
        ▼
bootloader/build/efi/BOOTX64.EFI
        │
        ▼
bootloader/build/esp/esp.img
        │
        ▼
QEMU + OVMF
```

## Compiler

The bootloader is compiled using:

```text
x86_64-w64-mingw32-gcc
```

Compilation uses freestanding mode:

```text
-ffreestanding
```

The final UEFI application is linked as a PE/COFF executable with the UEFI subsystem and `efi_main` entry point.

## Build Commands

Build the bootloader:

```bash
make
```

Clean generated bootloader artifacts:

```bash
make clean
```

Build and run the bootloader under QEMU:

```bash
make run
```

## Build Artifacts

Generated files are stored under:

```text
bootloader/build/
├── obj/
│   ├── main.o
│   └── console.o
├── efi/
│   └── BOOTX64.EFI
└── esp/
    └── esp.img
```

The `build/` directory contains generated artifacts and is not part of the bootloader source tree.

## ESP Preparation

The ESP image is automatically prepared by:

```text
scripts/build-esp.sh
```

The script:

1. Creates the ESP image when required.
2. Formats it as FAT32.
3. Mounts the image.
4. Creates the UEFI boot directory.
5. Installs `BOOTX64.EFI`.
6. Unmounts the image.

The resulting boot path is:

```text
EFI/BOOT/BOOTX64.EFI
```

## QEMU

The bootloader is tested using QEMU with OVMF as the UEFI firmware environment.

QEMU execution is handled by:

```text
scripts/run-qemu.sh
```

The complete development workflow is therefore:

```text
make run
    │
    ▼
Build bootloader
    │
    ▼
Prepare ESP
    │
    ▼
Launch QEMU + OVMF
```
