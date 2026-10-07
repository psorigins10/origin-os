#ifndef ORIGIN_BOOT_INFO_H
#define ORIGIN_BOOT_INFO_H

#define ORIGIN_BOOT_MAGIC 0x4F524947494E424FULL

#define ORIGIN_BOOT_PROTOCOL_MAJOR 0
#define ORIGIN_BOOT_PROTOCOL_MINOR 1

#define ORIGIN_BOOTLOADER_MAJOR 0
#define ORIGIN_BOOTLOADER_MINOR 1

#ifndef NULL
#define NULL ((void *)0)
#endif

#include <stdint.h>

typedef struct {

    // Meta data
    uint64_t magic;

    uint16_t boot_protocol_major;
    uint16_t boot_protocol_minor;

    uint16_t bootloader_major;
    uint16_t bootloader_minor;

    // Memory
    uint64_t memory_map;
    uint64_t memory_map_size;
    uint64_t memory_descriptor_size;

    // Display
    uint64_t fb_base;
    uint64_t fb_size;
    uint32_t fb_width;
    uint32_t fb_height;
    uint32_t fb_pixels_per_scanline;
    uint32_t fb_pixel_format;   /* 0 = RGB, 1 = BGR */

    // Firmware/platform
    uint64_t acpi_rsdp;

    // Boot configuration
    uint64_t cmdline;
    uint64_t cmdline_size;
} ORIGIN_BOOT_INFO;

#endif