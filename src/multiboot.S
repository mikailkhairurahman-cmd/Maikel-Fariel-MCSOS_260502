.section .multiboot
.align 8

.set MAGIC, 0xe85250d6
.set ARCH, 0
.set HEADER_LENGTH, multiboot_header_end - multiboot_header
.set CHECKSUM, -(MAGIC + ARCH + HEADER_LENGTH)

multiboot_header:
    .long MAGIC
    .long ARCH
    .long HEADER_LENGTH
    .long CHECKSUM

    # end tag
    .short 0
    .short 0
    .long 8

multiboot_header_end:
