# Exception and Panic Handling

ORIGIN OS currently has two related but distinct fatal-error paths:

- **CPU exception handling:** the IDT routes selected processor exceptions to assembly entry stubs, which pass a saved frame to C.
- **Kernel panic handling:** kernel code calls `panic(message)` to print a reason and a register snapshot to the serial port.

Both paths currently stop the processor rather than attempt recovery.

## 1. Installed exception handlers

The IDT currently installs handlers for:

- Vector 0 — Divide Error (#DE)
- Vector 8 — Double Fault (#DF)
- Vector 13 — General Protection Fault (#GP)
- Vector 14 — Page Fault (#PF)

The other IDT vectors are currently absent; see [GDT, TSS, and IDT](descriptor-tables.md).

## 2. Assembly entry and exception frames

The assembly stubs normalize the stack for the C dispatcher. Some x86 exceptions push an error code automatically, while others do not. The stub must account for that difference so the C frame has a consistent vector/error-code layout.

For example, divide error does not provide a CPU error code, so its stub supplies a synthetic zero. General protection and page faults provide an error code from the CPU, so their stubs must not add a second synthetic error code.

The C structure in `exception.h` and the assembly pushes/pops form one interface. If either changes, review the other at the same time. A successful build alone does not prove that the fields map to the intended registers and stack-frame values.

The dispatcher currently uses the frame's vector, error code, and RIP for its diagnostics. It then halts. Since it does not return to the interrupted code, this implementation is a fatal-reporting path rather than a recoverable exception framework.

### Page-fault details

The CPU stores the page-fault address in CR2. The current dispatcher does not print CR2. Adding it safely requires the assembly entry path to capture the value without corrupting the interrupted register state, plus a matching C frame definition and offset checks.

**Partial:** page faults reach the fatal exception path, but address-aware page-fault diagnostics and virtual-memory recovery are not implemented by this dispatcher.

## 3. Panic path

The public `panic(message)` function:

1. Disables maskable interrupts.
2. Writes a panic banner and reason directly to serial output.
3. Calls the assembly `register_capture` helper to fill an `ORIGIN_REGISTER_CONTEXT`.
4. Prints general-purpose registers and the captured RSP, RIP, and RFLAGS.
5. Prints exception vector and error code if the context marks exception metadata as available.
6. Halts the processor.

The panic formatter emits fixed-width 64-bit hexadecimal values with a `0x` prefix. This keeps formatting small and avoids depending on a general-purpose formatted-I/O implementation.

### Register-capture caveat

Because `register_capture` is called from C, its assembly implementation determines exactly which machine state is saved. Registers may describe the capture call site rather than the original point of failure unless the routine explicitly preserves the required state. The C structure and assembly layout must be audited together before treating the output as a precise crash dump.

### Serial output and robustness

Serial output is used because a fatal error may happen before the graphical console is initialized or when console state is unreliable. However, serial output itself still depends on the serial driver and hardware being usable.

**Future improvement:** provide an emergency panic-output path with minimal dependencies, and consider a dedicated panic stack for cases where the current kernel stack may be damaged. Neither should be considered implemented until integrated and tested.

## 4. Halt behavior

The current fatal paths do not attempt to resume normal execution. They disable interrupts and execute a `cli; hlt` loop. This is appropriate for early fatal diagnostics, but it is not a substitute for recovery, scheduling, or orderly shutdown support.

## 5. Safe development checklist

When changing exception or panic code:

- Keep assembly stack layout and C frame fields synchronized.
- Verify structure offsets and sizes with compile-time assertions.
- Remember which exceptions push an error code automatically.
- Preserve interrupted register state before using scratch registers in an entry stub.
- Confirm stack alignment before calling C from assembly.
- Test deliberate faults in an emulator before relying on diagnostics on physical hardware.
- Keep claims in documentation aligned with what has actually been built and tested.
