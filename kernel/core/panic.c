#include <stdint.h>

#include <mcsos/kernel/panic.h>
#include <mcsos/kernel/log.h>

static inline void cpu_cli(void)
{
    __asm__ volatile ("cli");
}

static inline void cpu_hlt(void)
{
    __asm__ volatile ("hlt");
}

_Noreturn void kernel_panic_at(
    const char *message,
    uint64_t code,
    const char *file,
    uint64_t line
)
{
    cpu_cli();

    log_writeln("");
    log_writeln("========== KERNEL PANIC ==========");

    log_write("message: ");
    log_writeln(message);

    log_key_value_hex64(
        "panic_code",
        code
    );

    log_write("file: ");
    log_writeln(file);

    log_key_value_hex64(
        "line",
        line
    );

    log_writeln("system halted");

    for (;;) {
        cpu_hlt();
    }
}
