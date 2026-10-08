# Console API

## 1. Scope

This document defines the public API provided by the ORIGIN OS console
subsystem.

Public console interface:

    console_init()
    console_writeLine()
    console_headLine()
    console_clear()

Header:

    include/origin/console/console.h


## 2. `console_init()`

### Declaration

    void console_init(void);

### Description

Initializes the console cursor position.

After initialization:

    cursor_x = 0
    cursor_y = 0

The cursor therefore starts at the top-left console position.

### Parameters

None.

### Return Value

None.

### Side Effects

Resets the internal console cursor position.


## 3. `console_writeLine()`

### Declaration

    void console_writeLine(
        const char *string,
        origin_color_t color
    );

### Description

Writes a string using the fixed-size 8x16 terminal font.

Characters are rendered sequentially from the current console cursor
position.

### Parameters

`string`

Pointer to a null-terminated string.

If `string` is `NULL`, the function returns without performing any
operation.

`color`

Logical console color represented by `origin_color_t`.

The logical color is converted to a framebuffer color before rendering.

### Newline Handling

The newline character:

    '\n'

is not rendered.

Instead:

    cursor_x = 0
    cursor_y++

The next character is rendered at the beginning of the next console row.

### Cursor Behavior

After rendering a normal character:

    cursor_x++

The framebuffer position is calculated as:

    pixel_x = cursor_x * 8
    pixel_y = cursor_y * 16

using the cursor position before the character is rendered.

### Return Value

None.

### Current Limitations

The function does not currently perform automatic horizontal wrapping
or vertical scrolling.


## 4. `console_headLine()`

### Declaration

    void console_headLine(
        const char *string,
        uint32_t x,
        uint32_t y,
        uint32_t scale,
        origin_color_t color
    );

### Description

Renders a string using the scalable 8x8 font.

The heading is positioned using framebuffer pixel coordinates rather than
the current console cursor position.

### Parameters

`string`

Pointer to a null-terminated string.

If `string` is `NULL`, the function returns without performing any
operation.

`x`

Horizontal framebuffer pixel coordinate at which rendering begins.

`y`

Vertical framebuffer pixel coordinate at which rendering begins.

`scale`

Integer scale factor applied to the base 8x8 glyph dimensions.

A scale value of zero causes the function to return without rendering.

`color`

Logical console color represented by `origin_color_t`.

The logical color is converted to a framebuffer color before rendering.

### Rendering Dimensions

For a scale value of `scale`:

    width  = 8 * scale
    height = 8 * scale

per character.

### Cursor Behavior

After rendering, the console cursor is repositioned below the heading.

The cursor is updated using:

    cursor_x = 0

    cursor_y = (y + (8 * scale) + 15) / 16

This allows subsequent normal console output to begin below the vertical
space occupied by the heading.

### Return Value

None.

### Current Limitations

The scalable renderer does not currently interpret newline characters
as console control characters.


## 5. `console_clear()`

### Declaration

    void console_clear(void);

### Description

Clears the framebuffer using the logical black console color and resets
the console cursor.

### Parameters

None.

### Return Value

None.

### Side Effects

The framebuffer is cleared.

The cursor is reset:

    cursor_x = 0
    cursor_y = 0


## 6. Color Interface

The console API accepts:

    origin_color_t

Color definitions are provided by:

    include/origin/video/color/framebuffer_color.h

Logical colors are converted to framebuffer-specific `uint32_t` values
using:

    framebuffer_color_value()

The conversion occurs before calling the framebuffer rendering API.


## 7. API Dependencies

The console API depends on the framebuffer interface for rendering.

Console:

    include/origin/console/console.h

Framebuffer:

    include/origin/video/framebuffer.h

Color definitions:

    include/origin/video/color/framebuffer_color.h


## 8. API Usage

Example:

    console_init();

    console_writeLine(
        "ORIGIN OS\n",
        ORIGIN_COLOR_WHITE
    );

    console_writeLine(
        "Status: READY\n",
        ORIGIN_COLOR_GREEN
    );

Example heading:

    console_headLine(
        "Welcome to ORIGIN OS",
        40,
        40,
        3,
        ORIGIN_COLOR_WHITE
    );

Example clear:

    console_clear();