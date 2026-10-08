#include "origin/printk/printk.h"
#include "origin/console/console.h"

/*
 * Print a message using the default console color.
 */
void printk(const char *message)
{
    console_writeLine(
        message,
        ORIGIN_COLOR_WHITE
    );
}

/*
 * Print a message using the specified console color.
 */
void printk_color(
    const char *message,
    origin_color_t color
)
{
    console_writeLine(
        message,
        color
    );
}