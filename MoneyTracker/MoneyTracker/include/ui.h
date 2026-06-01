#pragma once
// ============================================================
//  ui.h  —  Tampilan terminal berwarna & helper
// ============================================================
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include "database.h"

// ── ANSI Color codes ─────────────────────────────────────────
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define DIM     "\033[2m"

#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[97m"

#define BG_BLACK   "\033[40m"
#define BG_BLUE    "\033[44m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN    "\033[46m"

// ── Format Rupiah ─────────────────────────────────────────────
inline std::string format_rp(double amount) {
    std::ostringstream oss;
    long long val = static_cast<long long>(amount);
    std::string s = std::to_string(val);
    std::string result;
    int count = 0;
    for (int i = s.size() - 1; i >= 0; --i) {
        if (count && count % 3 == 0) result = "." + result;
        result = s[i] + result;
        count++;
    }
    return "Rp " + result;
}

// ── Garis ─────────────────────────────────────────────────────
inline void line(char c = '─', int len = 60) {
    std::cout << DIM;
    for (int i = 0; i < len; i++) std::cout << c;
    std::cout << RESET << "\n";
}

inline void line_bold(int len = 60) {
    std::cout << CYAN;
    for (int i = 0; i < len; i++) std::cout << "═";
    std::cout << RESET << "\n";
}

// ── Header App ────────────────────────────────────────────────
inline void print_header() {
    std::cout << "\n";
    line_bold(60);
    std::cout << BOLD << CYAN
              << "   💎  MoneyTracker Pro  —  C++ + MySQL + AI\n"
              << RESET;
    line_bold(60);
    std::cout << "\n";
}

// ── Menu ──────────────────────────────────────────────────────
inline void print_menu() {
    std::cout << BOLD << WHITE << "  MENU UTAMA\n" << RESET;
    line('─', 40);
    std::cout << "  " << CYAN << "[1]" << RESET << " 📈  Tambah Pemasukan\n";
    std::cout << "  " << CYAN << "[2]" << RESET << " 📉  Tambah Pengeluaran\n";
    std::cout << "  " << CYAN << "[3]" << RESET << " 📋  Lihat Transaksi\n";
    std::cout << "  " << CYAN << "[4]" << RESET << " 📊  Ringkasan Keuangan\n";
    std::cout << "  " << CYAN << "[5]" << RESET << " 🤖  Chat AI Advisor\n";
    std::cout << "  " << CYAN << "[6]" << RESET << " 🗑️   Hapus Transaksi\n";
    std::cout << "  " << CYAN << "[0]" << RESET << " 🚪  Keluar\n";
    line('─', 40);
    std::cout << BOLD << "  Pilihan: " << RESET;
}

// ── Tampilkan Ringkasan ───────────────────────────────────────
inline void print_summary(const FinancialSummary& s) {
    std::cout << "\n";
    line_bold(60);
    std::cout << BOLD << "  📊  RINGKASAN KEUANGAN\n" << RESET;
    line('─', 60);

    // Income
    std::cout << "  " << GREEN << BOLD << "📈 Total Pemasukan  : "
              << std::left << std::setw(20) << format_rp(s.total_income)
              << RESET << "\n";

    // Expense
    std::cout << "  " << RED << BOLD << "📉 Total Pengeluaran: "
              << std::left << std::setw(20) << format_rp(s.total_expense)
              << RESET << "\n";

    line('─', 60);

    // Balance
    std::string bal_color = s.balance >= 0 ? GREEN : RED;
    std::cout << "  " << bal_color << BOLD
              << "💰 Saldo Bersih     : "
              << std::left << std::setw(20) << format_rp(s.balance)
              << RESET << "\n";

    std::cout << "  " << DIM
              << "   Total Transaksi  : " << s.total_transactions
              << " transaksi" << RESET << "\n";

    line_bold(60);
}

// ── Tampilkan Daftar Transaksi ────────────────────────────────
inline void print_transactions(const std::vector<Transaction>& txs) {
    if (txs.empty()) {
        std::cout << YELLOW << "\n  ⚠️  Belum ada transaksi.\n" << RESET;
        return;
    }

    std::cout << "\n";
    line_bold(72);
    std::cout << BOLD
              << std::left
              << "  " << std::setw(5)  << "ID"
              << std::setw(10) << "Tipe"
              << std::setw(16) << "Kategori"
              << std::setw(18) << "Jumlah"
              << std::setw(14) << "Tanggal"
              << "Deskripsi\n"
              << RESET;
    line('─', 72);

    for (const auto& t : txs) {
        std::string color = (t.type == "income") ? GREEN : RED;
        std::string sign  = (t.type == "income") ? "+" : "-";
        std::string tipe  = (t.type == "income") ? "Masuk" : "Keluar";

        std::cout
            << "  " << DIM << std::left << std::setw(5) << t.id << RESET
            << color << BOLD << std::setw(10) << tipe << RESET
            << std::setw(16) << (t.icon + " " + t.category)
            << color << BOLD
            << std::setw(18) << (sign + format_rp(t.amount))
            << RESET
            << DIM << std::setw(14) << t.created_at << RESET
            << t.description << "\n";
    }
    line('─', 72);
    std::cout << "  " << DIM << "Menampilkan " << txs.size()
              << " transaksi terbaru\n" << RESET;
}

// ── Print Pesan AI ────────────────────────────────────────────
inline void print_ai_response(const std::string& text) {
    std::cout << "\n";
    line('─', 60);
    std::cout << MAGENTA << BOLD << "  🤖 AI Financial Advisor:\n" << RESET;
    line('─', 60);

    // Word-wrap sederhana
    std::istringstream iss(text);
    std::string line_str;
    while (std::getline(iss, line_str)) {
        std::cout << "  " << CYAN << line_str << RESET << "\n";
    }
    line('─', 60);
}
