#!/usr/bin/env bash

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
BOLD='\033[1m'
DIM='\033[2m'
RESET='\033[0m'

PASS=0; FAIL=0

bar() {
  echo -e "${DIM}${CYAN}  ──────────────────────────────────────────${RESET}"
  sleep 0.1
}

pass() {
  sleep 0.15
  echo -e "  ${GREEN}✓${RESET}  $1"
  PASS=$((PASS+1))
}

fail() {
  sleep 0.15
  echo -e "  ${RED}✗${RESET}  $1"
  FAIL=$((FAIL+1))
}

header() {
  sleep 0.2
  echo ""
  echo -e "  ${YELLOW}${BOLD}$1${RESET}"
  sleep 0.05
}

clear
sleep 0.3
echo ""
echo -e "${BOLD}${CYAN}"
sleep 0.1
echo "  ╔═══════════════════════════════════════════╗"
sleep 0.05
echo "  ║                                           ║"
sleep 0.05
echo "  ║    MCSOS  ·  Milestone Status  M0–M16     ║"
sleep 0.05
echo "  ║                                           ║"
sleep 0.05
echo "  ╚═══════════════════════════════════════════╝"
echo -e "${RESET}"
sleep 0.4

header "M0  —  Baseline Setup"
git log --oneline --all | grep -q "M0\|baseline" \
  && pass "M0: commit baseline ditemukan" \
  || fail "M0: commit tidak ada"

header "M1  —  Toolchain Reproducible"
git log --oneline --all | grep -q "M1\|toolchain" \
  && pass "M1: commit toolchain ditemukan" \
  || fail "M1: commit tidak ada"

header "M2  —  Boot Image / Kernel ELF64"
git log --oneline --all | grep -q "M2\|bootable\|ELF" \
  && pass "M2: commit boot image ditemukan" \
  || fail "M2: commit tidak ada"

header "M3  —  Panic Path / GDB"
[ -f evidence/M3/kernel.elf ] \
  && pass "M3: kernel.elf tersedia di evidence/" \
  || git log --oneline --all | grep -q "M3\|panic" \
  && pass "M3: commit panic path ditemukan" \
  || fail "M3: tidak ada"

header "M4  —  IDT / Exception / Interrupt"
[ -f evidence/M4/kernel.elf ] \
  && pass "M4: kernel.elf tersedia di evidence/" \
  || git log --oneline --all | grep -q "M4\|IDT" \
  && pass "M4: commit IDT ditemukan" \
  || fail "M4: tidak ada"

header "M5  —  Timer / IO Abstraction"
git log --oneline --all | grep -q "M5\|m5\|interrupt\|timer\|io" \
  && pass "M5: commit timer/IO ditemukan" \
  || fail "M5: commit tidak ada"

header "M6  —  Physical Memory Manager"
git log --oneline --all | grep -q "M6\|m6\|pmm\|physical" \
  && pass "M6: commit PMM ditemukan" \
  || fail "M6: commit tidak ada"

header "M7  —  Virtual Memory Manager"
git log --oneline --all | grep -q "M7\|m7\|vmm\|virtual" \
  && pass "M7: commit VMM ditemukan" \
  || fail "M7: commit tidak ada"

header "M8  —  Kernel Heap"
git log --oneline --all | grep -q "M8\|m8\|heap" \
  && pass "M8: commit heap ditemukan" \
  || fail "M8: commit tidak ada"

header "M9  —  Kernel Thread / Scheduler"
git log --oneline --all | grep -q "M9\|m9\|scheduler" \
  && pass "M9: commit scheduler ditemukan" \
  || fail "M9: commit tidak ada"

header "M10  —  Syscall ABI / Dispatcher"
[ -f build/syscall.o ] \
  && pass "M10: syscall.o tersedia" \
  || git log --oneline --all | grep -q "M10\|syscall" \
  && pass "M10: commit syscall ditemukan" \
  || fail "M10: tidak ada"
[ -f build/test_syscall_host ] \
  && pass "M10: host test binary tersedia" \
  || fail "M10: host test tidak ada"

header "M11  —  ELF64 User Loader"
[ -f build/m11_elf_loader.o ] \
  && pass "M11: m11_elf_loader.o tersedia" \
  || git log --oneline --all | grep -q "M11\|elf.*loader" \
  && pass "M11: commit ELF loader ditemukan" \
  || fail "M11: tidak ada"
[ -f build/m11_host_test ] \
  && pass "M11: host test binary tersedia" \
  || fail "M11: host test tidak ada"

header "M12  —  Synchronization"
[ -d evidence/M12 ] \
  && pass "M12: evidence/ tersedia" \
  || git log --oneline --all | grep -q "M12\|spinlock\|mutex" \
  && pass "M12: commit sync ditemukan" \
  || fail "M12: tidak ada"

header "M13  —  VFS / RAMFS / FD Table"
git log --oneline --all | grep -q "M13\|m13\|vfs\|ramfs" \
  && pass "M13: commit VFS ditemukan" \
  || fail "M13: commit tidak ada"

header "M14  —  Block Layer / RAM Block Driver"
[ -f artifacts/m14/mcsos_m14.iso ] \
  && pass "M14: mcsos_m14.iso tersedia" \
  || fail "M14: ISO tidak ada"
[ -f artifacts/m14/host_test.txt ] \
  && pass "M14: host test log tersedia" \
  || fail "M14: host test log tidak ada"

header "M15  —  MCSFS1 Persistent Filesystem"
[ -f fs/mcsfs1/mcsfs1.c ] \
  && pass "M15: mcsfs1.c tersedia" \
  || fail "M15: mcsfs1.c tidak ada"
[ -f artifacts/m15/host_test.txt ] \
  && pass "M15: host test log tersedia" \
  || fail "M15: host test log tidak ada"

header "M16  —  Journal / Recovery"
[ -d artifacts/m16 ] \
  && pass "M16: artifacts/m16/ tersedia" \
  || git log --oneline --all | grep -q "M16\|m16\|journal\|recovery" \
  && pass "M16: commit journal ditemukan" \
  || fail "M16: tidak ada"

sleep 0.3
echo ""
echo -e "${DIM}${CYAN}  ──────────────────────────────────────────${RESET}"
sleep 0.2
echo ""

if [ "$FAIL" -eq 0 ]; then
  echo -e "${BOLD}${GREEN}  ✦  Semua milestone PASS  ✦${RESET}"
else
  echo -e "${BOLD}${RED}  ✦  Ada $FAIL milestone yang FAIL${RESET}"
fi

sleep 0.3
echo ""
echo -e "${BOLD}${CYAN}"
sleep 0.05
echo "  ╔═══════════════════════════════════════════╗"
sleep 0.05
printf "  ║   PASS: %-3s   FAIL: %-3s                   ║\n" "$PASS" "$FAIL"
sleep 0.05
echo "  ╠═══════════════════════════════════════════╣"
sleep 0.05
printf "  ║   Branch : %-30s  ║\n" "$(git branch --show-current | cut -c1-30)"
sleep 0.05
printf "  ║   Commit : %-30s  ║\n" "$(git log --oneline -1 | cut -c1-30)"
sleep 0.05
echo "  ╚═══════════════════════════════════════════╝"
echo -e "${RESET}"
