#include "io.h"
#include "pic.h"
#include "pit.h"
#include "serial.h"

void kmain(void)
{
    serial_init();
    serial_write_string("M5 SERIAL HIDUP\n");

    cpu_cli();

    serial_write_string("[MCSOS:M5] boot start\n");

    pic_remap(0x20, 0x28);

    serial_write_string("[MCSOS:M5] pic remapped\n");

    pit_configure_hz(100);

    serial_write_string("[MCSOS:M5] pit configured\n");

    cpu_sti();

    serial_write_string("[MCSOS:M5] interrupts enabled\n");

    for (;;)
    {
        cpu_hlt();
    }
}
