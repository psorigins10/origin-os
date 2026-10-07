#include "origin/boot/boot_info.h"
#include "origin/video/framebuffer.h"

void kernel_main(ORIGIN_BOOT_INFO *BootInfo)
{
    if (BootInfo == NULL) {
        for (;;)
            __asm__ volatile ("cli; hlt");
    }

    framebuffer_init(BootInfo);

    if (BootInfo->magic != ORIGIN_BOOT_MAGIC) {

        framebuffer_clear(0x00000000);

        framebuffer_draw_string(
            "BOOT ERROR : Invalid Magic",
            40,
            40,
            2,
            0x00FFFFFF
        );

        for (;;)
            __asm__ volatile ("cli; hlt");
    }

    if (BootInfo->boot_protocol_major != ORIGIN_BOOT_PROTOCOL_MAJOR) {

        framebuffer_clear(0x00000000);

        framebuffer_draw_string(
            "BOOT ERROR : Invalid PROTOCOL",
            40,
            40,
            2,
            0x00FFFFFF
        );

        for (;;)
            __asm__ volatile ("cli; hlt");
    }

    framebuffer_clear(0x00000000);

    framebuffer_draw_string(
        "Welcome to ORIGIN OS",
        40,
        40,
        3,
        0x00FFFFFF
    );

    for (;;)
        __asm__ volatile ("hlt");
}