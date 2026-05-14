# Laporan Praktikum M0 — Baseline Requirements, Governance, dan Lingkungan Pengembangan Reproducible MCSOS 260502

## 1. Sampul

- Judul Praktikum:
  Praktikum M0 — Baseline Requirements, Governance, dan Lingkungan Pengembangan Reproducible MCSOS 260502

- Nama Mahasiswa:
  - Moch Fariel Aurizki
  - Mikail Khairu Rahman
 
- Kelompok: Cacing Naga 
  -

- NIM:
  - 25832072007 
  - 25832073005

- Kelas:
  - PTI 1A

- Dosen:
  Muhaemin Sidiq, S.Pd., M.Pd.

- Program Studi:
  Pendidikan Teknologi Informasi, Institut Pendidikan Indonesia

- Tanggal:
  - 06/05/2026

---

## 2. Tujuan

Praktikum M0 bertujuan untuk:

1. Menyiapkan lingkungan pengembangan yang konsisten untuk pengembangan sistem operasi MCSOS 260502.
2. Memahami konsep host dan target dalam pengembangan sistem operasi.
3. Menginstal dan mengonfigurasi WSL 2 sebagai lingkungan Linux pada Windows.
4. Menginstal toolchain seperti Git, Clang, NASM, QEMU, GDB, Python, dan tools pendukung lainnya.
5. Membuat struktur repository standar sebagai baseline awal proyek.
6. Melakukan validasi lingkungan menggunakan script `check_env.sh`.
7. Membuat dan menguji smoke object menggunakan compiler freestanding.
8. Memverifikasi object ELF64 x86_64.
9. Menerapkan konsep reproducibility dan evidence-first engineering.

---

## 3. Dasar Teori Ringkas

### 3.1 Host vs Target

Host merupakan sistem yang digunakan untuk melakukan proses pengembangan seperti menulis kode, kompilasi, dan menjalankan tools. Target merupakan sistem atau arsitektur yang menjalankan hasil program. Pada praktikum ini Windows 11 dengan WSL digunakan sebagai host, sedangkan target menggunakan arsitektur x86_64.

### 3.2 WSL 2 (Windows Subsystem for Linux 2)

WSL 2 merupakan fitur Windows yang memungkinkan pengguna menjalankan Linux secara langsung tanpa dual boot. WSL menggunakan kernel Linux asli sehingga kompatibilitas aplikasi Linux lebih baik.

### 3.3 Cross-Compilation

Cross-compilation merupakan proses kompilasi program pada satu sistem untuk menghasilkan program pada arsitektur yang berbeda.

### 3.4 ELF Object

ELF (Executable and Linkable Format) merupakan format file executable, library, dan object file pada Linux.

### 3.5 QEMU

QEMU merupakan emulator yang digunakan untuk menjalankan sistem operasi pada lingkungan virtual.

### 3.6 OVMF

OVMF (Open Virtual Machine Firmware) merupakan implementasi firmware UEFI pada lingkungan virtual.

### 3.7 Git

Git merupakan Version Control System (VCS) yang digunakan untuk mengelola perubahan kode.

### 3.8 Reproducibility

Reproducibility memastikan proses build menghasilkan output yang sama ketika dijalankan pada lingkungan yang sama.

### 3.9 Evidence-First Engineering

Evidence-first engineering merupakan pendekatan yang menekankan penggunaan bukti melalui log, metadata, dan hasil pengujian.

---

## 4. Lingkungan

| Komponen | Versi / Output |
|-----------|----------------|
| Windows | |
| WSL Distro | |
| Kernel Linux WSL | |
| Git | |
| Clang | |
| LLD | |
| Binutils / Readelf | |
| NASM | |
| QEMU | |
| GDB | |
| Python | |

Lampirkan isi file:

`build/meta/toolchain-versions.txt`

---

## 5. Desain Baseline

Jelaskan:

- ## 5. Desain Baseline

Desain baseline pada M0 dibuat untuk menyediakan struktur awal yang konsisten sehingga proses pengembangan dapat dilakukan dengan standar yang sama. Baseline ini belum berfokus pada implementasi kernel, tetapi mempersiapkan fondasi pengembangan.

### 5.1 Struktur Repository

Struktur repository digunakan untuk memisahkan fungsi setiap komponen agar lebih mudah dikelola.

Contoh struktur:

```text
.
├── build
│   ├── meta
│   └── smoke
├── docs
│   ├── adr
│   ├── architecture
│   ├── governance
│   ├── operations
│   ├── reports
│   ├── requirements
│   ├── security
│   └── testing
├── smoke
├── tools
├── README.md
├── Makefile
└── .gitignore

---

## 6. Langkah Kerja

Tuliskan:

- Perintah yang dijalankan
- Alasan teknis
- Hasil yang diperoleh

Contoh langkah:

1. Menginstal WSL 2.
2. Menginstal toolchain.
3. Membuat struktur repository M0.
4. Membuat file baseline.
5. Menjalankan `make check`.
6. Menjalankan `make smoke`.
7. Memverifikasi hasil object.

## 6. Langkah Kerja

Langkah-langkah yang dilakukan pada praktikum M0 adalah sebagai berikut:

### 6.1 Menyiapkan WSL 2

Perintah:

```bash
wsl --install
```

Alasan:

Menginstal Windows Subsystem for Linux versi 2 sebagai lingkungan Linux pada Windows.

Hasil:

WSL berhasil terinstal dan dapat menjalankan Ubuntu.

---

### 6.2 Menginstal Toolchain

Perintah:

```bash
sudo apt update

sudo apt install -y \
git make clang lld llvm \
binutils nasm qemu-system-x86 \
gdb python3 shellcheck cppcheck tree
```

Alasan:

Menginstal tools yang diperlukan untuk pengembangan MCSOS.

Hasil:

Seluruh tool berhasil terinstal.

---

### 6.3 Mengonfigurasi Git

Perintah:

```bash
git config --global user.name "Mikail"
git config --global user.email "Mikailkhairurahman@gmail.com"
git config --global init.defaultBranch main
git config --global core.autocrlf input
git config --global pull.rebase false
```

Alasan:

Mengatur identitas pengguna Git dan konfigurasi dasar repository.

Hasil:

Konfigurasi Git berhasil disimpan.

---

### 6.4 Membuat Struktur Repository

Perintah:

```bash
mkdir -p \
docs/adr \
docs/architecture \
docs/requirements \
docs/security \
docs/testing \
docs/governance \
docs/operations \
docs/reports \
tools \
smoke \
build/meta \
build/smoke
```

Alasan:

Membuat struktur direktori awal M0.

Hasil:

Struktur direktori berhasil dibuat.

---

### 6.5 Membuat File Baseline

Perintah:

```bash
nano README.md
nano .gitignore
nano Makefile
```

Alasan:

Membuat file dasar yang dibutuhkan pada project.

Hasil:

File baseline berhasil dibuat.

---

### 6.6 Membuat Script Validasi Lingkungan

Perintah:

```bash
nano tools/check_env.sh
chmod +x tools/check_env.sh
```

Alasan:

Membuat script untuk memvalidasi toolchain dan lingkungan.

Hasil:

Script berhasil dibuat dan dapat dijalankan.

---

### 6.7 Menjalankan Validasi Lingkungan

Perintah:

```bash
make check
```

Alasan:

Memeriksa apakah seluruh tools telah tersedia.

Hasil:

Semua tool terdeteksi dengan status `[OK]`.

---

### 6.8 Menjalankan Smoke Test

Perintah:

```bash
make smoke
```

Alasan:

Melakukan pengujian awal object freestanding.

Hasil:

Object file berhasil dibuat.

---

### 6.9 Verifikasi File Object

Perintah:

```bash
readelf -h build/smoke/freestanding.o
```

Alasan:

Memastikan object file menggunakan target ELF64 x86_64.

Hasil:

Output menunjukkan:

```text
ELF 64-bit LSB relocatable, x86-64
```

yang menunjukkan bahwa proses build berhasil.

---

## 7. Hasil Uji

## 7. Hasil Uji

Pengujian dilakukan untuk memastikan lingkungan pengembangan M0 telah dikonfigurasi dengan benar dan toolchain dapat berjalan sesuai kebutuhan.

| Pengujian | Command | Hasil | Pass/Fail |
|---|---|---|---|
| WSL Version | `wsl --list --verbose` | Ubuntu berhasil terdeteksi dan menggunakan WSL versi 2 | PASS |
| Tool Check | `bash tools/check_env.sh` | Semua tools berhasil terdeteksi dengan status `[OK]` | PASS |
| Metadata | `cat build/meta/toolchain-versions.txt` | Metadata toolchain berhasil dibuat | PASS |
| Smoke Object | `make smoke` | Object file berhasil dibuat tanpa error | PASS |
| ELF Header | `readelf -h build/smoke/freestanding.o` | Menampilkan ELF 64-bit LSB relocatable x86-64 | PASS |
| Git Status | `git status` | Repository berhasil terdeteksi oleh Git | PASS |

### Output Validasi Lingkungan

Output dari `make check`:

```text
[M0] Repository root: /root/src/mcsos
[OK] Repository is not under /mnt/<drive>.
[M0] Checking required tools
[OK] git
[OK] make
[OK] clang
[OK] ld.lld
[OK] llvm-readelf
[OK] llvm-objdump
[OK] readelf
[OK] objdump
[OK] nasm
[OK] qemu-system-x86_64
[OK] gdb
[OK] python3
[OK] shellcheck
[OK] cppcheck
[M0] Environment check completed
```

### Output Smoke Test

Perintah:

```bash
make smoke
```

Hasil:

```text
build/smoke/freestanding.o:
ELF 64-bit LSB relocatable, x86-64
```

### Verifikasi Object ELF

Perintah:

```bash
readelf -h build/smoke/freestanding.o
```

Hasil menunjukkan:

```text
Class: ELF64
Machine: Advanced Micro Devices X86-64
Type: REL (Relocatable file)
```

Berdasarkan hasil pengujian di atas, seluruh proses validasi lingkungan M0 berhasil dijalankan dengan baik.

---

## 8. Analisis

Pada pelaksanaan praktikum M0 terdapat beberapa kendala selama konfigurasi lingkungan.

### 8.1 Kendala yang Ditemukan

1. Error instalasi WSL:

A distribution with the supplied name already exists

2. Error command:

wsl: command not found

3. Error permission:

Elevated permissions are required to run DISM

4. Error password Linux:

passwd: Authentication token manipulation error

5. Error script:

EOF: command not found

### 8.2 Penyebab Masalah

- Distribusi Linux sebelumnya sudah terinstal.
- Perintah dijalankan pada terminal yang salah.
- CMD tidak dijalankan sebagai Administrator.
- Konfigurasi user Linux belum selesai.
- Kesalahan saat menyalin script.

### 8.3 Solusi dan Perbaikan

- Mengecek daftar WSL:

`wsl --list --verbose`

- Menjalankan CMD sebagai Administrator.
- Mengonfigurasi ulang user Linux.
- Memeriksa script:

`nano tools/check_env.sh`

- Menghapus EOF yang tidak sesuai.
- Menjalankan ulang:

`make check`

`make smoke`

### 8.4 Bukti Keberhasilan

- Semua tools berhasil terdeteksi `[OK]`
- Metadata berhasil dibuat
- Smoke object berhasil dibuat
- Validasi menunjukkan:

ELF 64-bit LSB relocatable, x86-64

---

## 9. Keamanan dan Reliability

Pada pengembangan sistem operasi, keamanan dan reliability penting untuk menjaga kestabilan lingkungan pengembangan.

### 9.1 Risiko Supply-Chain

Supply-chain merupakan risiko yang berasal dari package atau software yang tidak terpercaya.

Mitigasi:

- Menggunakan repository resmi Linux
- Menggunakan package manager resmi
- Memverifikasi tools yang diinstal

### 9.2 Toolchain Mismatch

Perbedaan versi toolchain dapat menyebabkan hasil build berbeda.

Dampak:

- Build gagal
- Error kompatibilitas
- Hasil object berbeda

Mitigasi:

- Menggunakan toolchain yang sama
- Menyimpan metadata toolchain
- Menjalankan:

`bash tools/check_env.sh`

### 9.3 Repository Path

Menyimpan project di `/mnt/c` dapat menimbulkan masalah performa dan permission.

Mitigasi:

Gunakan:

`~/src/mcsos`

### 9.4 Permission

Kesalahan permission dapat menyebabkan script gagal berjalan.

Mitigasi:

`chmod +x tools/check_env.sh`

### 9.5 Log Integrity

Log dan metadata perlu dijaga agar bukti pengujian tetap valid.

Mitigasi:

- Menyimpan log pengujian
- Menyimpan metadata
- Menggunakan Git

### 9.6 Reliability yang Diterapkan

1. Validasi toolchain
2. Smoke test
3. Metadata toolchain
4. Verifikasi ELF
5. Version control Git

---

## 10. Failure Modes dan Rollback

## 10. Failure Modes dan Rollback

Failure modes merupakan kondisi kegagalan yang mungkin terjadi selama proses praktikum M0 beserta langkah diagnosis dan perbaikannya.

| Failure Mode | Gejala | Diagnosis | Rollback/Perbaikan |
|---|---:|---|---|
| WSL bukan versi 2 | WSL gagal berjalan atau Ubuntu tidak dapat dijalankan | Versi WSL masih menggunakan WSL 1 atau belum dikonfigurasi | Jalankan `wsl --set-version Ubuntu 2` |
| Distribusi sudah ada | Muncul pesan `A distribution with the supplied name already exists` | Distribusi Ubuntu sebelumnya sudah terinstal | Jalankan `wsl --list --verbose` dan gunakan distribusi yang sudah ada |
| Command `wsl` tidak ditemukan | Muncul `wsl: command not found` | Perintah dijalankan di terminal Linux, bukan CMD/PowerShell | Jalankan perintah pada CMD atau PowerShell |
| Elevated permissions diperlukan | Muncul `Error: 740 Elevated permissions are required` | CMD tidak dijalankan sebagai Administrator | Tutup CMD lalu buka kembali dengan **Run as Administrator** |
| Tool tidak ditemukan | Muncul `command not found` | Package belum terinstal | Instal package menggunakan `apt install` |
| Repository berada di `/mnt/c` | Build lebih lambat atau permission bermasalah | Repository disimpan pada filesystem Windows | Pindahkan project ke `~/src/mcsos` |
| Smoke object salah target | Output ELF tidak menunjukkan x86-64 | Konfigurasi compiler salah | Periksa target compiler dan Makefile |
| Permission script gagal | Script tidak dapat dijalankan | File belum memiliki izin execute | Jalankan `chmod +x tools/check_env.sh` |
| Error script `EOF: command not found` | Script berhenti saat dijalankan | Penulisan EOF tidak sesuai | Perbaiki posisi `EOF` pada script |
| OVMF tidak ditemukan | Emulator gagal berjalan | Firmware belum tersedia | Instal package OVMF |

### Analisis Rollback

Rollback dilakukan dengan mengembalikan konfigurasi atau memperbaiki komponen yang menyebabkan kegagalan. Pada praktikum M0, rollback lebih banyak dilakukan dengan memperbaiki konfigurasi WSL, menginstal ulang package yang belum tersedia, memperbaiki script, dan memvalidasi ulang lingkungan menggunakan:

```bash
make check
```

serta:

```bash
make smoke
```

Setelah perbaikan dilakukan, seluruh tool berhasil terdeteksi dan object file ELF berhasil dibuat sehingga proses M0 dapat dilanjutkan ke tahap berikutnya.

---

## 11. Kesimpulan

Berdasarkan hasil praktikum M0 yang telah dilakukan, proses penyiapan lingkungan pengembangan sistem operasi MCSOS 260502 berhasil dilakukan dengan baik. Lingkungan pengembangan berhasil dikonfigurasi menggunakan WSL 2 pada Windows sebagai host serta arsitektur x86_64 sebagai target pengembangan.

Toolchain yang dibutuhkan seperti Git, Clang, NASM, QEMU, GDB, Python, ShellCheck, dan tools pendukung lainnya berhasil diinstal dan diverifikasi melalui script validasi lingkungan `check_env.sh`. Hasil pengujian menunjukkan seluruh tools dapat terdeteksi dengan status `[OK]`.

Selain itu, pengujian smoke test menggunakan compiler freestanding berhasil menghasilkan object file dengan format:

```text
ELF 64-bit LSB relocatable, x86-64
```

Hasil tersebut menunjukkan bahwa proses kompilasi dan target arsitektur telah sesuai dengan kebutuhan praktikum.

Pada tahap M0 belum dilakukan proses boot kernel ataupun implementasi komponen sistem operasi seperti manajemen memori, scheduler, maupun sistem file. Tahap ini hanya berfokus pada persiapan baseline lingkungan, validasi toolchain, dokumentasi, serta penerapan konsep reproducibility dan evidence-first engineering.

Dengan tercapainya seluruh pengujian dan validasi, lingkungan pengembangan dinyatakan siap untuk melanjutkan ke tahap berikutnya yaitu M1.

---

## 12. Lampiran

- Output `tools/check_env.sh`
- Isi `build/meta/toolchain-versions.txt`
- Output `readelf -h`
- Output `objdump`
- Screenshot relevan
- Commit hash

---

## 13. Referensi

[1] Git Documentation, Git Reference Manual.

[2] Microsoft Documentation, Windows Subsystem for Linux.

[3] QEMU Documentation.

[4] LLVM/Clang Documentation.

[5] Linux ELF Documentation.


