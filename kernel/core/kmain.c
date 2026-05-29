#include "serial.h"
#include "pic.h"
#include "pit.h"
#include "io.h"

#include "pmm.h"
#include "vmm.h"

#include <stdint.h>

static struct pmm_state *g_pmm = 0;

static uint64_t kernel_vmm_alloc(void *ctx)
{
    struct pmm_state *pmm = ctx;
    return pmm_alloc_frame(pmm);
}

static void kernel_vmm_free(void *ctx, uint64_t frame_paddr)
{
    struct pmm_state *pmm = ctx;
    pmm_free_frame(pmm, frame_paddr);
}

static void *kernel_phys_to_virt(void *ctx, uint64_t paddr)
{
    uint64_t hhdm_offset = *(uint64_t *)ctx;
    return (void *)(hhdm_offset + paddr);
}

void kmain(void)
{
    serial_init();

    serial_write_string("[M7] kernel start\n");

    cpu_cli();

    pic_remap(0x20, 0x28);

    pit_configure_hz(100);

    static struct vmm_space kernel_space;

    uint64_t hhdm = 0xFFFF800000000000ULL;
(void)hhdm;

    uint64_t pml4_phys = 0x1000;

    vmm_space_init(
        &kernel_space,
        pml4_phys,
        g_pmm,
        kernel_vmm_alloc,
        kernel_vmm_free,
        kernel_phys_to_virt
    );

    serial_write_string("[M7] vmm init ok\n");

    vmm_map_page(
        &kernel_space,
        0xFFFF800000200000ULL,
        0x200000ULL,
        VMM_PAGE_PRESENT |
        VMM_PAGE_WRITABLE
    );

    serial_write_string("[M7] map page ok\n");

    vmm_load_cr3(pml4_phys);

    serial_write_string("[M7] cr3 loaded\n");

    cpu_sti();

    serial_write_string("[M7] interrupts enabled\n");

    for (;;)
    {
        __asm__ volatile ("hlt");
    }
}
