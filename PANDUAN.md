# 💎 MoneyTracker Pro — Panduan Lengkap
### C++ · MySQL · Anthropic Claude AI

---

## 📁 Struktur Proyek

```
moneytracker/
├── main.cpp                ← Program utama
├── Makefile                ← Build script
├── include/
│   ├── config.h            ← ⚙️  Konfigurasi DB & API Key
│   ├── database.h          ← Semua fungsi MySQL
│   ├── ai_client.h         ← Integrasi Claude AI via HTTP
│   └── ui.h                ← Tampilan terminal berwarna
└── sql/
    └── setup.sql           ← Script buat database & tabel
```

---

## 🛠️ LANGKAH 1 — Install Dependencies

### Ubuntu / Debian / WSL

```bash
# Update package list
sudo apt update

# Install compiler C++
sudo apt install -y g++ make

# Install MySQL client library (bukan server)
sudo apt install -y libmysqlclient-dev

# Install MySQL server (jika belum ada)
sudo apt install -y mysql-server

# Install libcurl (untuk HTTP ke Anthropic API)
sudo apt install -y libcurl4-openssl-dev
```

### macOS (Homebrew)

```bash
# Install Homebrew jika belum: https://brew.sh
brew install mysql-client curl

# Export path (tambahkan ke ~/.zshrc atau ~/.bash_profile)
export PKG_CONFIG_PATH="/usr/local/opt/mysql-client/lib/pkgconfig"
export CPPFLAGS="-I/usr/local/opt/mysql-client/include"
export LDFLAGS="-L/usr/local/opt/mysql-client/lib"
```

### Windows

Gunakan **WSL2** (Windows Subsystem for Linux) dengan Ubuntu,
lalu ikuti langkah Ubuntu di atas. Ini cara paling mudah.

```powershell
# Buka PowerShell sebagai Admin, lalu:
wsl --install -d Ubuntu
# Restart PC, lalu buka Ubuntu dan ikuti langkah Ubuntu
```

---

## ⚙️ LANGKAH 2 — Konfigurasi

### 2a. Edit `include/config.h`

Buka file ini dan sesuaikan:

```cpp
// ── MySQL ──────────────────────────────────────
#define DB_HOST  "localhost"
#define DB_USER  "root"          // ← ganti user MySQL kamu
#define DB_PASS  "password"      // ← ganti password MySQL kamu
#define DB_NAME  "moneytracker"
#define DB_PORT  3306

// ── Anthropic API ──────────────────────────────
#define ANTHROPIC_API_KEY  "sk-ant-api03-XXXXX..."  // ← ganti dengan API key kamu
```

### 2b. Dapatkan Anthropic API Key

1. Buka **https://console.anthropic.com**
2. Daftar / Login dengan akun kamu
3. Masuk ke menu **"API Keys"**
4. Klik **"Create Key"** → copy key yang muncul
5. Tempel key tersebut ke `config.h` di bagian `ANTHROPIC_API_KEY`

> ⚠️ **Jangan share API key kamu ke siapapun!**

---

## 🗄️ LANGKAH 3 — Setup Database MySQL

### 3a. Jalankan MySQL dan buat user (jika perlu)

```bash
# Masuk ke MySQL sebagai root
sudo mysql -u root

# Di dalam MySQL shell:
ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY 'password_baru';
FLUSH PRIVILEGES;
EXIT;
```

### 3b. Buat database dan tabel

```bash
# Dari folder proyek moneytracker/
mysql -u root -p < sql/setup.sql

# Atau gunakan Makefile:
make db-setup
```

Kalau berhasil, akan muncul:
```
Database MoneyTracker berhasil dibuat!
```

### 3c. Verifikasi (opsional)

```bash
mysql -u root -p moneytracker

# Di dalam MySQL:
SHOW TABLES;
SELECT * FROM categories;
```

---

## 🔨 LANGKAH 4 — Compile & Jalankan

### Compile

```bash
# Masuk ke folder proyek
cd moneytracker

# Compile
make

# Output jika berhasil:
#   ✅  Build berhasil! Jalankan dengan: ./moneytracker
```

### Jalankan

```bash
./moneytracker
```

---

## 🎮 CARA MENGGUNAKAN APLIKASI

### Menu Utama

Setelah dijalankan, kamu akan melihat:

```
═══════════════════════════════════════════════════════════
   💎  MoneyTracker Pro  —  C++ + MySQL + AI
═══════════════════════════════════════════════════════════

  📈 Rp 5.000.000   📉 Rp 2.300.000   💰 Rp 2.700.000

  MENU UTAMA
  ────────────────────────────
  [1] 📈  Tambah Pemasukan
  [2] 📉  Tambah Pengeluaran
  [3] 📋  Lihat Transaksi
  [4] 📊  Ringkasan Keuangan
  [5] 🤖  Chat AI Advisor
  [6] 🗑️   Hapus Transaksi
  [0] 🚪  Keluar
  ────────────────────────────
  Pilihan:
```

### Menu 1 & 2 — Tambah Transaksi

1. Pilih **[1]** untuk pemasukan atau **[2]** untuk pengeluaran
2. Pilih kategori dari daftar yang muncul (ketik nomor)
3. Masukkan jumlah dalam Rupiah (tanpa titik/koma)
4. Masukkan deskripsi (opsional, tekan Enter untuk skip)
5. Transaksi langsung tersimpan ke MySQL

### Menu 3 — Lihat Transaksi

Menampilkan 30 transaksi terbaru dengan format tabel:
- Warna **hijau** = pemasukan
- Warna **merah** = pengeluaran

### Menu 4 — Ringkasan Keuangan

Menampilkan:
- Total pemasukan, pengeluaran, dan saldo
- Top 5 pengeluaran per kategori

### Menu 5 — 🤖 Chat AI Advisor

Ini fitur unggulan! Kamu bisa ngobrol dengan AI tentang keuangan:

```
  Kamu: analisis
  → AI akan analisis kondisi keuanganmu berdasarkan data nyata

  Kamu: tips
  → AI berikan 5 tips hemat berdasarkan pola belanjamu

  Kamu: investasi
  → AI berikan panduan mulai investasi sesuai kondisi keuanganmu

  Kamu: [pertanyaan bebas]
  → Tanya apa saja! Contoh:
     "Kenapa saldo saya terus berkurang?"
     "Bagaimana cara membuat anggaran bulanan?"
     "Berapa % gaji yang ideal untuk ditabung?"

  Kamu: exit
  → Keluar dari sesi AI
```

> **Cara kerja AI:** Setiap sesi, AI secara otomatis membaca data keuanganmu dari MySQL (total pemasukan, pengeluaran, saldo, top pengeluaran) dan menggunakannya sebagai konteks. Jadi sarannya personal dan relevan!

### Menu 6 — Hapus Transaksi

1. Daftar transaksi terbaru akan ditampilkan
2. Masukkan **ID** transaksi yang ingin dihapus
3. Konfirmasi dengan **y** atau batalkan dengan **n**

---

## ❌ Troubleshooting

### Error: `libmysqlclient not found`
```bash
sudo apt install -y libmysqlclient-dev
# atau di macOS:
brew install mysql-client
```

### Error: `Koneksi DB gagal: Access denied`
- Cek username & password di `include/config.h`
- Pastikan MySQL sudah running: `sudo service mysql start`

### Error: `Koneksi DB gagal: Unknown database`
- Jalankan ulang: `mysql -u root -p < sql/setup.sql`

### Error: `[libcurl error]` saat pakai AI
- Cek koneksi internet
- Pastikan API key di `config.h` sudah benar (mulai dengan `sk-ant-`)
- Cek saldo/quota di https://console.anthropic.com

### AI tidak merespons / respons kosong
- API key mungkin sudah expired atau habis kuota
- Coba generate API key baru di console.anthropic.com

---

## 🏗️ Skema Database

```sql
categories          transactions            ai_chat_history
──────────────      ──────────────────      ───────────────
id (PK)             id (PK)                 id (PK)
name                type (income/expense)   role (user/assistant)
type                category_id (FK)        content
icon                amount                  created_at
                    description
                    created_at
```

---

## 📦 Dependensi Ringkasan

| Library         | Fungsi                        | Install                         |
|-----------------|-------------------------------|---------------------------------|
| `libmysqlclient`| Koneksi ke MySQL              | `apt install libmysqlclient-dev`|
| `libcurl`       | HTTP request ke Anthropic API | `apt install libcurl4-openssl-dev`|
| `g++ 17`        | Compiler C++17                | `apt install g++`               |

---

*MoneyTracker Pro — dibuat dengan C++, MySQL, dan Claude AI*
