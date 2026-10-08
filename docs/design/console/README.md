# Console

The ORIGIN OS console subsystem provides early kernel text output
through the framebuffer.

The current console is intended for kernel initialization messages,
status output, headings, and basic debugging output.


## 1. Architecture

The current output path is:

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

The console owns text positioning and logical colors.

The framebuffer owns pixel-level rendering and framebuffer access.


## 2. Current Components

### Console

Header:

    include/origin/console/console.h

Implementation:

    kernel/console/console.c

Provides:

    console_init()
    console_writeLine()
    console_headLine()
    console_clear()


### Framebuffer

Header:

    include/origin/video/framebuffer.h

Implementation:

    kernel/video/framebuffer.c

Provides framebuffer initialization, pixel access, character
rendering, string rendering, and framebuffer clearing.


### Printk

Header:

    include/origin/printk/printk.h

Implementation:

    kernel/printk/printk.c

Provides:

    printk()
    printk_color()

`printk()` is the default kernel text output interface.

`printk_color()` allows kernel messages to specify a logical color.


### Fonts

Fixed-size terminal font:

    include/origin/video/font8x16.h

Scalable heading font:

    include/origin/video/font8x8.h


### Colors

Header:

    include/origin/video/color/framebuffer_color.h

Provides the logical `origin_color_t` color type and conversion to
framebuffer color values.


## 3. Rendering Paths

Normal console output uses the fixed 8x16 terminal font:

    printk()
        |
        v
    console_writeLine()
        |
        v
    framebuffer_draw_character()
        |
        v
    font8x16[]
        |
        v
    framebuffer


Headings use the scalable 8x8 font:

    console_headLine()
        |
        v
    framebuffer_draw_string()
        |
        v
    font8x8[]
        |
        v
    framebuffer


## 4. Console Coordinates

Normal console text uses character-cell coordinates.

Each terminal cell occupies:

    8 x 16 pixels

The console therefore stores its cursor as:

    cursor_x
    cursor_y

Framebuffer coordinates are derived from the cursor position.

Scalable headings use framebuffer pixel coordinates directly.


## 5. Current Behavior

The current console supports:

- Fixed-size 8x16 terminal text.
- Scalable 8x8 headings.
- Newline handling.
- Logical console colors.
- Framebuffer clearing.
- Console cursor tracking.
- Basic framebuffer bounds checking.


## 6. Current Limitations

The current console does not yet provide:

- Automatic horizontal line wrapping.
- Vertical scrolling.
- Dynamic console dimensions.
- A text backbuffer.
- Terminal control sequences.
- Input handling.
- Glyph caching.


## 7. Documentation

Detailed console documentation is divided into the following files:

    architecture.md

Describes subsystem ownership, component boundaries, cursor state,
data flow, and current architecture.

    rendering.md

Describes glyph representation, character rendering, scaling,
coordinate conversion, and pixel generation.

    api.md

Documents the public console API, parameters, behavior, and side
effects.


## 8. Source of Truth

The console implementation is located in:

    include/origin/console/
    include/origin/video/
    kernel/console/
    kernel/video/
    kernel/printk/

The documentation describes the current implementation and should be
updated when console behavior or public interfaces change.