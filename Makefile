SHELL := /usr/bin/env bash
.DEFAULT_GOAL := all

BUILD_DIR := build

CC := clang
LD := ld.lld
OBJDUMP := objdump
READELF := readelf
NM := nm

CFLAGS := \
	--target=x86_64-unknown-none-elf \
	-std=c17 \
	-ffreestanding \
	-fno-builtin \
	-fno-stack-protector \
	-fno-stack-check \
	-fno-pic \
	-fno-pie \
	-m64 \
	-mno-red-zone \
	-Wall \
	-Wextra \
	-Werror \
	-Iinclude \
	-Ikernel/include \
	-Ikernel/arch/x86_64/include

LDFLAGS := \
	-nostdlib \
	-static \
	-z max-page-size=0x1000 \
	-T linker.ld

SRC_C := $(shell find kernel -name '*.c')
SRC_S := $(shell find kernel -name '*.S')
$(info ASM FILES = $(SRC_S))

OBJ_C := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRC_C))
OBJ_S := $(patsubst %.S,$(BUILD_DIR)/%.o,$(SRC_S))
OBJS := \
$(BUILD)/multiboot.o \
$(BUILD)/boot.o \
$(BUILD)/interrupts.o \
$(BUILD)/serial.o \
$(BUILD)/panic.o \
$(BUILD)/pic.o \
$(BUILD)/pit.o \
$(BUILD)/idt.o \
$(BUILD)/kernel.o

OBJ := $(OBJ_C) $(OBJ_S)

KERNEL := $(BUILD_DIR)/kernel.elf
MAPFILE := $(BUILD_DIR)/mcsos-m5.map
M5_ELF := $(BUILD_DIR)/mcsos-m5.elf

.PHONY: all clean iso run inspect grade

all: $(KERNEL)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.S
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL): $(OBJ)
	@mkdir -p $(BUILD_DIR)
	$(LD) $(LDFLAGS) \
	-Map=$(MAPFILE) \
	-o $(KERNEL) \
	$(OBJ)

inspect: $(KERNEL)
	$(READELF) -h $(KERNEL)
	$(NM) -n $(KERNEL)

grade: $(KERNEL)
	mkdir -p $(BUILD_DIR)
	cp $(KERNEL) $(M5_ELF)

	$(READELF) -hW $(KERNEL) > $(BUILD_DIR)/readelf-header.txt
	$(READELF) -SW $(KERNEL) > $(BUILD_DIR)/readelf-sections.txt
	$(READELF) -lW $(KERNEL) > $(BUILD_DIR)/readelf-program-headers.txt

	$(NM) -n $(KERNEL) > $(BUILD_DIR)/symbols.txt
	$(NM) -u $(KERNEL) > $(BUILD_DIR)/undefined.txt

	$(OBJDUMP) -drwC $(KERNEL) > $(BUILD_DIR)/disassembly.txt

	@echo "[M5] grade artifacts generated"

iso: $(KERNEL)
	mkdir -p build/isofiles/boot/grub
	cp $(KERNEL) build/isofiles/boot/kernel.elf
	cp grub.cfg build/isofiles/boot/grub/grub.cfg
	grub-mkrescue -o build/mcsos.iso build/isofiles

run: iso
	qemu-system-x86_64 \
	-machine q35 \
	-m 512M \
	-serial mon:stdio \
	-no-reboot \
	-no-shutdown \
	-cdrom build/mcsos.iso
clean:
	rm -rf $(BUILD_DIR)
