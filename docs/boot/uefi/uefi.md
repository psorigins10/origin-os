# UEFI

ORIGIN uses the UEFI firmware environment as the boot interface for the
bootloader.

The bootloader maintains its own UEFI type and protocol declarations under
`bootloader/include/uefi/` and uses only the firmware interfaces required by
the current boot flow.

## UEFI Application

The bootloader is built as a UEFI PE/COFF application:

```text
BOOTX64.EFI
```

For x86-64 removable-media boot, the firmware loads the application from:

```text
EFI/BOOT/BOOTX64.EFI
```

UEFI transfers control to the bootloader through:

```c
EFI_STATUS EFIAPI efi_main(
    EFI_HANDLE ImageHandle,
    EFI_SYSTEM_TABLE *SystemTable
);
```

The bootloader receives:

- `ImageHandle` — handle identifying the loaded bootloader image.
- `SystemTable` — pointer to the UEFI System Table.

## UEFI System Table

`EFI_SYSTEM_TABLE` is provided by firmware and contains references to the
UEFI interfaces required by the bootloader.

The current bootloader uses:

```text
EFI_SYSTEM_TABLE
├── ConOut
├── BootServices
└── RuntimeServices
```

`ConOut` provides the UEFI console output interface used during the boot
phase.

`BootServices` provides services required for protocol discovery, memory
management, filesystem access, memory-map acquisition, and termination of
the UEFI boot environment.

`RuntimeServices` is declared in the UEFI interface layer but is not
currently used by the bootloader.

## UEFI Boot Services

`EFI_BOOT_SERVICES` provides firmware services available during the UEFI
boot phase.

The bootloader currently uses Boot Services for:

- Retrieving protocol interfaces.
- Locating protocols.
- Allocating memory pools.
- Obtaining the UEFI memory map.
- Terminating UEFI Boot Services.

The relevant interfaces include:

```text
HandleProtocol()
LocateProtocol()
AllocatePool()
GetMemoryMap()
ExitBootServices()
```

Boot Services are available only until `ExitBootServices()` succeeds.

After successful termination, the bootloader performs no further Boot
Services operations before transferring control to the kernel.

## UEFI Runtime Services

`EFI_RUNTIME_SERVICES` contains firmware services that may remain available
after `ExitBootServices()`.

The ORIGIN UEFI interface layer declares the Runtime Services table, but the
current bootloader does not use Runtime Services during the kernel handoff.

Runtime Services are therefore not part of the current boot protocol.

## Protocols

The bootloader currently uses the following UEFI protocols.

### Loaded Image Protocol

The Loaded Image Protocol provides information about the currently executing
bootloader image, including the device from which the image was loaded.

The bootloader uses the loaded-image information to identify the device
handle used to locate the filesystem containing the kernel.

### Simple File System Protocol

The Simple File System Protocol provides access to the filesystem associated
with the boot device.

The bootloader obtains the protocol and opens the filesystem root through:

```c
OpenVolume()
```

The returned root handle is an `EFI_FILE_PROTOCOL`.

### File Protocol

The File Protocol provides operations for accessing files and directories.

The bootloader uses the file interface to locate and read:

```text
\boot\kernel.elf
```

The kernel image is read into a UEFI-allocated memory buffer before being
passed to the ELF loader.

### Graphics Output Protocol

The Graphics Output Protocol provides information about the active
framebuffer.

The bootloader obtains:

- Framebuffer base address.
- Framebuffer size.
- Horizontal resolution.
- Vertical resolution.
- Pixels per scanline.
- Pixel format.

These values are stored in `ORIGIN_BOOT_INFO` and passed to the kernel.

## Kernel Loading

The bootloader loads the kernel as an ELF64 image.

The kernel file is located through the UEFI filesystem interfaces and read
into memory before being processed by the ORIGIN ELF loader.

The ELF loader validates and loads the required kernel image data and
provides the kernel entry point.

The bootloader does not use UEFI `LoadImage()` and `StartImage()` for the
ORIGIN kernel. The kernel is loaded and entered directly by the ORIGIN
bootloader.

## Memory Management

The bootloader uses UEFI Boot Services memory allocation during the boot
phase.

Memory pools are used for bootloader data and buffers, including the kernel
image and boot information.

The UEFI memory map is obtained before the transition to the kernel.

The bootloader maintains separate handling for the initial memory map used
for inspection and the final memory map used for `ExitBootServices()`.

See:

- [Memory Map](memory-map.md)

## ExitBootServices()

`ExitBootServices()` terminates the UEFI Boot Services environment.

Before calling it, the bootloader obtains a fresh memory map and its
associated `MapKey`.

The required transition is:

```text
GetMemoryMap()
      │
      │ returns MapKey
      ▼
ExitBootServices(ImageHandle, MapKey)
      │
      ▼
UEFI Boot Services terminated
      │
      ▼
Kernel handoff
```

The memory map can change as a result of Boot Services activity. Therefore,
the final `GetMemoryMap()` is performed immediately before
`ExitBootServices()`.

If `ExitBootServices()` returns `EFI_INVALID_PARAMETER`, the bootloader
obtains a fresh memory map and retries using the new map key.

After successful termination:

- Boot Services are unavailable.
- The bootloader does not call UEFI console services.
- The bootloader does not allocate or free Boot Services memory.
- The bootloader does not request another memory map.
- Control is transferred to the kernel.

## UEFI Interface Layer

ORIGIN maintains its own declarations for the UEFI interfaces required by
the bootloader.

The declarations are located under:

```text
bootloader/include/uefi/
```

The interface layer includes:

```text
efi_types.h
efi_annotations.h
efi_system_table.h
efi_table_header.h
efi_file.h

services/
├── efi_boot_services.h
└── efi_runtime_services.h

protocols/
├── efi_loaded_image.h
├── efi_simple_file_system.h
└── efi_graphics_output.h
```

These declarations provide the structures, function-pointer types, and
protocol layouts required by the bootloader.

The UEFI protocol and service structures are ABI-sensitive. Their member
ordering must match the UEFI-defined interface layout.

## UEFI and Kernel Boundary

UEFI interfaces are used only while the bootloader remains in the UEFI boot
environment.

The kernel does not receive the UEFI System Table or Boot Services table as
its primary boot interface.

Instead, the bootloader collects the required information and constructs:

```c
ORIGIN_BOOT_INFO
```

The boot information is passed to the kernel after `ExitBootServices()`
succeeds.

See:

- [Boot Process](../architecture/boot-process.md)
- [Boot Protocol](../architecture/boot-protocol.md)