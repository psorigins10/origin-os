#ifndef ORIGIN_CONSOLE_H
#define ORIGIN_CONSOLE_H

#include <stdint.h>

#include "origin/video/color/framebuffer_color.h"

/*
 * Initialize the console cursor position.
 */
void console_init(void);

/*
 * Write text using the fixed-size terminal font.
 *
 * Newline characters advance the console to the next line.
 */
void console_writeLine(
    const char *string,
    origin_color_t color
);

/*
 * Draw a scalable heading at the specified framebuffer position.
 *
 * The heading uses the scalable 8x8 font and does not use the
 * normal console cursor for its drawing position.
 */
void console_headLine(
    const char *string,
    uint32_t x,
    uint32_t y,
    uint32_t scale,
    origin_color_t color
);

/*
 * Clear the console and reset the cursor position.
 */
void console_clear(void);

#endif