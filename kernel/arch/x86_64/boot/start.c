extern void kmain(void);

__attribute__((noreturn))
void _start(void) {

    kmain();

    for (;;) {
        __asm__ volatile (
            "cli; hlt"
            :
            :
            : "memory"
        );
    }
}
