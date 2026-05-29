#include <stdint.h>

#include <mcsos/arch/idt.h>

#include <mcsos/kernel/log.h>
#include <mcsos/kernel/panic.h>

static const char *exception_names[32] = {
    "#DE Divide Error",
    "#DB Debug",
    "NMI Interrupt",
    "#BP Breakpoint",
    "#OF Overflow",
    "#BR Bound Range Exceeded",
    "#UD Invalid Opcode",
    "#NM Device Not Available",
    "#DF Double Fault",
    "Coprocessor Segment Overrun",
    "#TS Invalid TSS",
    "#NP Segment Not Present",
    "#SS Stack Segment Fault",
    "#GP General Protection Fault",
    "#PF Page Fault",
    "Reserved",
    "#MF x87 Floating-Point Exception",
    "#AC Alignment Check",
    "#MC Machine Check",
    "#XM SIMD Floating-Point Exception",
    "#VE Virtualization Exception",
    "#CP Control Protection Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "#HV Hypervisor Injection Exception",
    "#VC VMM Communication Exception",
    "#SX Security Exception",
    "Reserved"
};

static uint64_t trap_count;

static const char *trap_name(uint64_t vector)
{
    if (vector < 32u) {
        return exception_names[vector];
    }

    return "external-or-user-defined-interrupt";
}

uint64_t m4_trap_count_for_test(void)
{
    return trap_count;
}

static void log_trap_frame(
    const x86_64_trap_frame_t *frame
)
{
    log_key_value_hex64(
        "trap_vector",
        frame->vector
    );

    log_key_value_hex64(
        "trap_error",
        frame->error_code
    );

    log_key_value_hex64(
        "trap_rip",
        frame->rip
    );

    log_key_value_hex64(
        "trap_cs",
        frame->cs
    );

    log_key_value_hex64(
        "trap_rflags",
        frame->rflags
    );
}

void x86_64_trap_dispatch(
    x86_64_trap_frame_t *frame
)
{
    if (frame == (x86_64_trap_frame_t *)0) {
        kernel_panic_at(
            "null trap frame",
            0,
            __FILE__,
            __LINE__
        );
    }

    ++trap_count;

    log_write("[M4] trap dispatch: ");

    log_writeln(
        trap_name(frame->vector)
    );

    log_trap_frame(frame);

    if (frame->vector == 3u) {

        log_writeln(
            "[M4] breakpoint handled; returning with iretq"
        );

        return;
    }

    kernel_panic_at(
        "unrecoverable CPU exception",
        frame->vector,
        __FILE__,
        __LINE__
    );
}
