# Boot Process

ORIGIN OS currently boots as a UEFI application using a UEFI firmware environment.

## Current Boot Flow

```text
UEFI Firmware
      │
      │ Loads
      ▼
BOOTX64.EFI
      │
      │ Calls
      ▼
efi_main()
      │
      │ Provides
      ▼
EFI_SYSTEM_TABLE
      │
      ▼
ORIGIN Bootloader
      │
      ▼
Console Output
```

## UEFI Application

The bootloader is built as a UEFI PE/COFF application named:

```text
BOOTX64.EFI
```

The UEFI firmware locates the application using the standard x86-64 removable-media boot path:

```text
EFI/BOOT/BOOTX64.EFI
```

## Entry Point

UEFI transfers control to the bootloader through:

```c
EFI_STATUS EFIAPI efi_main(
    EFI_HANDLE ImageHandle,
    EFI_SYSTEM_TABLE *SystemTable
);
```

The bootloader receives:

* `ImageHandle` — handle identifying the loaded bootloader image.
* `SystemTable` — pointer to the firmware-provided UEFI System Table.

## System Table

`EFI_SYSTEM_TABLE` provides access to firmware interfaces required by the bootloader.

The current implementation uses:

```text
SystemTable
    └── ConOut
          └── OutputString()
```

The bootloader wraps console output through its own `Print()` interface.

## Current State

The current bootloader:

* Starts as a UEFI application.
* Receives the UEFI System Table.
* Accesses the UEFI console output protocol.
* Displays bootloader output.
* Remains running under the UEFI environment.

## Future Boot Flow

The boot process will eventually extend to:

```text
UEFI Firmware
      │
      ▼
ORIGIN Bootloader
      │
      ├── Locate filesystem
      ├── Load kernel
      ├── Obtain memory map
      ├── Obtain graphics information
      ├── Locate ACPI information
      ├── ExitBootServices()
      │
      ▼
ORIGIN Kernel
```

These stages are not currently implemented.
