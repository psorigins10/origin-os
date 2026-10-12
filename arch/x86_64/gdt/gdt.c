/*
 * ORIGIN OS — x86-64 Global Descriptor Table (GDT).
 *
 * Initializes the kernel code/data descriptors and the 64-bit
 * Task State Segment (TSS). The TSS provides IST1, which the IDT
 * uses to give the double-fault handler a dedicated stack.
 */

#include <stddef.h>
#include <stdint.h>

#include <origin/arch/x86_64/gdt/gdt.h>
#include <origin/arch/x86_64/gdt/tss.h>

#define IST_STACK_SIZE (16U * 1024U)

/* Assembly helpers for loading and inspecting descriptor registers. */
extern void gdt_load(const void *pointer);
extern void gdt_read(void *buffer);
extern void tss_load(uint16_t selector);
extern uint16_t tss_read(void);

/*
 * GDTR format in 64-bit mode:
 *   limit: 16 bits
 *   base:  64 bits
 *
 * The packed attribute is required because the CPU expects a
 * 10-byte pseudo-descriptor.
 */
typedef struct {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) GDT_POINTER;

_Static_assert(sizeof(GDT_POINTER) == 10,
    "GDT pointer must be 10 bytes");

/* The TSS must remain at a stable address after its descriptor is set. */
static ORIGIN_TSS tss __attribute__((aligned(16)));

/* Dedicated stack for the double-fault handler's IST1 entry. */
static uint8_t double_fault_stack[IST_STACK_SIZE]
    __attribute__((aligned(16)));

/*
 * GDT layout:
 *   Entry 0 (0x00): null descriptor
 *   Entry 1 (0x08): 64-bit kernel code segment
 *   Entry 2 (0x10): kernel data segment
 *   Entries 3-4 (0x18): 64-bit TSS descriptor
 *
 * The TSS descriptor occupies 16 bytes, unlike ordinary segment
 * descriptors, which occupy 8 bytes.
 */
static uint64_t gdt[5] __attribute__((aligned(8))) = {
    0x0000000000000000ULL, /* 0x00: Null descriptor. */
    0x00AF9A000000FFFFULL, /* 0x08: Kernel code segment. */
    0x00CF92000000FFFFULL, /* 0x10: Kernel data segment. */
    0x0000000000000000ULL, /* 0x18: TSS descriptor, low half. */
    0x0000000000000000ULL  /* 0x20: TSS descriptor, high half. */
};

/* GDTR value passed to the assembly loader. */
static GDT_POINTER gdt_pointer;

/*
 * Encode a 64-bit TSS descriptor into two consecutive GDT entries.
 *
 * base:  Address of the TSS structure.
 * limit: Last valid byte offset within the TSS.
 */
static void gdt_set_tss_descriptor(
    uint64_t *descriptor,
    uintptr_t base,
    uint32_t limit
)
{
    uint64_t low = 0;
    uint64_t high = 0;

    low |= (uint64_t)(limit & 0xFFFFU);
    low |= (uint64_t)(base & 0xFFFFFFU) << 16;
    low |= (uint64_t)0x89U << 40; /* Present, DPL 0, available 64-bit TSS. */
    low |= (uint64_t)((limit >> 16) & 0x0FU) << 48;
    low |= (uint64_t)((base >> 24) & 0xFFU) << 56;

    /* Upper 32 bits of the 64-bit TSS base address. */
    high = (uint64_t)(base >> 32);

    descriptor[0] = low;
    descriptor[1] = high;
}

/*
 * Initialize the GDT and TSS, load GDTR, and load the task register.
 *
 * The assembly loader is responsible for reloading the code segment
 * and the data segment registers as required by the architecture.
 */
void gdt_init(void)
{
    gdt_pointer.limit = (uint16_t)(sizeof(gdt) - 1U);
    gdt_pointer.base = (uint64_t)(uintptr_t)gdt;

    /* Initialize all TSS fields, including reserved fields, to zero. */
    tss = (ORIGIN_TSS){0};

    /* No I/O permission bitmap is provided after the TSS structure. */
    tss.iomap_base = (uint16_t)sizeof(ORIGIN_TSS);

    /*
     * The IST stack pointer must point to the stack's top, because
     * the CPU grows the stack downward.
     */
    tss.ist1 = (uint64_t)(uintptr_t)
        &double_fault_stack[IST_STACK_SIZE];

    /* The TSS limit is its size minus one. */
    gdt_set_tss_descriptor(
        &gdt[3],
        (uintptr_t)&tss,
        (uint32_t)(sizeof(tss) - 1U)
    );

    /* Load GDTR, then load TR with the TSS selector (0x18). */
    gdt_load(&gdt_pointer);
    tss_load(0x18);
}

/*
 * Debug helper: read GDTR for inspection in GDB.
 */
void gdt_debug_read(void)
{
    GDT_POINTER actual = {0};
    gdt_read(&actual);

    /* Keep the local result observable for debugger inspection. */
    __asm__ volatile ("" : : "r"(&actual) : "memory");
}

/*
 * Debug helper: return the current task-register selector.
 */
uint16_t gdt_debug_read_tr(void)
{
    return tss_read();
}