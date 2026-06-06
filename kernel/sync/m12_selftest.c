#include "mcs_sync.h"
#include "serial.h"

static mcs_spinlock_t boot_stats_lock;
static mcs_lockdep_state_t boot_lockdep;
static uint64_t boot_counter;

void m12_sync_selftest(void) {
    mcs_lockdep_init(&boot_lockdep);
    mcs_spin_init(&boot_stats_lock, 10u, "boot_stats");

    if (mcs_lockdep_before_acquire(&boot_lockdep, 10u, "boot_stats") != MCS_SYNC_OK) {
        serial_write_string("[M12] FAIL: lockdep acquire\n");
        return;
    }

    mcs_spin_lock(&boot_stats_lock);
    boot_counter++;
    mcs_spin_unlock(&boot_stats_lock);

    if (mcs_lockdep_after_release(&boot_lockdep, 10u, "boot_stats") != MCS_SYNC_OK) {
        serial_write_string("[M12] FAIL: lockdep release\n");
        return;
    }

    serial_write_string("[M12] sync selftest passed\n");
}
