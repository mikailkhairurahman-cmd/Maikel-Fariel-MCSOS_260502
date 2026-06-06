#include "mcsos/user/m11_elf_loader.h"
#include "mcsos/syscall.h"
#include "serial.h"
#include "m12_selftest.h"
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
/* M10 syscall smoke test */
    m12_sync_selftest();
    serial_write_string("[M10] syscall init\n");
    mcsos_syscall_ops_t ops = {
        .get_ticks     = 0,
        .yield_current = 0,
        .exit_current  = 0,
        .write_serial  = 0
    };
    mcsos_syscall_init(&ops);
    int64_t ping = mcsos_syscall_dispatch(0, 0, 0, 0, 0, 0, 0);
    if (ping == 0x2605020AL) {
        serial_write_string("[M10] syscall ping ok\n");
    } else {
        serial_write_string("[M10] syscall ping FAIL\n");
    }
    serial_write_string("[M10] syscall smoke done\n");

/* C6: int 0x80 entry smoke test via frame */
    serial_write_string("[M10] C6 frame dispatch test\n");
    mcsos_syscall_frame_t frame = {
        .nr   = MCSOS_SYS_PING,
        .arg0 = 0, .arg1 = 0, .arg2 = 0,
        .arg3 = 0, .arg4 = 0, .arg5 = 0,
        .ret  = 0
    };
    mcsos_syscall_dispatch_frame(&frame);
    if (frame.ret == 0x2605020AL) {
        serial_write_string("[M10] int80 frame ping ok\n");
    } else {
        serial_write_string("[M10] int80 frame ping FAIL\n");
    }
    serial_write_string("[M10] C6 entry smoke done\n");

/* M11 ELF loader smoke test */
    serial_write_string("[M11] elf loader init\n");
    static unsigned char m11_test_image[12288];
    struct m11_elf64_ehdr *m11_eh = (struct m11_elf64_ehdr *)(void *)m11_test_image;
    /* build minimal valid ELF64 in BSS */
    m11_eh->e_ident[0] = 0x7fu;
    m11_eh->e_ident[1] = 'E';
    m11_eh->e_ident[2] = 'L';
    m11_eh->e_ident[3] = 'F';
    m11_eh->e_ident[4] = 2u;   /* ELFCLASS64 */
    m11_eh->e_ident[5] = 1u;   /* ELFDATA2LSB */
    m11_eh->e_ident[6] = 1u;   /* EV_CURRENT */
    m11_eh->e_type     = 2u;   /* ET_EXEC */
    m11_eh->e_machine  = 62u;  /* EM_X86_64 */
    m11_eh->e_version  = 1u;
    m11_eh->e_entry    = 0x401000ull;
    m11_eh->e_phoff    = sizeof(struct m11_elf64_ehdr);
    m11_eh->e_ehsize   = (uint16_t)sizeof(struct m11_elf64_ehdr);
    m11_eh->e_phentsize = (uint16_t)sizeof(struct m11_elf64_phdr);
    m11_eh->e_phnum    = 1u;
    struct m11_elf64_phdr *m11_ph = (struct m11_elf64_phdr *)(void *)
        (m11_test_image + sizeof(struct m11_elf64_ehdr));
    m11_ph->p_type   = 1u;         /* PT_LOAD */
    m11_ph->p_flags  = 4u | 1u;   /* PF_R | PF_X */
    m11_ph->p_offset = 0x1000u;
    m11_ph->p_vaddr  = 0x400000ull;
    m11_ph->p_filesz = 16u;
    m11_ph->p_memsz  = 4096u;
    m11_ph->p_align  = 4096u;
    struct m11_user_region m11_region = { 0x400000ull, 0x8000000000ull };
    struct m11_process_image_plan m11_plan;
    int m11_rc = m11_elf64_plan_load(m11_test_image, 12288u, m11_region, &m11_plan);
    if (m11_rc == 0) {
        serial_write_string("[M11] elf: plan ok\n");
    } else {
        serial_write_string("[M11] elf: plan FAIL\n");
    }
    serial_write_string("[M11] user image plan ready\n");

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

/* vmm_load_cr3(pml4_phys); */ /* dinonaktifkan sementara M10 */
    serial_write_string("[M7] cr3 skipped for M10\n");

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
