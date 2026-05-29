#ifndef MCSOS_KERNEL_PANIC_H
#define MCSOS_KERNEL_PANIC_H

#include <stdint.h>

_Noreturn void kernel_panic_at(
    const char *message,
    uint64_t code,
    const char *file,
    uint64_t line
);

#endif
