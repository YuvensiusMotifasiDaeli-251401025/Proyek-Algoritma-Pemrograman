# 💰 Money Tracker - Qt C++ App

Aplikasi pencatat keuangan pribadi berbasis Qt6 C++ dengan tampilan modern gelap.

## 📦 File Proyek

```
moneytracker/
├── main.cpp           → Entry point aplikasi
├── mainwindow.h       → Header class MainWindow & struct Transaction
├── mainwindow.cpp     → Implementasi UI dan logika
├── MoneyTracker.pro   → Qt qmake project file
└── CMakeLists.txt     → CMake build file (alternatif)
```

## ✨ Fitur

| Tab | Fitur |
|-----|-------|
| 📊 Dashboard | Kartu saldo, pemasukan, pengeluaran + tabel transaksi terbaru |
| 💳 Transaksi | Form tambah transaksi, filter, hapus, tabel lengkap |
| 📈 Grafik | Pie chart pengeluaran per kategori + Bar chart bulanan |

## 🚀 Cara Build & Jalankan

### Menggunakan Qt Creator (Recommended)
1. Buka **Qt Creator**
2. Klik **File → Open File or Project**
3. Pilih `MoneyTracker.pro` atau `CMakeLists.txt`
4. Klik **Configure Project** → pilih Qt6 kit
5. Tekan **Ctrl+R** untuk build & run

### Menggunakan Command Line (qmake)
```bash
cd moneytracker
qmake MoneyTracker.pro
make           # Linux/Mac
nmake          # Windows (MSVC)
mingw32-make   # Windows (MinGW)
./MoneyTracker
```

### Menggunakan CMake
```bash
cd moneytracker
cmake -B build -DCMAKE_PREFIX_PATH=/path/to/Qt6
cmake --build build
./build/MoneyTracker
```

## 📋 Requirements

- Qt 6.x (dengan modul `Charts`)
- Compiler C++17: GCC, Clang, atau MSVC
- Qt Creator (opsional, tapi sangat direkomendasikan)

### Instalasi Qt6 (jika belum ada)
- **Windows/Mac**: Unduh dari https://www.qt.io/download-qt-installer
- **Ubuntu/Debian**: `sudo apt install qt6-base-dev qt6-charts-dev`
- **Fedora**: `sudo dnf install qt6-qtbase-devel qt6-qtcharts-devel`

## 🗂 Struktur Data

```cpp
struct Transaction {
    int     id;          // ID unik auto-increment
    QString type;        // "income" atau "expense"
    QString category;    // Kategori transaksi
    QString description; // Deskripsi
    double  amount;      // Jumlah (Rupiah)
    QDate   date;        // Tanggal transaksi
};
```

## 🎨 Tampilan

- Dark theme dengan palet biru-ungu (#7c83fd)
- Kartu ringkasan berwarna (hijau = pemasukan, merah = pengeluaran)
- Tabel dengan alternating rows
- Pie chart & Bar chart animasi

## 💡 Cara Penggunaan

1. **Tab Dashboard** → Lihat ringkasan keuangan Anda
2. **Tab Transaksi** → Tambah/hapus transaksi, filter berdasarkan tipe/kategori
3. **Tab Grafik** → Analisis visual pengeluaran dan tren bulanan
