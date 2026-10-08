#include "origin/console/console.h"
#include "origin/video/framebuffer.h"
#include "origin/video/color/framebuffer_color.h"

static uint32_t cursor_x;
static uint32_t cursor_y;
static uint32_t console_columns;

/*
 * Initialize the console cursor at the top-left corner.
 */
void console_init(ORIGIN_BOOT_INFO *boot_info) {
    cursor_x = 0;
    cursor_y = 0;
    console_columns = boot_info->fb_width / 8;
}

/*
 * Write text using the fixed-size 8x16 terminal font.
 *
 * Newline characters reset the horizontal cursor position and
 * advance the cursor to the next terminal row.
 */
void console_writeLine(
    const char *string,
    origin_color_t color
)
{
    if (string == 0)
        return;

    while (*string != '\0')
    {
        if (*string == '\n')
        {
            cursor_x = 0;
            cursor_y++;
        }
        else
        {
            framebuffer_draw_character(
                *string,
                cursor_x * 8,
                cursor_y * 16,
                framebuffer_color_value(color)
            );

            cursor_x++;
            if (cursor_x >= console_columns) {
                cursor_x = 0;
                cursor_y++;
            }
        }

        string++;
    }
}

/*
 * Draw a scalable heading using the 8x8 font.
 *
 * After drawing, move the console cursor below the heading
 * so subsequent console text does not overlap it vertically.
 */
void console_headLine(
    const char *string,
    uint32_t x,
    uint32_t y,
    uint32_t scale,
    origin_color_t color
)
{
    if (string == 0 || scale == 0)
        return;

    framebuffer_draw_string(
        string,
        x,
        y,
        scale,
        framebuffer_color_value(color)
    );

    cursor_x = 0;
    cursor_y = (y + (8 * scale) + 15) / 16;
}

/*
 * Clear the framebuffer and reset the console cursor.
 */
void console_clear(void)
{
    framebuffer_clear(
        framebuffer_color_value(ORIGIN_COLOR_BLACK)
    );

    cursor_x = 0;
    cursor_y = 0;
}