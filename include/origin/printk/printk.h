#ifndef ORIGIN_PRINTK_H
#define ORIGIN_PRINTK_H

#include "origin/video/color/framebuffer_color.h"

/*
 * Print a message using the default console color.
 */
void printk(const char *message);

/*
 * Print a message using the specified console color.
 */
void printk_color(
    const char *message,
    origin_color_t color
);

#endif