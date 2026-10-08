#include "origin/boot/boot_info.h"
#include "origin/video/framebuffer.h"
#include "origin/printk/printk.h"
#include "origin/console/console.h"
#include "origin/video/color/framebuffer_color.h"

/*
 * Kernel entry point.
 *
 * The bootloader provides the kernel with information about the
 * system through ORIGIN_BOOT_INFO.
 */
void kernel_main(ORIGIN_BOOT_INFO *BootInfo)
{
    /*
     * A valid boot information structure is required before
     * the kernel can access any bootloader-provided resources.
     */
    if (BootInfo == NULL)
    {
        for (;;)
            __asm__ volatile ("cli; hlt");
    }

    /*
     * Initialize the framebuffer before using any console output.
     */
    framebuffer_init(BootInfo);
    console_init(BootInfo);
    
    /*
    * Verify that the boot information belongs to ORIGIN OS.
    */
   if (BootInfo->magic != ORIGIN_BOOT_MAGIC)
   {
       console_clear();
       
       console_headLine(
           "BOOT ERROR : Invalid Magic",
           40,
           40,
           2,
           ORIGIN_COLOR_WHITE
        );
        
        for (;;)
        __asm__ volatile ("cli; hlt");
    }
    
    /*
    * Verify that the boot protocol version is supported
    * by this kernel.
    */
   if (BootInfo->boot_protocol_major != ORIGIN_BOOT_PROTOCOL_MAJOR)
   {
       console_clear();
       
       console_headLine(
           "BOOT ERROR : Invalid Protocol",
           40,
           40,
           2,
           ORIGIN_COLOR_WHITE
        );
        
        for (;;)
        __asm__ volatile ("cli; hlt");
    }
    
    /*
    * Start the kernel console after all basic boot information
    * has been validated.
    */
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

    /*
     * Keep the processor halted until the next kernel subsystem
     * takes control of execution.
     */
    for (;;)
        __asm__ volatile ("hlt");
}