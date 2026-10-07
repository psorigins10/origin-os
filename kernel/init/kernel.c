#include "origin/boot/boot_info.h"

static ORIGIN_BOOT_INFO *g_bi;

static const uint8_t *glyph(char c)
{
    static const uint8_t sp[8] = {0};
    static const uint8_t W[8] = {0xC6,0xC6,0xC6,0xD6,0xFE,0xEE,0xC6,0x00};
    static const uint8_t e[8] = {0x00,0x00,0x7C,0xC6,0xFE,0xC0,0xC6,0x7C};
    static const uint8_t l[8] = {0x70,0x30,0x30,0x30,0x30,0x30,0x78,0x00};
    static const uint8_t c_[8] = {0x00,0x00,0x7C,0xC6,0xC0,0xC6,0x7C,0x00};
    static const uint8_t o[8] = {0x00,0x00,0x7C,0xC6,0xC6,0xC6,0x7C,0x00};
    static const uint8_t m[8] = {0x00,0x00,0xEC,0xFE,0xD6,0xD6,0xC6,0x00};
    static const uint8_t t[8] = {0x30,0x30,0xFC,0x30,0x30,0x30,0x1C,0x00};
    static const uint8_t O[8] = {0x7C,0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x00};
    static const uint8_t R[8] = {0xFC,0xC6,0xC6,0xFC,0xD8,0xCC,0xC6,0x00};
    static const uint8_t I[8] = {0x7E,0x18,0x18,0x18,0x18,0x18,0x7E,0x00};
    static const uint8_t G[8] = {0x7C,0xC6,0xC0,0xCE,0xC6,0xC6,0x7E,0x00};
    static const uint8_t N[8] = {0xC6,0xE6,0xF6,0xDE,0xCE,0xC6,0xC6,0x00};
    static const uint8_t S[8] = {0x7C,0xC6,0xC0,0x7C,0x06,0xC6,0x7C,0x00};

    switch (c) {
        case 'W': return W;  case 'e': return e;  case 'l': return l;
        case 'c': return c_; case 'o': return o;  case 'm': return m;
        case 't': return t;  case 'O': return O;  case 'R': return R;
        case 'I': return I;  case 'G': return G;  case 'N': return N;
        case 'S': return S;  default:  return sp;
    }
}

static void put_pixel(uint32_t x, uint32_t y, uint32_t color)
{
    if (x >= g_bi->fb_width || y >= g_bi->fb_height) return;
    volatile uint32_t *fb = (volatile uint32_t *)(uintptr_t)g_bi->fb_base;
    fb[y * g_bi->fb_pixels_per_scanline + x] = color;
}

static void draw_char(char c, uint32_t x, uint32_t y, uint32_t scale, uint32_t color)
{
    const uint8_t *g = glyph(c);
    for (uint32_t row = 0; row < 8; row++)
        for (uint32_t col = 0; col < 8; col++)
            if (g[row] & (0x80 >> col))
                for (uint32_t dy = 0; dy < scale; dy++)
                    for (uint32_t dx = 0; dx < scale; dx++)
                        put_pixel(x + col * scale + dx, y + row * scale + dy, color);
}

static void draw_string(const char *s, uint32_t x, uint32_t y, uint32_t scale, uint32_t color)
{
    for (; *s; s++, x += 8 * scale)
        draw_char(*s, x, y, scale, color);
}

void kernel_main(ORIGIN_BOOT_INFO *BootInfo)
{
    g_bi = BootInfo;

    /* clear screen to black */
    for (uint32_t y = 0; y < BootInfo->fb_height; y++)
        for (uint32_t x = 0; x < BootInfo->fb_width; x++)
            put_pixel(x, y, 0x00000000);

    /* 0x00FFFFFF is white in both RGB and BGR, so no format handling yet */
    draw_string("Welcome to ORIGIN OS", 40, 40, 3, 0x00FFFFFF);

    for (;;) __asm__ volatile ("hlt");
}