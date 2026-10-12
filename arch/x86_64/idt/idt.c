/*
 * ORIGIN OS — x86-64 Interrupt Descriptor Table (IDT).
 *
 * Builds the IDT, installs handlers for the exceptions currently
 * supported by the kernel, and loads the table using LIDT.
 */

#include <stddef.h>
#include <stdint.h>

#include <origin/arch/x86_64/idt/idt.h>

/*
 * Present, ring-0, 64-bit interrupt gate.
 *
 * DPL 0 restricts software-triggered invocation of these gates
 * to ring 0. Interrupt gates also clear IF on entry.
 */
#define IDT_GATE_PRESENT_RING0_INTERRUPT \
    (ORIGIN_IDT_PRESENT | ORIGIN_IDT_DPL0 | \
     ORIGIN_IDT_INTERRUPT_GATE)

/* The CPU expects each IDT gate to be 16 bytes in 64-bit mode. */
_Static_assert(sizeof(IDT_GATE) == 16,
    "x86-64 IDT gate must be 16 bytes");

/* The IDT is aligned to simplify CPU access and inspection. */
static IDT_GATE idt[ORIGIN_IDT_ENTRIES]
    __attribute__((aligned(16)));

static IDTR idtr;

/* Assembly helpers for loading and inspecting the IDTR. */
extern void idt_load(const IDTR *idtr_pointer);
extern void idt_read(IDTR *idtr_pointer);

/* Exception entry points implemented in assembly. */
extern void divide_error_handler(void);
extern void double_fault_handler(void);
extern void general_protection_handler(void);
extern void page_fault_handler(void);

/*
 * Populate one IDT entry.
 *
 * vector:          CPU exception or interrupt vector.
 * handler_address: 64-bit address of the entry handler.
 * ist_index:       IST index from 0 to 7; 0 disables IST switching.
 */
static void idt_set_gate(
    uint8_t vector,
    uintptr_t handler_address,
    uint8_t ist_index
)
{
    IDT_GATE *gate = &idt[vector];

    gate->offset_low =
        (uint16_t)(handler_address & 0xFFFF);

    gate->selector = ORIGIN_GDT_KERNEL_CODE_SELECTOR;

    /* Only the low three bits encode the IST index. */
    gate->ist = ist_index & 0x07;

    gate->type_attributes = IDT_GATE_PRESENT_RING0_INTERRUPT;

    gate->offset_middle =
        (uint16_t)((handler_address >> 16) & 0xFFFF);

    gate->offset_high =
        (uint32_t)((handler_address >> 32) & 0xFFFFFFFF);

    gate->reserved = 0;
}

/*
 * Initialize and load the IDT.
 *
 * Unconfigured entries remain absent. Only the exception vectors
 * with implemented handlers are marked present.
 */
void idt_init(void)
{
    for (size_t i = 0; i < ORIGIN_IDT_ENTRIES; ++i)
    {
        idt[i] = (IDT_GATE){0};
    }

    idtr.limit = (uint16_t)(sizeof(idt) - 1);
    idtr.base = (uint64_t)(uintptr_t)idt;

    /* Vector 0: Divide Error (#DE). */
    idt_set_gate(
        0,
        (uintptr_t)divide_error_handler,
        0
    );

    /* Vector 8: Double Fault (#DF), using IST1. */
    idt_set_gate(
        8,
        (uintptr_t)double_fault_handler,
        1
    );

    /* Vector 13: General Protection Fault (#GP). */
    idt_set_gate(
        13,
        (uintptr_t)general_protection_handler,
        0
    );

    /* Vector 14: Page Fault (#PF). */
    idt_set_gate(
        14,
        (uintptr_t)page_fault_handler,
        0
    );

    /* Make the initialized IDT active on this CPU. */
    idt_load(&idtr);
}

/*
 * Debug helper: read the IDTR register into a local structure.
 *
 * This function is intended for GDB inspection, not normal
 * kernel operation. The compiler barrier keeps the local object's
 * address observable at this point during debugging.
 */
void idt_debug_read(void)
{
    IDTR actual = {0};

    idt_read(&actual);

    __asm__ volatile ("" : : "r"(&actual) : "memory");
}