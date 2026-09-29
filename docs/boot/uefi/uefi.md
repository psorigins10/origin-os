# UEFI

ORIGIN OS currently uses the UEFI specification as the firmware interface for the bootloader.

## UEFI Application

The bootloader is built as a UEFI PE/COFF application:

```text
BOOTX64.EFI
```

UEFI loads the application and transfers control to its entry point.

## Entry Point

The bootloader entry point is:

```c
EFI_STATUS EFIAPI efi_main(
    EFI_HANDLE ImageHandle,
    EFI_SYSTEM_TABLE *SystemTable
);
```

UEFI provides:

* `ImageHandle` — identifies the loaded application.
* `SystemTable` — provides access to UEFI firmware interfaces.

## System Table

`EFI_SYSTEM_TABLE` is a firmware-provided structure containing references to UEFI services, protocols, and configuration information.

The current implementation uses:

```text
EFI_SYSTEM_TABLE
├── ConIn
├── ConOut
├── StdErr
├── RuntimeServices
├── BootServices
└── ConfigurationTable
```

Only the interfaces required by the current bootloader are implemented.

## Protocols

UEFI protocols provide standardized interfaces to firmware functionality.

The current bootloader uses:

```text
EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL
```

through:

```text
SystemTable->ConOut
```

Console output is performed through the firmware-provided:

```text
OutputString()
```

function.

## Boot Services

`EFI_BOOT_SERVICES` provides firmware services available during the boot phase.

The bootloader currently declares the Boot Services interface but does not yet use its services for kernel loading or memory management.

Future bootloader functionality will use Boot Services to:

* Locate protocols
* Access filesystems
* Allocate memory
* Obtain the memory map
* Prepare for `ExitBootServices()`

## Runtime Services

`EFI_RUNTIME_SERVICES` provides services that may remain available after `ExitBootServices()`.

The bootloader currently declares the Runtime Services interface but does not actively use it.

## Configuration Tables

UEFI exposes system-specific information through configuration tables.

The bootloader currently declares the configuration table interface but does not yet consume entries such as ACPI information.

## Implementation

ORIGIN maintains its own UEFI type and interface declarations rather than depending on GNU-EFI or EDK II.

Relevant declarations are located under:

```text
bootloader/include/uefi/
```

The declarations are implemented incrementally as required by the bootloader.
