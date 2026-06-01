#pragma once
// ============================================================
//  config.h  —  Konfigurasi koneksi & API
//  Edit file ini sesuai setting lokal kamu!
// ============================================================

// ── MySQL ────────────────────────────────────────────────────
#define DB_HOST     "localhost"
#define DB_USER     "root"          // ganti dengan user MySQL kamu
#define DB_PASS     "password"      // ganti dengan password MySQL kamu
#define DB_NAME     "moneytracker"
#define DB_PORT     3306

// ── Anthropic API ────────────────────────────────────────────
// Dapatkan API key di: https://console.anthropic.com
#define ANTHROPIC_API_KEY   "sk-ant-XXXXXXXXXXXXXXXXXXXXXXXX"
#define ANTHROPIC_MODEL     "claude-sonnet-4-20250514"
#define ANTHROPIC_MAX_TOK   1024
