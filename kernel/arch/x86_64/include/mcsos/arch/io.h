#ifndef MCSOS_ARCH_IO_H
#define MCSOS_ARCH_IO_H

#include <stdint.h>

static inline void outb(
    uint16_t port,
    uint8_t value
)
{
    __asm__ volatile(
        "outb %0, %1"
        :
        : "a"(value), "Nd"(port)
    );
}

static inline uint8_t inb(
    uint16_t port
)
{
    uint8_t value;

    __asm__ volatile(
        "inb %1, %0"
        : "=a"(value)
        : "Nd"(port)
    );

    return value;
}

static inline void cli(void)
{
    __asm__ volatile("cli");
}

static inline void sti(void)
{
    __asm__ volatile("sti");
}

static inline void hlt(void)
{
    __asm__ volatile("hlt");
}

static inline uint16_t read_cs(void)
{
    uint16_t cs;

    __asm__ volatile(
        "mov %%cs, %0"
        : "=r"(cs)
    );

    return cs;
}

#endif
