#ifndef ORIGIN_BOOT_INFO_H
#define ORIGIN_BOOT_INFO_H

#include <stdint.h>

typedef struct {
    uint64_t memory_map;
    uint64_t memory_map_size;
    uint64_t memory_descriptor_size;

    uint64_t fb_base;
    uint64_t fb_size;
    uint32_t fb_width;
    uint32_t fb_height;
    uint32_t fb_pixels_per_scanline;
    uint32_t fb_pixel_format;   /* 0 = RGB, 1 = BGR */
} ORIGIN_BOOT_INFO;

#endif