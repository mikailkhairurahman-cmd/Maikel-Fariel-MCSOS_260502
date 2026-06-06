.set KERNEL_STACK, 0x90000

.section .multiboot_bss, "aw", @nobits
.align 4096
pml4:   .skip 4096
pdpt:   .skip 4096
pd:     .skip 4096

.section .rodata
msg64:
    .asciz "[START] long mode ok\n"

.section .data
.align 16
gdt64:
    .quad 0x0000000000000000   /* null */
    .quad 0x00af9a000000ffff   /* code 64-bit */
    .quad 0x00af92000000ffff   /* data 64-bit */
gdt64_end:

gdt64_ptr:
    .word gdt64_end - gdt64 - 1
    .quad gdt64

.section .text
.global _start
.extern kmain
.extern serial_init
.extern serial_write_string
.code32

_start:
    cli
    mov $KERNEL_STACK, %esp

    /* check CPUID long mode support */
    mov $0x80000001, %eax
    cpuid
    bt  $29, %edx

    /* setup paging identity map for first 1GB */
    mov $pml4, %edi
    mov $pdpt, %eax
    or  $0x03, %eax
    mov %eax, (%edi)

    mov $pdpt, %edi
    mov $pd, %eax
    or  $0x03, %eax
    mov %eax, (%edi)

    mov $pd, %edi
    mov $0x00000083, %eax   /* 2MB pages, present, writable, huge */
    mov $512, %ecx
1:
    mov %eax, (%edi)
    add $0x200000, %eax
    add $8, %edi
    loop 1b

    /* load PML4 into CR3 */
    mov $pml4, %eax
    mov %eax, %cr3

    /* enable PAE */
    mov %cr4, %eax
    or  $0x20, %eax
    mov %eax, %cr4

    /* set LME bit in EFER */
    mov $0xC0000080, %ecx
    rdmsr
    or  $0x100, %eax
    wrmsr

/* enable paging + protected mode */
    mov %cr0, %eax
    or  $0x80000001, %eax
    mov %eax, %cr0

    /* load GDT dulu sebelum far jump */
    lgdt gdt64_ptr

    /* far jump to 64-bit code segment */
    ljmp $0x08, $start64

.code64
start64:
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %ss
    xor %ax, %ax
    mov %ax, %fs
    mov %ax, %gs

    mov $KERNEL_STACK, %rsp

    lgdt gdt64_ptr(%rip)

    call serial_init
    call kmain

2:
    hlt
    jmp 2b
