# Console Rendering

## 1. Overview

The console subsystem currently provides two text rendering paths:

1. Fixed-size 8x16 terminal character rendering.
2. Scalable 8x8 string rendering.

Both rendering paths ultimately write pixels through the framebuffer
subsystem.


## 2. Normal Character Rendering

Normal console text uses the 8x16 font stored in:

    include/origin/video/font8x16.h

A character is represented as 16 rows of 8 bits.

Each row represents one horizontal pixel row of the glyph.

Each bit determines whether the corresponding pixel is rendered.

The glyph dimensions are:

    width  = 8 pixels
    height = 16 pixels


## 3. Character Rendering Flow

The normal console rendering path is:

    console_writeLine()
        |
        v
    framebuffer_draw_character()
        |
        v
    font8x16[]
        |
        v
    glyph row data
        |
        v
    put_pixel()


`framebuffer_draw_character()` receives the framebuffer pixel position
and the already-converted framebuffer color.

For each glyph row:

    row = 0 .. 15

For each glyph column:

    column = 0 .. 7

A set bit in the glyph row causes the corresponding framebuffer pixel
to be written.


## 4. Glyph Bit Representation

Each glyph row is stored as an 8-bit value.

The most significant bit represents the first pixel column.

The bit mask used for column traversal is:

    0x80 >> column

For example, for:

    10000000

only the first pixel column is rendered.

For:

    11111111

all eight pixel columns are rendered.


## 5. Console Character Coordinates

The console stores its cursor position in character cells rather than
framebuffer pixels.

The normal terminal cell size is:

    8 x 16 pixels

The framebuffer position is calculated as:

    pixel_x = cursor_x * 8
    pixel_y = cursor_y * 16

This allows the console to manage text using rows and columns while the
framebuffer subsystem operates using pixel coordinates.


## 6. Scalable String Rendering

Headings use the scalable 8x8 font stored in:

    include/origin/video/font8x8.h

The base glyph dimensions are:

    width  = 8 pixels
    height = 8 pixels

A scale factor enlarges each source pixel into a square block.

For a scale value of `scale`:

    rendered_width  = 8 * scale
    rendered_height = 8 * scale


## 7. Scaled Pixel Expansion

The scalable renderer examines each pixel of the 8x8 glyph.

For every set glyph pixel, it writes a block of:

    scale x scale

framebuffer pixels.

The resulting framebuffer position is calculated as:

    x + column * scale + dx
    y + row * scale + dy

where:

    dx = 0 .. scale - 1
    dy = 0 .. scale - 1


## 8. Scalable String Rendering Flow

The scalable heading path is:

    console_headLine()
        |
        v
    framebuffer_draw_string()
        |
        v
    draw_char()
        |
        v
    font8x8[]
        |
        v
    put_pixel()


`framebuffer_draw_string()` renders characters sequentially.

After each character:

    x += 8 * scale

The vertical position remains unchanged for the duration of the string.


## 9. Character Spacing

For the fixed-size terminal renderer, each character occupies one:

    8 x 16

terminal cell.

For the scalable renderer, each character advances the horizontal
position by:

    8 * scale

The current scalable renderer does not implement additional character
spacing or proportional glyph widths.


## 10. Rendering Coordinates

Normal console rendering uses character-cell coordinates internally.

Scalable heading rendering uses direct framebuffer pixel coordinates.

Normal console:

    cursor_x
        |
        v
    cursor_x * 8
        |
        v
    framebuffer pixel X


    cursor_y
        |
        v
    cursor_y * 16
        |
        v
    framebuffer pixel Y


Scalable heading:

    x, y
        |
        v
    framebuffer pixel coordinates directly


## 11. Color Representation

The console API uses the logical color type:

    origin_color_t

Before a color reaches the framebuffer rendering functions, it is
converted to a raw framebuffer color value using:

    framebuffer_color_value()

The rendering layer therefore receives:

    uint32_t color

The framebuffer renderer does not perform logical color conversion.


## 12. Pixel Bounds

All framebuffer pixel writes pass through `put_pixel()`.

Before writing, the coordinates are checked against the framebuffer
dimensions:

    x < fb_width
    y < fb_height

Coordinates outside the framebuffer are ignored.

This prevents rendering operations from writing outside the known
framebuffer dimensions.


## 13. Rendering Limitations

The current rendering implementation has the following limitations:

- The terminal font is fixed at 8x16 pixels.
- The scalable font is based on an 8x8 glyph grid.
- The scalable renderer uses integer pixel scaling.
- The scalable renderer does not implement proportional character widths.
- The scalable renderer does not process newline characters.
- Character rendering does not perform terminal-level line wrapping.
- The framebuffer renderer does not maintain a text buffer.
- No glyph caching is implemented.
- No hardware-accelerated rendering is used.


## 14. Current Rendering Components

Font data:

    include/origin/video/font8x16.h
    include/origin/video/font8x8.h

Framebuffer interface:

    include/origin/video/framebuffer.h

Framebuffer implementation:

    kernel/video/framebuffer.c

Console interface:

    include/origin/console/console.h

Console implementation:

    kernel/console/console.c