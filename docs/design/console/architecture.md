# Console Architecture

## 1. Overview

The ORIGIN OS console subsystem provides kernel-level text output on the
framebuffer.

The console subsystem is implemented as a layer between the kernel message
interface (`printk`) and the framebuffer rendering subsystem.

Current output path:

    printk
        |
        v
    console
        |
        v
    framebuffer
        |
        v
    framebuffer memory


## 2. Subsystem Layers

The console output system is divided into three layers.

### 2.1 printk Layer

Location:

    include/origin/printk/printk.h
    kernel/printk/printk.c

The printk layer provides the kernel-facing message output interface.

Implemented interfaces:

    void printk(const char *message);

    void printk_color(
        const char *message,
        origin_color_t color
    );

`printk()` outputs messages using `ORIGIN_COLOR_WHITE`.

`printk_color()` accepts a logical console color and forwards the message
to the console subsystem.


### 2.2 Console Layer

Location:

    include/origin/console/console.h
    kernel/console/console.c

The console layer provides text-oriented output management.

The console layer maintains two cursor coordinates:

    cursor_x
    cursor_y

The cursor coordinates represent character cells rather than framebuffer
pixel coordinates.

The current terminal cell size is:

    width  = 8 pixels
    height = 16 pixels

The console converts character-cell coordinates into framebuffer pixel
coordinates before calling the framebuffer rendering functions.

Implemented interfaces:

    void console_init(void);

    void console_writeLine(
        const char *string,
        origin_color_t color
    );

    void console_headLine(
        const char *string,
        uint32_t x,
        uint32_t y,
        uint32_t scale,
        origin_color_t color
    );

    void console_clear(void);


### 2.3 Framebuffer Layer

Location:

    include/origin/video/framebuffer.h
    kernel/video/framebuffer.c

The framebuffer layer provides the low-level drawing interface used by the
console.

It receives framebuffer pixel colors as `uint32_t` values and performs
pixel-level rendering.

Implemented interfaces include:

    void framebuffer_init(ORIGIN_BOOT_INFO *boot_info);

    void framebuffer_clear(uint32_t color);

    void framebuffer_draw_character(
        const char character,
        uint32_t pixel_x,
        uint32_t pixel_y,
        uint32_t color
    );

    void framebuffer_draw_string(
        const char *string,
        uint32_t x,
        uint32_t y,
        uint32_t scale,
        uint32_t color
    );


## 3. Console Cursor Model

The console maintains its position using character-cell coordinates.

For normal terminal output:

    pixel_x = cursor_x * 8
    pixel_y = cursor_y * 16

A normal character therefore occupies an 8x16 framebuffer cell.

When a newline character is encountered:

    cursor_x = 0
    cursor_y++

The newline character itself is not rendered.

After rendering a normal character:

    cursor_x++

The current implementation does not yet perform automatic horizontal
wrapping when `cursor_x` reaches the framebuffer width.


## 4. Text Rendering Paths

The console currently uses two separate text rendering paths.

### 4.1 Normal Console Text

Normal console output uses the fixed-size 8x16 font.

Flow:

    console_writeLine()
        |
        v
    framebuffer_draw_character()
        |
        v
    font8x16
        |
        v
    put_pixel()


### 4.2 Scalable Headings

`console_headLine()` uses the scalable 8x8 font.

Flow:

    console_headLine()
        |
        v
    framebuffer_draw_string()
        |
        v
    8x8 font
        |
        v
    put_pixel()

The scalable character size is determined by:

    rendered_width  = 8 * scale
    rendered_height = 8 * scale

The heading is positioned using framebuffer pixel coordinates rather than
console cursor coordinates.


## 5. Heading and Console Cursor Synchronization

After rendering a heading, `console_headLine()` updates the normal console
cursor so subsequent console output starts below the rendered heading.

The heading bottom position is calculated as:

    heading_bottom = y + (8 * scale)

The console row is then calculated using ceiling division by the normal
16-pixel console cell height:

    cursor_y = (heading_bottom + 15) / 16

The horizontal cursor is reset:

    cursor_x = 0

This prevents subsequent normal console output from starting inside the
vertical area occupied by the heading.


## 6. Color Handling

The console API uses the logical color type:

    origin_color_t

Framebuffer drawing functions operate on raw `uint32_t` framebuffer colors.

Color conversion is performed before entering the framebuffer rendering
layer:

    origin_color_t
        |
        v
    framebuffer_color_value()
        |
        v
    uint32_t framebuffer color

This keeps logical console colors separate from framebuffer-specific pixel
values.

The framebuffer layer does not perform logical color conversion.


## 7. Framebuffer Access

The framebuffer is initialized using `ORIGIN_BOOT_INFO` provided by the
bootloader.

The framebuffer implementation stores this information internally:

    static ORIGIN_BOOT_INFO *g_fb;

Pixel writes are performed using:

    fb[y * fb_pixels_per_scanline + x]

Coordinates are checked against:

    fb_width
    fb_height

before a pixel is written.

Out-of-range pixel coordinates are ignored.


## 8. Console Clearing

`console_clear()` performs two operations:

1. Clears the framebuffer using `ORIGIN_COLOR_BLACK`.
2. Resets the console cursor.

After clearing:

    cursor_x = 0
    cursor_y = 0

The console therefore returns to the top-left terminal position.


## 9. Current Component Boundaries

The current subsystem follows these ownership rules:

    printk
        |
        | message interface
        v
    console
        |
        | text and cursor management
        v
    framebuffer
        |
        | pixel rendering
        v
    framebuffer memory

`printk` does not directly access the framebuffer.

The console does not directly access framebuffer memory.

The framebuffer subsystem does not manage console cursor state.

This separates message output, terminal behavior, and pixel rendering.


## 10. Current Implementation

Implemented:

- Framebuffer initialization from boot information.
- Direct framebuffer pixel writes.
- Framebuffer clearing.
- Fixed-size 8x16 character rendering.
- Scalable 8x8 string rendering.
- Console cursor tracking.
- Newline handling.
- Console clearing.
- Logical console colors.
- Colored kernel output.
- `printk()` interface.
- `printk_color()` interface.
- Scalable console headings.
- Cursor synchronization after heading rendering.


## 11. Current Limitations

The current console implementation does not yet provide:

- Automatic horizontal line wrapping.
- Automatic vertical scrolling.
- Console bounds management based on framebuffer dimensions.
- A text backbuffer.
- Character-cell clipping beyond framebuffer pixel bounds.
- Terminal control sequences.
- Input handling.


## 12. Next Implementation Work

The next console implementation work is:

1. Expose framebuffer dimensions through the framebuffer interface.
2. Calculate the available console columns from framebuffer width.
3. Calculate the available console rows from framebuffer height.
4. Implement automatic horizontal wrapping.
5. Handle the console cursor when the bottom of the framebuffer is reached.

These changes will allow the console to operate using the actual framebuffer
dimensions instead of relying on fixed display assumptions.