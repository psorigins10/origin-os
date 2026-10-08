#ifndef ORIGIN_FRAMEBUFFER_COLOR_H
#define ORIGIN_FRAMEBUFFER_COLOR_H

#include <stdint.h>

typedef enum
{
    ORIGIN_COLOR_WHITE,
    ORIGIN_COLOR_RED,
    ORIGIN_COLOR_GREEN,
    ORIGIN_COLOR_BLUE,
    ORIGIN_COLOR_YELLOW,
    ORIGIN_COLOR_BLACK
} origin_color_t;

static inline uint32_t framebuffer_color_value(origin_color_t color)
{
    switch (color)
    {
        case ORIGIN_COLOR_WHITE:
            return 0x00FFFFFF;

        case ORIGIN_COLOR_RED:
            return 0x00FF0000;

        case ORIGIN_COLOR_GREEN:
            return 0x0000FF00;

        case ORIGIN_COLOR_BLUE:
            return 0x000000FF;

        case ORIGIN_COLOR_YELLOW:
            return 0x00FFFF00;

        case ORIGIN_COLOR_BLACK:
            return 0x00000000;

        default:
            return 0x00FFFFFF;
    }
}

#endif