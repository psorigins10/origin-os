# Boot Process

ORIGIN currently boots through a UEFI firmware environment. The bootloader
is a UEFI PE/COFF application that prepares the kernel execution environment,
terminates UEFI Boot Services, and transfers control to the ORIGIN kernel.

## Boot Image

The bootloader is built as:

```text
BOOTX64.EFI
```

For the standard x86-64 removable-media UEFI boot path, the firmware locates
the application at:

```text
EFI/BOOT/BOOTX64.EFI
```

UEFI loads the bootloader and transfers control to its entry point.

## Bootloader Entry Point

UEFI enters the bootloader through:

```c
EFI_STATUS EFIAPI efi_main(
    EFI_HANDLE ImageHandle,
    EFI_SYSTEM_TABLE *SystemTable
);
```

The bootloader receives:

- `ImageHandle` — handle identifying the loaded bootloader image.
- `SystemTable` — pointer to the UEFI System Table.

The System Table provides access to the UEFI interfaces required during the
boot phase.

## Current Boot Flow

The current boot sequence is:

```text
UEFI Firmware
      │
      ▼
BOOTX64.EFI
      │
      ▼
efi_main()
      │
      ├── UEFI interface initialization
      │
      ├── Loaded Image Protocol
      │        │
      │        ▼
      │   Boot device handle
      │
      ├── Simple File System Protocol
      │        │
      │        ▼
      │   Filesystem root
      │        │
      │        ▼
      │   \boot\kernel.elf
      │
      ├── ELF64 kernel loading
      │        │
      │        ▼
      │   Kernel entry point
      │
      ├── Graphics Output Protocol
      │        │
      │        ▼
      │   Framebuffer information
      │
      ├── Initial memory map
      │
      ├── ORIGIN_BOOT_INFO
      │
      ├── Final memory map
      │
      ├── ExitBootServices()
      │
      ▼
kernel_entry
      │
      ▼
kernel_main()
```

## Kernel Discovery

The bootloader uses the UEFI Loaded Image Protocol to identify the device
from which the bootloader was loaded.

It then uses the UEFI Simple File System Protocol to open the filesystem
associated with that device.

The filesystem root is opened through:

```c
OpenVolume()
```

The kernel image is then opened through the UEFI File Protocol at:

```text
\boot\kernel.elf
```

The kernel is read into a UEFI-allocated buffer before being processed by the
ORIGIN ELF loader.

## Kernel ELF Loading

The ORIGIN bootloader loads the kernel as an ELF64 image.

The ELF loader validates and processes the kernel image and provides its
entry point to the bootloader.

The kernel is not started through UEFI's image-loading interface. The
bootloader loads the ORIGIN kernel directly and transfers control to its ELF
entry point after the UEFI boot environment has been terminated.

The kernel is linked with:

```text
kernel/linker.ld
```

and its ELF entry point is:

```text
kernel_entry
```

## Graphics Initialization

The bootloader locates the UEFI Graphics Output Protocol and obtains the
active framebuffer information.

The following values are recorded in `ORIGIN_BOOT_INFO`:

```text
Framebuffer base
Framebuffer size
Horizontal resolution
Vertical resolution
Pixels per scanline
Pixel format
```

These values allow the kernel to access the framebuffer after
`ExitBootServices()`.

The current kernel uses the framebuffer to clear the display and render its
initial text output.

## Initial Memory Map

The bootloader obtains an initial UEFI memory map during the boot process.

The initial map is used for memory-map inspection and debugging.

It is not the map used for the final `ExitBootServices()` transition.

See:

- [Memory Map](../uefi/memory-map.md)

## Boot Information

Before leaving the UEFI environment, the bootloader constructs:

```c
ORIGIN_BOOT_INFO
```

The current structure contains:

```text
Memory map address
Memory map size
Memory descriptor size

Framebuffer base
Framebuffer size
Framebuffer width
Framebuffer height
Pixels per scanline
Framebuffer pixel format
```

The structure is the bootloader-to-kernel interface for information required
after UEFI Boot Services are no longer available.

See:

- [Boot Protocol](boot-protocol.md)

## Final Memory Map

The bootloader obtains a fresh memory map immediately before
`ExitBootServices()`.

The final map has its own allocated buffer and capacity. The actual returned
map size is tracked separately from the buffer capacity.

The final sequence is:

```text
GetMemoryMap()
      │
      │ returns MapKey
      ▼
ExitBootServices(ImageHandle, MapKey)
      │
      ├── success
      │
      └── EFI_INVALID_PARAMETER
                │
                ▼
         fresh GetMemoryMap()
                │
                └── retry
```

No additional Boot Services operation is performed between the final
`GetMemoryMap()` and `ExitBootServices()`.

After successful termination, the bootloader does not use UEFI Boot Services
again.

## ExitBootServices()

`ExitBootServices()` terminates the UEFI Boot Services environment.

A successful call establishes the boundary between firmware-controlled boot
services and kernel execution.

After successful `ExitBootServices()`:

- UEFI Boot Services are unavailable.
- The bootloader does not perform console output.
- The bootloader does not allocate or free Boot Services memory.
- The bootloader does not request another memory map.
- The final boot information is already prepared.
- Control is transferred to the kernel.

The final memory map and framebuffer information are therefore prepared
before the transition.

## Kernel Entry

The bootloader transfers control to:

```text
kernel_entry
```

The kernel entry assembly:

1. Disables interrupts.
2. Initializes the bootstrap stack.
3. Clears `RBP`.
4. Calls `kernel_main`.

The bootstrap stack is reserved by the kernel entry assembly and is 16 KiB in
size.

The kernel entry point preserves the incoming `RDI` register value so that
the `ORIGIN_BOOT_INFO *` argument remains available to `kernel_main` under
the System V AMD64 calling convention.

## ABI Boundary

The bootloader and kernel are built with different compiler ABIs.

The bootloader is built with the MinGW x86-64 environment, while the kernel
is built using the System V AMD64 ABI.

The kernel entry function pointer used by the bootloader is therefore
declared with:

```c
__attribute__((sysv_abi))
```

This ensures that the `ORIGIN_BOOT_INFO *` argument is passed in `RDI` when
the bootloader enters the kernel.

The kernel entry assembly does not move the argument from `RCX` to `RDI`.

## Kernel Handoff

The final handoff is conceptually:

```text
UEFI Boot Services
       │
       │ ExitBootServices()
       ▼
ORIGIN kernel environment
       │
       ▼
kernel_entry
       │
       ▼
kernel_main(ORIGIN_BOOT_INFO *)
```

The kernel receives the prepared boot information rather than the UEFI
System Table or Boot Services table.

## Current Kernel Initialization

The current kernel consumes the framebuffer information from
`ORIGIN_BOOT_INFO`.

It:

1. Stores the boot information pointer.
2. Clears the framebuffer.
3. Renders the initial ORIGIN OS text.
4. Enters a halt loop.

The memory-map fields are currently passed to the kernel but are not yet
consumed by `kernel_main`.

## Ownership Boundary

The boot process has a strict ownership boundary at successful
`ExitBootServices()`:

```text
Before ExitBootServices()
────────────────────────────────
UEFI firmware + ORIGIN bootloader

- UEFI protocols
- Boot Services
- Memory allocation
- Filesystem access
- Graphics discovery
- Memory-map acquisition
- Kernel loading


After ExitBootServices()
────────────────────────────────
ORIGIN kernel

- No UEFI Boot Services
- No UEFI filesystem access
- No UEFI console output
- Uses ORIGIN_BOOT_INFO
- Executes from kernel entry point
```

This boundary defines the transition from firmware-assisted bootstrapping to
kernel-controlled execution.