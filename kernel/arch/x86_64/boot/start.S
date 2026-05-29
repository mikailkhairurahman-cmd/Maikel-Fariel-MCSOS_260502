.section .text
.global _start
.extern kmain

_start:
    cli

    mov $0x90000, %rsp
    mov $0x90000, %rbp

    call kmain

1:
    hlt
    jmp 1b
