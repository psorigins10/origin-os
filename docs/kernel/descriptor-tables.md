# x86-64 Descriptor Tables: GDT, TSS, and IDT

This page explains how ORIGIN OS currently configures the Global Descriptor Table (GDT), Task State Segment (TSS), and Interrupt Descriptor Table (IDT).

## 1. Global Descriptor Table (GDT)

The GDT is defined in the x86-64 GDT implementation under `arch/x86_64/gdt/`. The current table has five 8-byte slots:

| Slot | Selector | Purpose |
|---:|---:|---|
| 0 | `0x00` | Null descriptor |
| 1 | `0x08` | Kernel code segment |
| 2 | `0x10` | Kernel data segment |
| 3–4 | `0x18` | 16-byte 64-bit TSS descriptor |

The selector `0x18` points to slot 3. The TSS descriptor consumes two adjacent GDT slots because a 64-bit TSS descriptor stores a 64-bit base address.

The GDT pointer passed to the assembly loader contains a 16-bit limit and a 64-bit base. In 64-bit mode, the packed pseudo-descriptor is 10 bytes.

### TSS descriptor

The implementation builds the descriptor from the TSS base and limit. Its access byte is `0x89`: present, ring-0 privilege, and an available 64-bit TSS. The assembly helper loads the GDT, and `tss_load(0x18)` loads the task register (TR). Loading TR makes the descriptor busy; that is expected CPU behavior.

The assembly implementation of `gdt_load` is responsible for any required segment-register and code-segment reloads. When modifying this path, check the assembly routine as well as the C structure.

## 2. Task State Segment (TSS)

The packed `ORIGIN_TSS` structure follows the 104-byte 64-bit TSS layout defined by the architecture. It contains:

- `rsp0`–`rsp2`: stack pointers used for privilege-level transitions.
- `ist1`–`ist7`: Interrupt Stack Table stack pointers.
- Reserved fields that must remain at their defined offsets.
- `iomap_base`: offset of the optional I/O permission bitmap.

The current initialization zeroes the TSS and sets `iomap_base` to `sizeof(ORIGIN_TSS)`. No I/O permission bitmap is included in the structure.

### Double-fault stack

ORIGIN OS reserves a 16 KiB stack for double-fault handling and stores its top address in `tss.ist1`. The CPU grows the stack downward, so IST1 must point to the end of the reserved buffer, not its beginning.

The IDT entry for vector 8 selects IST index 1. This lets the processor switch to the dedicated stack when entering the double-fault handler, provided the TSS is loaded and the stack is valid.

## 3. Interrupt Descriptor Table (IDT)

Each 64-bit IDT gate is 16 bytes. The IDTR pseudo-descriptor contains a 16-bit limit and a 64-bit base and is packed to 10 bytes.

The current initialization clears all entries first, then installs these handlers:

| Vector | Exception | Handler | IST |
|---:|---|---|---:|
| 0 | Divide Error (#DE) | `divide_error_handler` | 0 |
| 8 | Double Fault (#DF) | `double_fault_handler` | 1 |
| 13 | General Protection Fault (#GP) | `general_protection_handler` | 0 |
| 14 | Page Fault (#PF) | `page_fault_handler` | 0 |

The installed gates are present, ring-0 interrupt gates. An interrupt gate clears the interrupt-enable flag (IF) on entry. IST value 0 means that the gate does not request an IST stack switch; value 1 selects IST1.

The table contains 256 entries, so its limit is `256 * 16 - 1 = 4095` (`0xFFF`). `idt_load` loads the IDTR using `LIDT`.

### Unhandled vectors

All entries other than the four listed above remain absent. This is suitable only for an early bring-up stage. An exception or interrupt that arrives on an absent entry can escalate into a double fault and, if that cannot be handled, a triple fault and processor reset.

**Not yet implemented:** a complete set of default exception handlers and a general interrupt-controller setup.

## 4. Initialization order

The kernel entry assembly calls `gdt_init` before entering `kernel_main`. The kernel then calls `idt_init` during early initialization. Preserve this ordering: the IDT gates use the kernel code selector, and the double-fault gate relies on the TSS and IST1 being configured before the IDT is used.

## 5. Debugging checks

When inspecting these structures in a debugger, useful invariants include:

- `CS = 0x08` and the kernel data selector is `0x10`.
- TR contains selector `0x18` after TSS loading.
- The GDT limit covers all five 8-byte slots: `0x27`.
- The IDT limit is `0xFFF` for 256 16-byte gates.
- IDT entries 0, 8, 13, and 14 have the expected handler addresses and attributes.
- The double-fault gate selects IST1, and the TSS's IST1 points to the top of the dedicated stack.

These are debugging expectations for the current layout, not hard-coded addresses; linked addresses can change between builds.
