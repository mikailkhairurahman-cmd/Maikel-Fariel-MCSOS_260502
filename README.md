# MCSOS 260502 - Cacing Naga

Proyek kernel sistem operasi pendidikan berbasis x86_64, dikembangkan secara bertahap dari M0 hingga M16 menggunakan WSL 2 di Windows 11. Setiap modul memiliki laporan praktikum tersendiri yang mencakup analisis, implementasi, dan hasil pengujian.

Status akhir: M16 — semua modul telah diselesaikan, mulai dari persiapan lingkungan hingga journal recovery.

## Spesifikasi & Modul

* Arsitektur target: x86_64
* Emulator: QEMU x86_64
* Firmware: OVMF / UEFI
* Bahasa pemrograman: C17 freestanding dan assembly x86_64
* Model kernel: monolitik dengan struktur modular internal

| Modul | Topik | Status |
|-------|-------|--------|
| M0 | Persiapan lingkungan | ✅ |
| M1 | Toolchain | ✅ |
| M2 | Boot image | ✅ |
| M3 | Panic & debug | ✅ |
| M4 | IDT & exception handling | ✅ |
| M5 | Timer & IRQ | ✅ |
| M6 | Physical memory manager | ✅ |
| M7 | Virtual memory manager | ✅ |
| M8 | Kernel heap | ✅ |
| M9 | Thread & scheduler | ✅ |
| M10 | Syscall ABI | ✅ |
| M11 | ELF loader | ✅ |
| M12 | Sinkronisasi | ✅ |
| M13 | VFS & RamFS | ✅ |
| M14 | Block device | ✅ |
| M15 | MCSFS1 filesystem | ✅ |
| M16 | Journal recovery | ✅ |

## Cara Menjalankan

Jalankan perintah `make build && make run && make verify` untuk membangun, menjalankan, dan memverifikasi kernel secara sekaligus. Log verifikasi disimpan di `evidence/verification/make_verify.log`. Validasi boot QEMU/OVMF dijalankan pada lingkungan Windows 11 + WSL 2.

## Dokumen Utama

* `docs/laporan/` — Laporan praktikum M0 hingga M16
* `docs/architecture/overview.md`
* `docs/testing/verification_matrix.md`
* `docs/security/threat_model.md`
