# Kernel Documentation

This section documents the kernel components that are currently present in ORIGIN OS. It is written for both developers working on the code and learners studying how a small x86-64 kernel is brought up.

> **Scope:** These pages describe the implementation represented by the current source tree and the code reviewed during documentation work. A feature is not considered implemented merely because it appears in a design discussion.

## Current kernel path

The early kernel path currently has these responsibilities:

1. The bootloader transfers control to the kernel entry point and supplies an `ORIGIN_BOOT_INFO` pointer.
2. The assembly entry code establishes a bootstrap stack, initializes the GDT/TSS, and calls `kernel_main`.
3. Kernel initialization starts serial output and validates the boot information's pointer, magic value, and boot-protocol major version.
4. The framebuffer and console are initialized from the validated boot information.
5. The IDT is initialized with handlers for selected CPU exceptions.
6. The kernel displays its welcome/status messages and halts in a loop while later subsystems are not yet taking over execution.

The exact order matters: early code must not use bootloader-provided resources before checking that the boot information is present and compatible.

## Pages

- [GDT, TSS, and IDT](descriptor-tables.md) — descriptor-table layout, selectors, the double-fault IST stack, and currently installed IDT entries.
- [Exceptions and panic handling](exceptions-and-panic.md) — exception entry, diagnostic output, register capture, and current limitations.

## Status vocabulary

- **Implemented:** The code path exists in the current implementation.
- **Partial:** Some basic functionality exists, but important coverage, validation, or integration remains.
- **Planned / not yet documented as implemented:** A future direction, not a claim about current behavior.

## Important current limitations

- The IDT installs handlers for divide error (#DE), double fault (#DF), general protection fault (#GP), and page fault (#PF). The other IDT entries are left absent.
- The double-fault handler uses IST1, backed by a dedicated 16 KiB stack configured in the TSS.
- The exception dispatcher currently prints the vector, error code, and instruction pointer (RIP). More complete fault diagnostics, such as page-fault address reporting, require the exception frame and assembly entry path to capture the relevant state consistently.
- Panic register capture exists, but the assembly capture routine and its C structure must remain layout-compatible. The exact meaning of each captured register should be verified against that assembly implementation.
- The kernel currently halts after displaying its ready message. A scheduler, user-mode execution, and general interrupt-driven operation are not described here as implemented.

When changing the kernel, update these documents alongside the code so they continue to describe observable behavior rather than intended future behavior.
