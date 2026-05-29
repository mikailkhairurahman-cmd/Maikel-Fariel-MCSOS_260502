#include <io.h>
#include <serial.h>

#define COM1 0x3F8

void serial_init(void) {
    outb(COM1 + 1, 0x00);
    outb(COM1 + 3, 0x80);
    outb(COM1 + 0, 0x03);
    outb(COM1 + 1, 0x00);
    outb(COM1 + 3, 0x03);
    outb(COM1 + 2, 0xC7);
    outb(COM1 + 4, 0x0B);
}

static int serial_transmit_empty(void) {
    return inb(COM1 + 5) & 0x20;
}

void serial_write_char(char c) {
    while (!serial_transmit_empty()) {
        __asm__ volatile ("pause");
    }

    outb(COM1, (uint8_t)c);
}

void serial_write_string(const char *s) {
    while (*s) {
        if (*s == '\n') {
            serial_write_char('\r');
        }

        serial_write_char(*s++);
    }
}

void serial_write_hex64(uint64_t value) {
    static const char hex[] = "0123456789abcdef";

    serial_write_string("0x");

    for (int i = 60; i >= 0; i -= 4) {
        serial_write_char(hex[(value >> i) & 0xF]);
    }
}

void serial_write_dec64(uint64_t value) {
    char buf[32];
    int i = 0;

    if (value == 0) {
        serial_write_char('0');
        return;
    }

    while (value > 0) {
        buf[i++] = '0' + (value % 10);
        value /= 10;
    }

    while (i--) {
        serial_write_char(buf[i]);
    }
}
