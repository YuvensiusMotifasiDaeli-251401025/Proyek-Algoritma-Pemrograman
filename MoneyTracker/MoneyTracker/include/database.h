#pragma once
// ============================================================
//  database.h  —  Wrapper MySQL
// ============================================================
#include <mysql/mysql.h>
#include <string>
#include <vector>
#include <iostream>
#include "config.h"

struct Transaction {
    int         id;
    std::string type;           // "income" / "expense"
    std::string category;
    std::string icon;
    double      amount;
    std::string description;
    std::string created_at;
};

struct Category {
    int         id;
    std::string name;
    std::string type;
    std::string icon;
};

struct FinancialSummary {
    double total_income;
    double total_expense;
    double balance;
    int    total_transactions;
};

// ── Koneksi Global ───────────────────────────────────────────
inline MYSQL* db_connect() {
    MYSQL* conn = mysql_init(nullptr);
    if (!conn) {
        std::cerr << "mysql_init() gagal\n";
        return nullptr;
    }
    if (!mysql_real_connect(conn,
            DB_HOST, DB_USER, DB_PASS, DB_NAME,
            DB_PORT, nullptr, 0)) {
        std::cerr << "Koneksi DB gagal: " << mysql_error(conn) << "\n";
        mysql_close(conn);
        return nullptr;
    }
    mysql_set_character_set(conn, "utf8mb4");
    return conn;
}

// ── Escape String ────────────────────────────────────────────
inline std::string db_escape(MYSQL* conn, const std::string& s) {
    std::string out(s.size() * 2 + 1, '\0');
    unsigned long len = mysql_real_escape_string(conn, &out[0], s.c_str(), s.size());
    out.resize(len);
    return out;
}

// ── Tambah Transaksi ─────────────────────────────────────────
inline bool db_add_transaction(MYSQL* conn,
        const std::string& type, int category_id,
        double amount, const std::string& desc) {
    std::string safe_desc = db_escape(conn, desc);
    char query[512];
    snprintf(query, sizeof(query),
        "INSERT INTO transactions (type, category_id, amount, description) "
        "VALUES ('%s', %d, %.2f, '%s')",
        type.c_str(), category_id, amount, safe_desc.c_str());
    return mysql_query(conn, query) == 0;
}

// ── Hapus Transaksi ──────────────────────────────────────────
inline bool db_delete_transaction(MYSQL* conn, int id) {
    char query[128];
    snprintf(query, sizeof(query),
        "DELETE FROM transactions WHERE id = %d", id);
    return mysql_query(conn, query) == 0;
}

// ── Ambil Semua Transaksi ────────────────────────────────────
inline std::vector<Transaction> db_get_transactions(MYSQL* conn, int limit = 50) {
    std::vector<Transaction> result;
    char query[256];
    snprintf(query, sizeof(query),
        "SELECT t.id, t.type, IFNULL(c.name,'—'), IFNULL(c.icon,'📌'), "
        "t.amount, t.description, "
        "DATE_FORMAT(t.created_at,'%%d %%b %%Y %%H:%%i') "
        "FROM transactions t "
        "LEFT JOIN categories c ON t.category_id = c.id "
        "ORDER BY t.created_at DESC LIMIT %d", limit);

    if (mysql_query(conn, query)) return result;
    MYSQL_RES* res = mysql_store_result(conn);
    if (!res) return result;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))) {
        Transaction t;
        t.id          = std::stoi(row[0]);
        t.type        = row[1];
        t.category    = row[2];
        t.icon        = row[3];
        t.amount      = std::stod(row[4]);
        t.description = row[5] ? row[5] : "";
        t.created_at  = row[6] ? row[6] : "";
        result.push_back(t);
    }
    mysql_free_result(res);
    return result;
}

// ── Ringkasan Keuangan ───────────────────────────────────────
inline FinancialSummary db_get_summary(MYSQL* conn) {
    FinancialSummary s{0,0,0,0};
    const char* q =
        "SELECT "
        "  SUM(CASE WHEN type='income'  THEN amount ELSE 0 END), "
        "  SUM(CASE WHEN type='expense' THEN amount ELSE 0 END), "
        "  COUNT(*) "
        "FROM transactions";
    if (mysql_query(conn, q)) return s;
    MYSQL_RES* res = mysql_store_result(conn);
    if (!res) return s;
    MYSQL_ROW row = mysql_fetch_row(res);
    if (row) {
        s.total_income      = row[0] ? std::stod(row[0]) : 0;
        s.total_expense     = row[1] ? std::stod(row[1]) : 0;
        s.balance           = s.total_income - s.total_expense;
        s.total_transactions = row[2] ? std::stoi(row[2]) : 0;
    }
    mysql_free_result(res);
    return s;
}

// ── Ambil Kategori ───────────────────────────────────────────
inline std::vector<Category> db_get_categories(MYSQL* conn,
        const std::string& type = "") {
    std::vector<Category> result;
    std::string query = "SELECT id, name, type, icon FROM categories";
    if (!type.empty())
        query += " WHERE type = '" + type + "'";
    query += " ORDER BY id";

    if (mysql_query(conn, query.c_str())) return result;
    MYSQL_RES* res = mysql_store_result(conn);
    if (!res) return result;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))) {
        Category c;
        c.id   = std::stoi(row[0]);
        c.name = row[1];
        c.type = row[2];
        c.icon = row[3] ? row[3] : "📌";
        result.push_back(c);
    }
    mysql_free_result(res);
    return result;
}

// ── Top Pengeluaran per Kategori ─────────────────────────────
inline std::string db_get_expense_breakdown(MYSQL* conn) {
    const char* q =
        "SELECT c.name, SUM(t.amount) AS total "
        "FROM transactions t "
        "JOIN categories c ON t.category_id = c.id "
        "WHERE t.type = 'expense' "
        "GROUP BY c.name ORDER BY total DESC LIMIT 5";
    if (mysql_query(conn, q)) return "";
    MYSQL_RES* res = mysql_store_result(conn);
    if (!res) return "";

    std::string out;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))) {
        char buf[128];
        snprintf(buf, sizeof(buf), "  - %s: Rp %.0f\n", row[0], std::stod(row[1]));
        out += buf;
    }
    mysql_free_result(res);
    return out;
}

// ── Simpan & Ambil Riwayat Chat ──────────────────────────────
inline void db_save_chat(MYSQL* conn,
        const std::string& role, const std::string& content) {
    std::string safe = db_escape(conn, content);
    char query[safe.size() + 128];
    snprintf(query, sizeof(query),
        "INSERT INTO ai_chat_history (role, content) VALUES ('%s', '%s')",
        role.c_str(), safe.c_str());
    mysql_query(conn, query);
}
