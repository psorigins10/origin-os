/*
 * ORIGIN OS — Kernel initialization.
 *
 * Receives the boot information structure from the bootloader,
 * initializes early output and interrupt handling, validates the
 * boot protocol, and displays the kernel's ready message.
 */

#include <stddef.h>

#include "origin/boot/boot_info.h"
#include "origin/video/framebuffer.h"
#include "origin/printk/printk.h"
#include "origin/console/console.h"
#include "origin/video/color/framebuffer_color.h"
#include "origin/drivers/serial/serial.h"
#include "origin/kernel/panic/panic.h"
#include "origin/arch/x86_64/idt/idt.h"

/*
 * Stop execution safely when early kernel initialization fails.
 *
 * Interrupts are disabled because the kernel has not yet established
 * its complete interrupt-handling environment.
 */
static _Noreturn void kernel_halt(void) {
    for (;;)
        __asm__ volatile ("cli; hlt");
}

/*
 * Kernel entry point.
 *
 * BootInfo is supplied by the bootloader and describes resources
 * such as the framebuffer and memory map.
 */
void kernel_main(ORIGIN_BOOT_INFO *BootInfo)
{
    /* Initialize serial output for early diagnostics. */
    serial_init();

    /*
     * Reject a missing boot information structure before attempting
     * to access any of its fields.
     */
    if (BootInfo == NULL)
    {
        serial_write("BOOT ERROR: BootInfo is NULL\n");
        kernel_halt();
    }

    /*
     * Validate the boot information before passing it to framebuffer
     * or console initialization code.
     */
    if (BootInfo->magic != ORIGIN_BOOT_MAGIC)
    {
        serial_write("BOOT ERROR: Invalid boot information magic\n");
        kernel_halt();
    }

    if (BootInfo->boot_protocol_major != ORIGIN_BOOT_PROTOCOL_MAJOR)
    {
        serial_write("BOOT ERROR: Unsupported boot protocol version\n");
        kernel_halt();
    }

    /*
     * Initialize the framebuffer and console only after the basic
     * boot information checks have succeeded.
     */
    framebuffer_init(BootInfo);
    console_init(BootInfo);

    /* Initialize the interrupt descriptor table. */
    idt_init();

    /* Display the kernel's initial ready screen. */
    console_clear();

    console_headLine(
        "Welcome to ORIGIN OS",
        40,
        40,
        3,
        ORIGIN_COLOR_WHITE
    );

    printk("\nORIGIN OS\n");
    printk_color("Status: ", ORIGIN_COLOR_GREEN);
    printk("READY\n");
    printk("Kernel initialized successfully.\n");
    printk_color("System ready.\n", ORIGIN_COLOR_GREEN);

    /* Report successful initialization through the serial port. */
    serial_write("ORIGIN OS: READY to launch\n");

    /*
     * Keep the processor idle until another kernel subsystem,
     * such as the scheduler, takes control.
     */
    for (;;)
        __asm__ volatile ("hlt");
}