ENTRY(_start)

SECTIONS
{
    . = 2M;

    .multiboot : ALIGN(8) {
        *(.multiboot)
    }

    .text : ALIGN(4K) {
        *(.text .text.*)
    }

    .rodata : ALIGN(4K) {
        *(.rodata .rodata.*)
    }

    .data : ALIGN(4K) {
        *(.data .data.*)
    }

    .bss : ALIGN(4K) {
        *(COMMON)
        *(.bss .bss.*)

        . = ALIGN(16);
        __stack_bottom = .;
        . += 64K;
        __stack_top = .;
    }
}
