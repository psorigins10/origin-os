ENTRY(kernel_entry)

SECTIONS
{
    /*
     * ORIGIN kernel physical/virtual load address.
     */
    . = 0x100000;

    /*
     * Executable code.
     */
    .text ALIGN(0x1000) :
    {
        *(.text*)
    }

    /*
     * Read-only data.
     */
    .rodata ALIGN(0x1000) :
    {
        *(.rodata*)
    }

    /*
     * Initialized writable data.
     */
    .data ALIGN(0x1000) :
    {
        *(.data*)
    }

    /*
     * Zero-initialized data.
     */
    .bss ALIGN(0x1000) :
    {
        *(COMMON)
        *(.bss*)
    }

    /*
     * These sections have no place in our early kernel image.
     */
    /DISCARD/ :
    {
        *(.eh_frame*)
        *(.note*)
        *(.comment*)
    }
}