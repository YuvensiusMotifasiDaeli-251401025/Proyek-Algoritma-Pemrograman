-- ============================================================
--  MoneyTracker Pro - Database Setup
--  Jalankan: mysql -u root -p < setup.sql
-- ============================================================

CREATE DATABASE IF NOT EXISTS moneytracker
  CHARACTER SET utf8mb4
  COLLATE utf8mb4_unicode_ci;

USE moneytracker;

-- Tabel kategori
CREATE TABLE IF NOT EXISTS categories (
    id       INT AUTO_INCREMENT PRIMARY KEY,
    name     VARCHAR(50)  NOT NULL,
    type     ENUM('income','expense') NOT NULL,
    icon     VARCHAR(10)  DEFAULT '📌'
) ENGINE=InnoDB;

-- Tabel transaksi
CREATE TABLE IF NOT EXISTS transactions (
    id          INT AUTO_INCREMENT PRIMARY KEY,
    type        ENUM('income','expense') NOT NULL,
    category_id INT,
    amount      DECIMAL(15,2) NOT NULL,
    description VARCHAR(255) DEFAULT '',
    created_at  DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (category_id) REFERENCES categories(id) ON DELETE SET NULL
) ENGINE=InnoDB;

-- Tabel riwayat chat AI
CREATE TABLE IF NOT EXISTS ai_chat_history (
    id         INT AUTO_INCREMENT PRIMARY KEY,
    role       ENUM('user','assistant') NOT NULL,
    content    TEXT NOT NULL,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB;

-- Data awal kategori
INSERT INTO categories (name, type, icon) VALUES
  ('Gaji',           'income',  '💼'),
  ('Investasi',      'income',  '📊'),
  ('Freelance',      'income',  '💻'),
  ('Bonus',          'income',  '🎁'),
  ('Lainnya Masuk',  'income',  '📥'),
  ('Makanan',        'expense', '🍜'),
  ('Transportasi',   'expense', '🚗'),
  ('Belanja',        'expense', '🛍️'),
  ('Hiburan',        'expense', '🎮'),
  ('Kesehatan',      'expense', '💊'),
  ('Tagihan',        'expense', '🧾'),
  ('Pendidikan',     'expense', '📚'),
  ('Lainnya Keluar', 'expense', '📤');

SELECT 'Database MoneyTracker berhasil dibuat!' AS status;
