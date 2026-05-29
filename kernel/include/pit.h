#ifndef PIT_H
#define PIT_H

#include <stdint.h>

extern volatile uint64_t g_ticks;

void pit_configure_hz(uint32_t hz);
void timer_on_irq0(void);

#endif
