#include <stdint.h>

#include <mcsos/arch/cpu.h>
#include <mcsos/arch/idt.h>

#include <mcsos/kernel/log.h>
#include <mcsos/kernel/panic.h>
#include <mcsos/kernel/version.h>

extern char __kernel_start[];
extern char __kernel_end[];

static void m4_selftest(void)
{
    KERNEL_ASSERT(__kernel_end > __kernel_start);

    KERNEL_ASSERT(sizeof(uintptr_t) == 8u);

    KERNEL_ASSERT(sizeof(x86_64_idt_entry_t) == 16u);

    KERNEL_ASSERT(x86_64_idt_base_for_test() != 0u);

    KERNEL_ASSERT(
        x86_64_idt_limit_for_test() == 4095u
    );

    log_writeln(
        "[M4] selftest: IDT invariants passed"
    );
}

void kmain(void)
{
    log_init();

    log_write(MCSOS_NAME);
    log_write(" ");
    log_write(MCSOS_VERSION);
    log_write(" ");
    log_write(MCSOS_MILESTONE);
    log_writeln(" kernel entered");

    log_key_value_hex64(
        "kernel_start",
        (uint64_t)(uintptr_t)__kernel_start
    );

    log_key_value_hex64(
        "kernel_end",
        (uint64_t)(uintptr_t)__kernel_end
    );

    log_key_value_hex64(
        "rflags_before_idt",
        cpu_read_rflags()
    );

    x86_64_idt_init();

    m4_selftest();

#ifdef MCSOS_M4_TRIGGER_BREAKPOINT

    log_writeln(
        "[M4] triggering intentional breakpoint exception"
    );

    x86_64_trigger_breakpoint_for_test();

    log_writeln(
        "[M4] returned from breakpoint handler"
    );

#endif

#ifdef MCSOS_M3_TRIGGER_PANIC

    KERNEL_PANIC(
        "intentional M3/M4 panic test",
        0x4D43534F533034u
    );

#else

    log_writeln(
        "[M4] IDT and exception dispatch path installed"
    );

    log_writeln(
        "[M4] ready for QEMU smoke test and GDB audit"
    );

    cpu_halt_forever();

#endif
}
#include "idt.h"
#include "io.h"
#include "panic.h"
#include "pic.h"
#include "pit.h"
#include "serial.h"

void kmain(void) {
    cpu_cli();
    serial_init();
    serial_write_string("[MCSOS:M5] boot: external interrupt bring-up start\n");

    idt_init();
    serial_write_string("[MCSOS:M5] idt: loaded\n");

    pic_remap(PIC_MASTER_OFFSET, PIC_SLAVE_OFFSET);
    pic_mask_all();
    pic_unmask_irq(0);
    serial_write_string("[MCSOS:M5] pic: remapped; mask master=");
    serial_write_hex64(pic_read_master_mask());
    serial_write_string(" slave=");
    serial_write_hex64(pic_read_slave_mask());
    serial_write_string("\n");

    pit_configure_hz(100u);
    serial_write_string("[MCSOS:M5] pit: configured 100Hz\n");
    serial_write_string("[MCSOS:M5] sti: enabling interrupts\n");
    cpu_sti();

#if defined(MCSOS_TEST_BREAKPOINT)
    __asm__ volatile ("int3");
#endif

    for (;;) {
        cpu_hlt();
    }
}
