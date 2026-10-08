#include "origin/video/framebuffer.h"
#include "origin/video/font8x16.h"
#include "origin/video/font8x8.h"

static ORIGIN_BOOT_INFO *g_fb;

/*
 * Store the bootloader-provided framebuffer information for later drawing.
 */
void framebuffer_init(ORIGIN_BOOT_INFO *boot_info)
{
    g_fb = boot_info;
}

/*
 * Write a single pixel to the framebuffer.
 *
 * Coordinates outside the framebuffer are ignored to prevent
 * out-of-bounds memory access.
 */
static void put_pixel(
    uint32_t x,
    uint32_t y,
    uint32_t color
)
{
    if (g_fb == 0)
        return;

    if (x >= g_fb->fb_width || y >= g_fb->fb_height)
        return;

    volatile uint32_t *fb =
        (volatile uint32_t *)(uintptr_t)g_fb->fb_base;

    fb[y * g_fb->fb_pixels_per_scanline + x] = color;
}

/*
 * Draw one character using the scalable 8x8 font.
 */
static void draw_char(
    char character,
    uint32_t x,
    uint32_t y,
    uint32_t scale,
    uint32_t color
)
{
    if ((unsigned char)character >= 128)
        return;

    const uint8_t *glyph = font[(unsigned char)character];

    for (uint32_t row = 0; row < 8; row++)
    {
        for (uint32_t col = 0; col < 8; col++)
        {
            if (glyph[row] & (0x80 >> col))
            {
                for (uint32_t dy = 0; dy < scale; dy++)
                {
                    for (uint32_t dx = 0; dx < scale; dx++)
                    {
                        put_pixel(
                            x + col * scale + dx,
                            y + row * scale + dy,
                            color
                        );
                    }
                }
            }
        }
    }
}

/*
 * Draw a string using the scalable 8x8 font.
 */
void framebuffer_draw_string(
    const char *string,
    uint32_t x,
    uint32_t y,
    uint32_t scale,
    uint32_t color
)
{
    if (string == 0 || scale == 0)
        return;

    while (*string != '\0')
    {
        draw_char(
            *string,
            x,
            y,
            scale,
            color
        );

        x += 8 * scale;
        string++;
    }
}

/*
 * Fill the entire framebuffer with a single color.
 */
void framebuffer_clear(uint32_t color)
{
    if (g_fb == 0)
        return;

    for (uint32_t y = 0; y < g_fb->fb_height; y++)
    {
        for (uint32_t x = 0; x < g_fb->fb_width; x++)
        {
            put_pixel(x, y, color);
        }
    }
}

/*
 * Draw one character using the fixed-size 8x16 terminal font.
 */
void framebuffer_draw_character(
    const char character,
    uint32_t pixel_x,
    uint32_t pixel_y,
    uint32_t color
)
{
    if (character == 0)
        return;

    const uint8_t *glyph = font8x16[(uint8_t)character];

    for (uint32_t row = 0; row < 16; row++)
    {
        uint8_t row_data = glyph[row];

        for (uint32_t col = 0; col < 8; col++)
        {
            if (row_data & (0x80 >> col))
            {
                put_pixel(
                    pixel_x + col,
                    pixel_y + row,
                    color
                );
            }
        }
    }
}