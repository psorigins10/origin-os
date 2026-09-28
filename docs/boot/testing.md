# Boot Testing

The ORIGIN bootloader is tested in a virtual UEFI environment using QEMU and OVMF.

## Environment

* Architecture: x86-64
* Firmware: OVMF
* Emulator: QEMU
* Boot format: UEFI
* ESP filesystem: FAT32

## Test Command

The complete build and boot process is executed with:

```bash
make run
```

This builds the bootloader, prepares the ESP image, and launches QEMU.

## Expected Result

A successful boot transfers control from the UEFI firmware to the ORIGIN bootloader.

The current bootloader displays:

```text
Welcome To ORIGIN Boot
```

and remains running.

## Boot Verification

A boot is considered successful when:

1. The bootloader builds without errors.
2. The ESP image is prepared successfully.
3. QEMU starts with OVMF.
4. UEFI locates `EFI/BOOT/BOOTX64.EFI`.
5. `efi_main()` executes.
6. The ORIGIN boot message is displayed.

## Test Environment

Testing is performed entirely in QEMU using OVMF. No physical firmware or host boot configuration is modified.
