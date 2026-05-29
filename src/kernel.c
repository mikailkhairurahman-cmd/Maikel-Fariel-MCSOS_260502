#include "pmm.h"
#include "serial.h"
#include "panic.h"

static struct pmm_state kernel_pmm;

static uint8_t kernel_pmm_bitmap[PMM_BITMAP_BYTES]
    __attribute__((aligned(4096)));

static struct boot_mem_region early_regions[] = {
    {
        .base = 0x00000000ULL,
        .length = 0x0009f000ULL,
        .type = BOOT_MEM_USABLE
    },
    {
        .base = 0x0009f000ULL,
        .length = 0x00001000ULL,
        .type = BOOT_MEM_RESERVED
    },
    {
        .base = 0x00100000ULL,
        .length = 0x03f00000ULL,
        .type = BOOT_MEM_USABLE
    }
};

static void kernel_memory_init(
    const struct boot_mem_region *regions,
    size_t region_count
) {
    bool ok = pmm_init_from_map(
        &kernel_pmm,
        regions,
        region_count,
        kernel_pmm_bitmap,
        sizeof(kernel_pmm_bitmap),
        PMM_MAX_PHYS_BYTES
    );

    if (!ok) {
        panic("pmm_init_from_map failed");
    }

    serial_write("[m6] pmm initialized\n");

    serial_write_u64_hex(
        pmm_frame_count(&kernel_pmm)
    );

    serial_write(" frames managed\n");

    serial_write_u64_hex(
        pmm_free_count(&kernel_pmm)
    );

    serial_write(" frames free\n");

    uint64_t f =
        pmm_alloc_frame(&kernel_pmm);

    if (f == PMM_INVALID_FRAME) {
        panic("pmm_alloc_frame returned invalid");
    }

    serial_write("[m6] sample frame = ");

    serial_write_u64_hex(f);

    serial_write("\n");

    if (!pmm_free_frame(&kernel_pmm, f)) {
        panic("pmm_free_frame failed");
    }
}
