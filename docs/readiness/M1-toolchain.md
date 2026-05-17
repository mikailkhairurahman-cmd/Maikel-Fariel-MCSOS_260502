# Readiness Review M1 - Toolchain Reproducible

## Identitas

- Nama kelompok: Cacing Naga
- Nama anggota: Moch Fariel Aurizki
- Nama anggota: Mikail Khairu Rahman
- NIM anggota: 25832072007
- NIM anggota: 25832073005
- Kelas: PTI 1A
- Dosen: Muhaemin Sidiq, S.Pd., M.Pd.
- Program Studi: Pendidikan Teknologi Informasi, Institut Pendidikan Indonesia
- Tanggal: 17 Mei 2026
- Commit hash: b9dee39

---

## Ringkasan hasil

Pada milestone M1 berhasil dibuat baseline toolchain reproducible untuk pengembangan MCSOS berbasis x86_64 freestanding environment pada WSL 2 Linux filesystem.

Seluruh script validasi toolchain, metadata collection, proof compilation, QEMU capability probing, dan reproducibility checking berhasil dijalankan menggunakan Makefile terpusat.

Artefak proof ELF64 x86_64 berhasil dihasilkan tanpa dependency terhadap hosted libc, startup runtime host, maupun dynamic linker.

Berdasarkan hasil pengujian `make meta`, `make check`, `make proof`, `make qemu-probe`, `make repro`, dan `make test`, lingkungan praktikum dinyatakan siap untuk melanjutkan ke M2.

---

## Evidence checklist

| Evidence | Path | Status | Catatan |
|---|---|---|---|
| Toolchain versions | `build/meta/toolchain-versions.txt` | PASS | Metadata toolchain berhasil dibuat |
| Host readiness | `build/meta/host-readiness.txt` | PASS | Informasi host WSL berhasil dicatat |
| QEMU capabilities | `build/meta/qemu-capabilities.txt` | PASS | QEMU q35 dan OVMF terdeteksi |
| Freestanding object | `build/proof/freestanding_probe.o` | PASS | Object ELF64 berhasil dibuat |
| Freestanding ELF | `build/proof/freestanding_probe.elf` | PASS | ELF freestanding berhasil dibuat |
| ELF header | `build/proof/readelf-header.txt` | PASS | Header ELF berhasil diverifikasi |
| ELF sections | `build/proof/readelf-sections.txt` | PASS | Section ELF berhasil diverifikasi |
| Disassembly | `build/proof/objdump-disassembly.txt` | PASS | Disassembly object tersedia |
| Undefined symbol report | `build/proof/nm-undefined.txt` | PASS | Tidak ada undefined symbol |
| Reproducibility hash | `build/repro/sha256-run1.txt`, `build/repro/sha256-run2.txt` | PASS | Hash reproducible identik |

---

## Acceptance criteria M1

| Kriteria | Lulus/Gagal | Bukti |
|---|---|---|
| Repository berada di filesystem Linux WSL | Lulus | Repository berada di `~/src/mcsos` |
| Semua tool wajib tersedia | Lulus | `make check` |
| `make meta` berhasil | Lulus | `toolchain-versions.txt` dan `host-readiness.txt` |
| `make check` berhasil | Lulus | Semua command terdeteksi |
| `make proof` berhasil | Lulus | ELF freestanding berhasil dibuat |
| `make qemu-probe` berhasil | Lulus | QEMU q35 dan OVMF tersedia |
| `make repro` berhasil | Lulus | Hash reproducibility identik |
| `make test` berhasil dari clean checkout | Lulus | Semua target Makefile berhasil |
| `nm-undefined.txt` kosong | Lulus | Tidak ada unresolved symbol |
| Hasil `readelf` menunjukkan ELF64 x86_64 | Lulus | `readelf-header.txt` |

---

## Known limitations

- Belum menggunakan cross compiler `x86_64-elf-gcc`.
- Belum terdapat continuous integration (CI).
- Belum terdapat bootable kernel image.
- Belum terdapat hardware test langsung.
- Belum terdapat linker script kernel penuh.
- Belum terdapat implementasi bootloader dan subsystem kernel.

---

## Risiko dan mitigasi

| Risiko | Dampak | Mitigasi |
|---|---|---|
| Repository berada di `/mnt/c` | Permission dan symlink tidak stabil | Repository dipindahkan ke filesystem Linux WSL |
| Toolchain tidak lengkap | Build gagal dijalankan | Validasi otomatis menggunakan `check_toolchain.sh` |
| ELF masih memiliki undefined symbol | Kernel tidak dapat dilink dengan benar | Validasi menggunakan `nm -u` |
| OVMF tidak tersedia | QEMU UEFI boot gagal pada M2 | Validasi menggunakan `qemu_probe.sh` |
| Build tidak reproducible | Sulit diverifikasi dan diaudit | Menggunakan `repro_check.sh` dan SHA256 |

---

## Readiness decision

Pilih salah satu:

- [ ] Belum siap lanjut M2.
- [ ] Siap lanjut M2 dengan catatan.
- [✓] Siap lanjut M2.

Alasan keputusan:

Seluruh acceptance criteria M1 telah terpenuhi. Toolchain freestanding x86_64 berhasil diverifikasi, artefak proof ELF dapat dihasilkan secara reproducible, dan seluruh script validasi berhasil dijalankan melalui Makefile tanpa dependency terhadap hosted runtime environment.
