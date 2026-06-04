#include "serial.h"
#include "pic.h"
#include "pit.h"
#include "io.h"

#include "pmm.h"
#include "vmm.h"
#include "mcsos_thread.h"

#include <stdint.h>

static struct pmm_state *g_pmm = 0;

static mcsos_scheduler_t g_sched;
static mcsos_thread_t g_boot_thread;
static mcsos_thread_t g_thread_a;
static mcsos_thread_t g_thread_b;

static unsigned char g_stack_a[8192]
    __attribute__((aligned(16)));

static unsigned char g_stack_b[8192]
    __attribute__((aligned(16)));

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

static void demo_thread_a(void *arg)
{
    (void)arg;

    for (;;)
    {
        serial_write_string("[M9] thread A\n");
        mcsos_sched_yield(&g_sched);
    }
}

static void demo_thread_b(void *arg)
{
    (void)arg;

    for (;;)
    {
        serial_write_string("[M9] thread B\n");
        mcsos_sched_yield(&g_sched);
    }
}

void kmain(void)
{
    serial_init();

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

serial_write_string("[M9] scheduler init\n");

mcsos_scheduler_init(
    &g_sched,
    &g_boot_thread
);

mcsos_thread_prepare(
    &g_thread_a,
    "thread-a",
    demo_thread_a,
    0,
    g_stack_a,
    sizeof(g_stack_a),
    g_sched.next_id++
);

mcsos_thread_prepare(
    &g_thread_b,
    "thread-b",
    demo_thread_b,
    0,
    g_stack_b,
    sizeof(g_stack_b),
    g_sched.next_id++
);

mcsos_sched_enqueue(
    &g_sched,
    &g_thread_a
);

mcsos_sched_enqueue(
    &g_sched,
    &g_thread_b
);

serial_write_string("[M9] scheduler ready\n");

serial_write_string("[M9] before first yield\n");

mcsos_sched_yield(&g_sched);

serial_write_string("[M9] after first yield\n");

for (;;)
{
    __asm__ volatile ("hlt");
}

}
