# Kernel Entry and Initialization

This page follows the current `main` branch from the bootloader's kernel handoff into the C kernel entry point. It describes what the checked-in code does today and calls out ordering and validation work that remains.

## 1. High-level handoff

The boot path currently follows this sequence:

1. The UEFI bootloader opens `\\boot\\kernel.elf`, reads it into a temporary buffer, and calls `LoadKernel()`.
2. The ELF loader validates basic ELF64/x86-64 properties, reserves the kernel's page-aligned physical range, copies each `PT_LOAD` segment to its physical destination, and zero-fills the remainder of each segment.
3. The bootloader obtains the final UEFI memory map and calls `ExitBootServices()`. It retries if the memory-map key is rejected.
4. The bootloader fills an `ORIGIN_BOOT_INFO` structure with the boot protocol version, memory-map information, and framebuffer properties.
5. The bootloader calls the ELF entry point through a function pointer declared with the System V ABI. The boot information pointer is therefore passed in `RDI`.
6. `kernel_entry` disables maskable interrupts, establishes a bootstrap stack, clears `RBP`, and calls `kernel_main`.
7. `kernel_main` initializes the framebuffer and console, checks the boot magic and protocol major version, displays a welcome/status message, initializes serial output, and halts.

The kernel is linked at physical/virtual address `0x100000` by `kernel/linker.ld`. The ELF loader currently expects an `ET_EXEC` x86-64 ELF image and requests the page-aligned load range at the addresses encoded in the ELF program headers.

## 2. ABI boundary: bootloader to kernel

The bootloader is built with the Microsoft x64 ABI, while the kernel C code uses the System V AMD64 ABI. Under the System V ABI, the first integer or pointer argument is passed in `RDI`; under the Microsoft x64 ABI, it is passed in `RCX`.

The bootloader handles this boundary by declaring the kernel entry pointer with `__attribute__((sysv_abi))` and then calling it with `BootInfo`. The current `kernel_entry.asm` therefore expects the pointer already in `RDI`; it must not blindly copy `RCX` into `RDI`.

When changing this boundary, keep the function-pointer declaration, compiler ABI, and assembly entry code consistent. A mismatch can cause the kernel to receive an invalid boot-information pointer even though the bootloader's call itself succeeds.

## 3. Assembly entry point

Current file: `arch/x86_64/boot/kernel_entry.asm`.

The current checked-in entry stub performs these operations:

- `cli` disables maskable interrupts during early startup.
- `lea rsp, [rel stack_top]` sets `RSP` to the top of a 16 KiB stack reserved in `.bss`.
- `xor rbp, rbp` clears the frame pointer, giving early stack traces a recognizable base.
- `call kernel_main` transfers control to C. The boot-information pointer remains in `RDI`.

If `kernel_main` returns, the assembly stub enters a `cli; hlt` loop.

**Current limitation:** the version of `kernel_entry.asm` on `main` does not call `gdt_init` or `idt_init`. Descriptor-table initialization must not be described as part of this entry path until the code is updated and verified. The GDT/TSS/IDT documentation describes those components separately, but integration order should be checked against the exact revision being built.

## 4. Boot information contract

The bootloader and kernel share `include/origin/boot/boot_info.h`. The structure currently contains:

| Field group | Fields | Purpose |
|---|---|---|
| Identity/version | `magic`, boot protocol major/minor, bootloader major/minor | Identify the structure and protocol version |
| Memory map | `memory_map`, `memory_map_size`, `memory_descriptor_size` | Describe the final UEFI memory map |
| Framebuffer | `fb_base`, `fb_size`, width, height, pixels per scanline, pixel format | Provide graphics-output metadata |
| Firmware/platform | `acpi_rsdp` | Reserved for the ACPI RSDP address |
| Boot configuration | `cmdline`, `cmdline_size` | Reserved for a command line |

The current bootloader fills the protocol identity, memory-map, and framebuffer fields. The ACPI RSDP and command-line fields exist in the structure but are not populated in the inspected handoff code; treat them as unavailable until their initialization is implemented.

The boot protocol currently defines major version `0` and minor version `1`. The kernel checks the magic value and major version. It does not currently reject a different minor version.

## 5. Current C initialization order

Current file: `kernel/init/kernel.c`.

The checked-in implementation currently performs the following operations:

1. Checks whether `BootInfo` is null and halts if it is.
2. Calls `framebuffer_init(BootInfo)` and `console_init(BootInfo)`.
3. Checks the boot magic value and protocol major version, displaying an error headline if either check fails.
4. Clears the console and displays the ORIGIN OS welcome/status messages.
5. Initializes serial output and writes `READY to launch`.
6. Enters a halt loop.

### Validation-order caveat

In the current file, framebuffer and console initialization happen **before** the magic and protocol checks. This means those routines may consume fields from a boot-information structure that has not yet been validated. The null check prevents a null dereference, but it does not validate the contents.

**Recommended follow-up:** validate the magic and supported protocol version before passing `BootInfo` to framebuffer or console initialization. For early error reporting, serial output can be initialized before validation so failures can be reported without depending on framebuffer state. This is a recommended improvement, not a description of the current order.

The current code also initializes serial output after printing the graphical-console ready messages. Do not assume serial output is available earlier in this version of `kernel_main`.

## 6. Memory-map timing

The bootloader obtains an initial memory map for debugging, but that initial snapshot is not the map handed to the kernel. It later allocates a dedicated buffer, obtains the final map, and immediately calls `ExitBootServices()` with the corresponding map key.

This ordering is important because UEFI boot-service activity can change the memory map and invalidate the map key. The final successful map size, descriptor size, and buffer address are stored in `BootInfo` after boot services have exited.

After `ExitBootServices()` succeeds, the bootloader must not call UEFI Boot Services. The kernel receives the memory map as data; a general physical-memory allocator is a separate subsystem and should not be implied by the existence of this field alone.

## 7. Current state and next steps

**Implemented in the inspected `main` branch:**

- UEFI bootloader loads an ELF64 x86-64 kernel.
- Kernel ELF loadable segments are copied to their linked physical addresses, and zero-fill regions are cleared.
- The bootloader passes a versioned `ORIGIN_BOOT_INFO` structure to the kernel.
- The assembly entry point establishes a bootstrap stack and calls C.
- The kernel initializes framebuffer/console output, checks the boot identity/version, prints a ready message, and halts.

**Follow-up work / not claimed as complete here:**

- Move boot-information validation before framebuffer and console initialization.
- Verify and integrate GDT/TSS/IDT initialization in the actual entry sequence.
- Populate or explicitly mark unavailable the ACPI RSDP and command-line fields.
- Implement a physical-memory manager that consumes the memory map.
- Add a scheduler and a defined transition from the startup halt loop to normal kernel execution.

Keep this page synchronized with the exact source revision being built. If local work is ahead of `main`, document that work on its branch rather than silently describing it as already present on `main`.
