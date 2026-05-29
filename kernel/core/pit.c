#include <stdint.h>

#include <pit.h>

#include <serial.h>
#include <mcsos/arch/io.h>

volatile uint64_t g_ticks = 0;

void pit_configure_hz(uint32_t hz)
{
    uint16_t divisor;

    divisor = (uint16_t)(1193182 / hz);

    outb(0x43, 0x36);

    outb(0x40, divisor & 0xFF);
    outb(0x40, divisor >> 8);
}

void timer_on_irq0(void)
{
    ++g_ticks;

    if ((g_ticks % 100u) == 0u)
    {
        serial_write_string("ticks=");
        serial_write_dec64(g_ticks);
        serial_write_string("\n");
    }
}
