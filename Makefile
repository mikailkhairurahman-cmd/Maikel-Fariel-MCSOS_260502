CC ?= cc
CLANG ?= clang
CFLAGS_HOST := -std=c17 -Wall -Wextra -Werror -Iinclude -O2
CFLAGS_FREESTANDING := --target=x86_64-elf -std=c17 -ffreestanding -fno-builtin -fno-stack-protector -fno-pic -mno-red-zone -Wall -Wextra -Werror -Iinclude -O2 -c
SRC := kernel/block/block.c kernel/block/ramblk.c kernel/block/bcache.c
OBJ := build/block.o build/ramblk.o build/bcache.o

.PHONY: all host-test freestanding audit clean
all: host-test freestanding audit

host-test: build/test_m14_block
	./build/test_m14_block

build/test_m14_block: tests/host/test_m14_block.c $(SRC) include/mcsos/block.h
	mkdir -p build
	$(CC) $(CFLAGS_HOST) tests/host/test_m14_block.c $(SRC) -o $@

freestanding: $(OBJ)

build/%.o: kernel/block/%.c include/mcsos/block.h
	mkdir -p build
	$(CLANG) $(CFLAGS_FREESTANDING) $< -o $@

audit: freestanding
	ld -r -o build/m14_block_layer.o $(OBJ)
	nm -u build/m14_block_layer.o > artifacts/m14_nm_undefined.txt
	readelf -h build/m14_block_layer.o > artifacts/m14_readelf_block.txt
	objdump -dr build/m14_block_layer.o > artifacts/m14_objdump_block.txt
	sha256sum $(OBJ) build/m14_block_layer.o build/test_m14_block > artifacts/m14_sha256.txt
	test ! -s artifacts/m14_nm_undefined.txt

clean:
	rm -rf build artifacts/*

# --- M15 MCSFS1 targets ---
CC ?= clang
HOST_CFLAGS_M15 := -std=c17 -Wall -Wextra -Werror -O2 -g
FREESTANDING_CFLAGS_M15 := -target x86_64-elf -std=c17 -ffreestanding -fno-builtin -fno-stack-protector -fno-pic -mno-red-zone -Wall -Wextra -Werror -O2 -g

.PHONY: m15-all m15-clean
m15-all: artifacts/m15/test_mcsfs1 artifacts/m15/mcsfs1.o artifacts/m15/mcsfs1.rel.o
	./artifacts/m15/test_mcsfs1 | tee artifacts/m15/host_test.txt
	nm -u artifacts/m15/mcsfs1.rel.o | tee artifacts/m15/nm_undefined.txt
	test ! -s artifacts/m15/nm_undefined.txt
	readelf -h artifacts/m15/mcsfs1.rel.o | tee artifacts/m15/readelf_header.txt
	objdump -dr artifacts/m15/mcsfs1.rel.o | tee artifacts/m15/objdump.txt >/dev/null
	sha256sum artifacts/m15/* | tee artifacts/m15/SHA256SUMS.txt

artifacts/m15/test_mcsfs1: tests/m15/test_mcsfs1.c fs/mcsfs1/mcsfs1.c fs/mcsfs1/mcsfs1.h
	mkdir -p artifacts/m15
	$(CC) $(HOST_CFLAGS_M15) -I. tests/m15/test_mcsfs1.c fs/mcsfs1/mcsfs1.c -o $@

artifacts/m15/mcsfs1.o: fs/mcsfs1/mcsfs1.c fs/mcsfs1/mcsfs1.h
	mkdir -p artifacts/m15
	$(CC) $(FREESTANDING_CFLAGS_M15) -I. -c fs/mcsfs1/mcsfs1.c -o $@

artifacts/m15/mcsfs1.rel.o: artifacts/m15/mcsfs1.o
	ld -r $< -o $@

m15-clean:
	rm -rf artifacts/m15/test_mcsfs1 artifacts/m15/mcsfs1.o \
	       artifacts/m15/mcsfs1.rel.o artifacts/m15/host_test.txt \
	       artifacts/m15/nm_undefined.txt artifacts/m15/readelf_header.txt \
	       artifacts/m15/objdump.txt artifacts/m15/SHA256SUMS.txt

# --- M15 QEMU smoke test ---
iso: all
	mkdir -p build/isofiles/boot/grub
	cp build/kernel.elf build/isofiles/boot/kernel.elf
	cp grub.cfg build/isofiles/boot/grub/grub.cfg
	grub-mkrescue -o build/mcsos.iso build/isofiles 2>/dev/null

run-qemu-m15: iso
	timeout 30 qemu-system-x86_64 \
		-machine q35 \
		-m 512M \
		-display none \
		-monitor /dev/null \
		-serial file:artifacts/m15/qemu_serial.log \
		-no-reboot \
		-no-shutdown \
		-cdrom build/mcsos.iso || true
	cat artifacts/m15/qemu_serial.log
