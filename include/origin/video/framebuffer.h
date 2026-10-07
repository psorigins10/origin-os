#ifndef ORIGIN_FRAMEBUFFER_H
#define ORIGIN_FRAMEBUFFER_H

#include <stdint.h>
#include "origin/boot/boot_info.h"

void framebuffer_init(ORIGIN_BOOT_INFO *boot_info);

void framebuffer_clear(uint32_t color);

void framebuffer_draw_string(
    const char *string,
    uint32_t x,
    uint32_t y,
    uint32_t scale,
    uint32_t color
);

#endif