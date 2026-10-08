#ifndef ORIGIN_FRAMEBUFFER_H
#define ORIGIN_FRAMEBUFFER_H

#include <stdint.h>

#include "origin/boot/boot_info.h"

/*
 * Initialize the framebuffer using information provided by the bootloader.
 */
void framebuffer_init(ORIGIN_BOOT_INFO *boot_info);

/*
 * Clear the entire framebuffer with the specified color.
 */
void framebuffer_clear(uint32_t color);

/*
 * Draw a single character using the 8x16 terminal font.
 */
void framebuffer_draw_character(
    const char character,
    uint32_t pixel_x,
    uint32_t pixel_y,
    uint32_t color
);

/*
 * Draw a string using the scalable 8x8 font.
 */
void framebuffer_draw_string(
    const char *string,
    uint32_t x,
    uint32_t y,
    uint32_t scale,
    uint32_t color
);

#endif