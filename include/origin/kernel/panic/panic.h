#ifndef ORIGIN_KERNEL_PANIC_PANIC_H
#define ORIGIN_KERNEL_PANIC_PANIC_H

#if defined(__cplusplus)
#define ORIGIN_NORETURN [[noreturn]]
#else
#include <stdnoreturn.h>
#define ORIGIN_NORETURN _Noreturn
#endif

/*
 * Report a fatal kernel error and halt execution.
 *
 * @param message Description of the fatal error.
 */
ORIGIN_NORETURN void panic(const char *message);

#endif