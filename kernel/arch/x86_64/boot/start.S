.section .rodata
msg:
    .asciz "[START entered]\n"

.section .text
.global _start
.extern kmain
.extern serial_init
.extern serial_write_string

_start:
    cli

    mov $0x90000, %rsp
    mov $0x90000, %rbp

    call serial_init

    lea msg(%rip), %rdi
    call serial_write_string

    call kmain

1:
    hlt
    jmp 1b
