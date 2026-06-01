#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QtSql>
#include <QTableWidget>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QDateEdit>
#include <QMessageBox>
#include <QFrame>
#include <QScrollBar>
#include <QSpacerItem>
#include <QStackedWidget>
#include <QGraphicsDropShadowEffect>
#include <QTimer>
#include <QPropertyAnimation>
#include <QFont>
#include <QFontDatabase>
#include <QSettings>
#include <QCryptographicHash>
#include <QDialog>
#include <QWidgetAction>
#include <QShortcut>

// ── Utilitas simpan/baca akun (QSettings) ────────────────────────────────────
namespace AccountStore {
static const QString ORG  = "AlproGang";
static const QString APP  = "MoneyTracker";

inline QString hashPw(const QString &pw) {
    return QCryptographicHash::hash(pw.toUtf8(), QCryptographicHash::Sha256).toHex();
}
inline bool hasAccount() {
    QSettings s(ORG, APP);
    return s.contains("username");
}
inline void save(const QString &user, const QString &pw) {
    QSettings s(ORG, APP);
    s.setValue("username", user);
    s.setValue("pwhash",   hashPw(pw));
}
inline bool verify(const QString &user, const QString &pw) {
    QSettings s(ORG, APP);
    return s.value("username").toString() == user &&
           s.value("pwhash").toString()   == hashPw(pw);
}
inline QString getUsername() {
    QSettings s(ORG, APP);
    return s.value("username").toString();
}
}

// ═══════════════════════════════════════════════════════════════════════════════
//  LOGIN WINDOW
// ═══════════════════════════════════════════════════════════════════════════════

class LoginWindow : public QWidget {
    Q_OBJECT
signals:
    void loginSuccess();
    void goToRegister();   // sinyal pindah ke halaman register

public:
    LoginWindow(QWidget *parent = nullptr) : QWidget(parent) {
        setWindowTitle("Money Tracker Mahasiswa");
        resize(1280, 800);
        setMinimumSize(960, 640);
        setStyleSheet("background:#080a14;");
        setupUI();
    }

private:
    QLineEdit   *usernameInput;
    QLineEdit   *passwordInput;
    QLabel      *errorLabel;
    QPushButton *loginBtn;
    QPushButton *eyeBtn;       // tombol mata untuk login password

    void setupUI() {
        QVBoxLayout *root = new QVBoxLayout(this);
        root->setContentsMargins(0,0,0,0);
        root->setSpacing(0);

        // ── TOP DECORATION ────────────────────────────────────────────────
        QWidget *topDeco = new QWidget();
        topDeco->setFixedHeight(8);
        topDeco->setStyleSheet(R"(
            background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                stop:0 #4f46e5, stop:0.5 #7c3aed, stop:1 #ec4899);
        )");
        root->addWidget(topDeco);

        // ── CENTER CARD ───────────────────────────────────────────────────
        QWidget *center = new QWidget();
        center->setStyleSheet("background:transparent;");
        QVBoxLayout *centerLayout = new QVBoxLayout(center);
        centerLayout->setContentsMargins(50, 40, 50, 40);
        centerLayout->setSpacing(0);

        // Logo Block
        QWidget *logoBlock = new QWidget();
        logoBlock->setFixedSize(80, 80);
        logoBlock->setStyleSheet(R"(
            background: qlineargradient(x1:0,y1:0,x2:1,y2:1,
                stop:0 #4f46e5, stop:1 #7c3aed);
            border-radius: 22px;
        )");
        QVBoxLayout *logoLayout = new QVBoxLayout(logoBlock);
        logoLayout->setContentsMargins(0,0,0,0);
        QLabel *logoText = new QLabel("Rp");
        logoText->setAlignment(Qt::AlignCenter);
        logoText->setStyleSheet("color:white; font-size:28px; font-weight:900; background:transparent;");
        logoLayout->addWidget(logoText);

        // Drop shadow on logo
        QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
        shadow->setBlurRadius(30);
        shadow->setColor(QColor(79, 70, 229, 160));
        shadow->setOffset(0, 8);
        logoBlock->setGraphicsEffect(shadow);

        QWidget *logoWrap = new QWidget();
        logoWrap->setStyleSheet("background:transparent;");
        QHBoxLayout *logoWrapLayout = new QHBoxLayout(logoWrap);
        logoWrapLayout->setContentsMargins(0,0,0,0);
        logoWrapLayout->addStretch();
        logoWrapLayout->addWidget(logoBlock);
        logoWrapLayout->addStretch();

        centerLayout->addWidget(logoWrap);
        centerLayout->addSpacing(24);

        // Title
        QLabel *title = new QLabel("Money Tracker Mahasiswa");
        title->setAlignment(Qt::AlignCenter);
        title->setStyleSheet(R"(
            font-size: 26px;
            font-weight: 800;
            color: #ffffff;
            background: transparent;
            letter-spacing: -0.5px;
        )");
        title->setWordWrap(true);
        centerLayout->addWidget(title);
        centerLayout->addSpacing(6);

        QLabel *subtitle = new QLabel("Alpro Gang");
        subtitle->setAlignment(Qt::AlignCenter);
        subtitle->setStyleSheet("font-size:13px; color:#4f5a8a; background:transparent;");
        centerLayout->addWidget(subtitle);
        centerLayout->addSpacing(36);

        // ── FORM CARD ─────────────────────────────────────────────────────
        QFrame *card = new QFrame();
        card->setStyleSheet(R"(
            QFrame {
                background: #0f1220;
                border-radius: 18px;
                border: 1px solid #1e2544;
            }
        )");
        QGraphicsDropShadowEffect *cardShadow = new QGraphicsDropShadowEffect();
        cardShadow->setBlurRadius(40);
        cardShadow->setColor(QColor(0,0,0,120));
        cardShadow->setOffset(0, 12);
        card->setGraphicsEffect(cardShadow);

        QVBoxLayout *cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(28, 28, 28, 28);
        cardLayout->setSpacing(16);

        // Username
        QLabel *userLabel = new QLabel("Username");
        userLabel->setStyleSheet("font-size:12px; font-weight:600; color:#818cf8; background:transparent;");
        usernameInput = new QLineEdit();
        usernameInput->setPlaceholderText("Masukkan username");
        styleInput(usernameInput);

        // Password
        QLabel *passLabel = new QLabel("Password");
        passLabel->setStyleSheet("font-size:12px; font-weight:600; color:#818cf8; background:transparent;");

        passwordInput = new QLineEdit();
        passwordInput->setPlaceholderText("Masukkan password");
        passwordInput->setEchoMode(QLineEdit::Password);
        styleInput(passwordInput);

        // Ikon mata sebagai QAction di dalam field
        eyeBtn = new QPushButton("👁");
        eyeBtn->setCheckable(true);
        eyeBtn->setFlat(true);
        eyeBtn->setCursor(Qt::PointingHandCursor);
        eyeBtn->setStyleSheet("color:#4f5a8a; font-size:16px; border:none; background:transparent; padding:0 4px;");

        QWidgetAction *eyeAction = new QWidgetAction(passwordInput);
        eyeAction->setDefaultWidget(eyeBtn);
        passwordInput->addAction(eyeAction, QLineEdit::TrailingPosition);

        connect(eyeBtn, &QPushButton::toggled, this, [this](bool checked){
            passwordInput->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
            eyeBtn->setStyleSheet(checked
                                      ? "color:#c8d0e7; font-size:16px; border:none; background:transparent; padding:0 4px;"
                                      : "color:#4f5a8a; font-size:16px; border:none; background:transparent; padding:0 4px;");
        });

        // Error label
        errorLabel = new QLabel("");
        errorLabel->setAlignment(Qt::AlignCenter);
        errorLabel->setStyleSheet(R"(
            font-size:12px;
            color:#fb7185;
            background:rgba(251,113,133,0.08);
            border:1px solid rgba(251,113,133,0.2);
            border-radius:8px;
            padding:8px 12px;
        )");
        errorLabel->setVisible(false);

        // Login Button
        loginBtn = new QPushButton("Masuk  →");
        loginBtn->setCursor(Qt::PointingHandCursor);
        loginBtn->setStyleSheet(R"(
            QPushButton {
                background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                    stop:0 #4f46e5, stop:1 #7c3aed);
                color: white;
                border: none;
                border-radius: 10px;
                padding: 14px;
                font-size: 14px;
                font-weight: 700;
                letter-spacing: 0.3px;
                min-height: 46px;
            }
            QPushButton:hover {
                background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                    stop:0 #6057f5, stop:1 #8b4cfd);
            }
            QPushButton:pressed {
                padding-top:15px; padding-bottom:13px;
            }
        )");

        cardLayout->addWidget(userLabel);
        cardLayout->addWidget(usernameInput);
        cardLayout->addWidget(passLabel);
        cardLayout->addWidget(passwordInput);
        cardLayout->addSpacing(4);
        cardLayout->addWidget(errorLabel);
        cardLayout->addWidget(loginBtn);

        centerLayout->addWidget(card);
        centerLayout->addSpacing(16);

        // Tombol daftar
        QHBoxLayout *regRow = new QHBoxLayout();
        QLabel *regHint = new QLabel("Belum punya akun?");
        regHint->setStyleSheet("font-size:12px; color:#4f5a8a; background:transparent;");
        QPushButton *registerBtn = new QPushButton("Daftar");
        registerBtn->setCursor(Qt::PointingHandCursor);
        registerBtn->setStyleSheet(R"(
            QPushButton {
                background: transparent;
                color: #818cf8;
                border: none;
                font-size: 12px;
                font-weight: 700;
                padding: 0;
                text-decoration: underline;
            }
            QPushButton:hover { color: #a5b4fc; }
        )");
        regRow->setAlignment(Qt::AlignCenter);
        regRow->addWidget(regHint);
        regRow->addWidget(registerBtn);
        centerLayout->addLayout(regRow);

        connect(registerBtn, &QPushButton::clicked, this, &LoginWindow::goToRegister);
        centerLayout->addStretch();

        root->addWidget(center, 1);

        // ── BOTTOM BAR ────────────────────────────────────────────────────
        QWidget *bottomBar = new QWidget();
        bottomBar->setFixedHeight(48);
        bottomBar->setStyleSheet("background:#080a14; border-top: 1px solid #131525;");
        QHBoxLayout *bbLayout = new QHBoxLayout(bottomBar);
        bbLayout->setContentsMargins(20,0,20,0);
        QLabel *footer = new QLabel("© 2026 Alpro Gang · Money Tracker Mahasiswa");
        footer->setStyleSheet("font-size:11px; color:#1e2544; background:transparent;");
        bbLayout->addStretch();
        bbLayout->addWidget(footer);
        bbLayout->addStretch();
        root->addWidget(bottomBar);

        connect(loginBtn, &QPushButton::clicked, this, &LoginWindow::tryLogin);
        connect(usernameInput, &QLineEdit::returnPressed, this, &LoginWindow::tryLogin);
        connect(passwordInput, &QLineEdit::returnPressed, this, &LoginWindow::tryLogin);
    }

    void styleInput(QLineEdit *input) {
        input->setStyleSheet(R"(
            QLineEdit {
                background: #080a14;
                color: #c8d0e7;
                border: 1px solid #252865;
                border-radius: 10px;
                padding: 12px 14px;
                font-size: 13px;
                min-height: 42px;
                selection-background-color: #5a52e0;
            }
            QLineEdit:focus {
                border: 1.5px solid #5a52e0;
                background: #0b0d1e;
            }
            QLineEdit::placeholder {
                color: #2d3464;
            }
        )");
    }

    void tryLogin() {
        QString user = usernameInput->text().trimmed();
        QString pass = passwordInput->text();

        if (AccountStore::verify(user, pass)) {
            emit loginSuccess();
        } else {
            errorLabel->setText("Username atau password salah. Coba lagi!");
            errorLabel->setVisible(true);
            passwordInput->clear();
            loginBtn->setEnabled(false);
            QTimer::singleShot(1500, loginBtn, [this]{ loginBtn->setEnabled(true); });
        }
    }
};


// ═══════════════════════════════════════════════════════════════════════════════
//  MAIN DASHBOARD
// ═══════════════════════════════════════════════════════════════════════════════

class MainWindow : public QMainWindow {
    Q_OBJECT
signals:
    void logoutRequested();
public:
    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        setupDatabase();
        setupUI();
        loadTransactions();
        updateSummary();
    }

private:
    QWidget      *centralWidget;
    QComboBox    *typeBox;
    QComboBox    *categoryBox;
    QLineEdit    *amountInput;
    QLineEdit    *noteInput;
    QDateEdit    *dateInput;
    QPushButton  *addButton;
    QLabel       *incomeLabel;
    QLabel       *expenseLabel;
    QLabel       *balanceLabel;
    QTableWidget *transactionTable;
    QLineEdit    *searchInput;
    QComboBox    *filterTypeBox;
    QPushButton  *deleteButton;

    // ── DATABASE ──────────────────────────────────────────────────────────

    void setupDatabase() {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("moneytracker.db");
        if (!db.open()) {
            QMessageBox::critical(this, "Kesalahan Database", db.lastError().text());
            exit(1);
        }
        QSqlQuery q;
        q.exec(R"(
            CREATE TABLE IF NOT EXISTS transactions (
                id       INTEGER PRIMARY KEY AUTOINCREMENT,
                type     TEXT,
                category TEXT,
                amount   REAL,
                note     TEXT,
                date     TEXT
            )
        )");
        q.exec("DELETE FROM sqlite_sequence WHERE name='transactions'");
    }

    // ── UI ────────────────────────────────────────────────────────────────

    void setupUI() {
        setWindowTitle("Money Tracker Mahasiswa");
        resize(1280, 800);
        setMinimumSize(960, 640);

        setStyleSheet(R"(
            QMainWindow, QWidget#central { background: #0d0f1a; }
            QLabel { color: #e2e8f0; }
            QComboBox QAbstractItemView {
                background: #1a1d2e;
                color: #e2e8f0;
                border: 1px solid #2d3250;
                selection-background-color: #5a52e0;
                selection-color: #ffffff;
                outline: none;
            }
            QScrollBar:vertical {
                background: #161929;
                width: 5px;
                border-radius: 3px;
            }
            QScrollBar::handle:vertical {
                background: #3d4166;
                border-radius: 3px;
                min-height: 30px;
            }
            QScrollBar::add-line:vertical,
            QScrollBar::sub-line:vertical { height: 0; }
            QToolTip {
                background: #1a1d2e;
                color: #e2e8f0;
                border: 1px solid #2d3250;
                padding: 4px 8px;
                border-radius: 6px;
            }
        )");

        centralWidget = new QWidget();
        centralWidget->setObjectName("central");
        setCentralWidget(centralWidget);

        QHBoxLayout *rootLayout = new QHBoxLayout(centralWidget);
        rootLayout->setContentsMargins(0, 0, 0, 0);
        rootLayout->setSpacing(0);

        // ── SIDEBAR ───────────────────────────────────────────────────────
        QWidget *sidebar = new QWidget();
        sidebar->setFixedWidth(210);
        sidebar->setStyleSheet("background:#080a14;");

        QVBoxLayout *sideLayout = new QVBoxLayout(sidebar);
        sideLayout->setContentsMargins(18, 32, 18, 28);
        sideLayout->setSpacing(0);

        // Brand
        QWidget *logoBlock = new QWidget();
        logoBlock->setFixedSize(52, 52);
        logoBlock->setStyleSheet(R"(
            background: qlineargradient(x1:0,y1:0,x2:1,y2:1,
                stop:0 #4f46e5, stop:1 #7c3aed);
            border-radius: 14px;
        )");
        QVBoxLayout *lbLayout = new QVBoxLayout(logoBlock);
        lbLayout->setContentsMargins(0,0,0,0);
        QLabel *lbText = new QLabel("Rp");
        lbText->setAlignment(Qt::AlignCenter);
        lbText->setStyleSheet("color:white; font-size:18px; font-weight:900; background:transparent;");
        lbLayout->addWidget(lbText);

        QGraphicsDropShadowEffect *logoShadow = new QGraphicsDropShadowEffect();
        logoShadow->setBlurRadius(20);
        logoShadow->setColor(QColor(79, 70, 229, 130));
        logoShadow->setOffset(0, 4);
        logoBlock->setGraphicsEffect(logoShadow);

        QWidget *brandWrap = new QWidget();
        brandWrap->setStyleSheet("background:transparent;");
        QVBoxLayout *brandLayout = new QVBoxLayout(brandWrap);
        brandLayout->setContentsMargins(0,0,0,0);
        brandLayout->setSpacing(8);
        brandLayout->setAlignment(Qt::AlignHCenter);
        brandLayout->addWidget(logoBlock, 0, Qt::AlignHCenter);

        QLabel *appName = new QLabel("Money Tracker");
        appName->setAlignment(Qt::AlignCenter);
        appName->setStyleSheet("font-size:18px; font-weight:800; color:#ffffff; background:transparent;");

        QLabel *appSub = new QLabel("MAHASISWA");
        appSub->setAlignment(Qt::AlignCenter);
        appSub->setStyleSheet("font-size:9px; color:#5a52e0; font-weight:700; letter-spacing:3px; background:transparent; margin-bottom:18px;");

        brandLayout->addWidget(appName);
        brandLayout->addWidget(appSub);

        QFrame *div = makeDivider();

        // Nav items
        QWidget *navDashboard = makeNavItem("Dasbor", true);

        // Tombol Logout
        QPushButton *logoutBtn = new QPushButton("🚪  Logout");
        logoutBtn->setCursor(Qt::PointingHandCursor);
        logoutBtn->setStyleSheet(R"(
            QPushButton {
                background: transparent;
                color: #f43f5e;
                border: 1px solid #3b1a24;
                border-radius: 10px;
                padding: 11px 14px;
                font-size: 13px;
                font-weight: 600;
                text-align: left;
            }
            QPushButton:hover {
                background: #1a0a10;
                border-color: #f43f5e;
            }
        )");
        connect(logoutBtn, &QPushButton::clicked, this, &MainWindow::logoutRequested);

        sideLayout->addWidget(brandWrap);
        sideLayout->addWidget(div);
        sideLayout->addSpacing(8);
        sideLayout->addWidget(navDashboard);
        sideLayout->addStretch();
        sideLayout->addWidget(logoutBtn);
        sideLayout->addSpacing(12);

        rootLayout->addWidget(sidebar);

        // ── MAIN CONTENT ──────────────────────────────────────────────────
        QWidget *content = new QWidget();
        content->setStyleSheet("background:#0d0f1a;");
        QVBoxLayout *contentLayout = new QVBoxLayout(content);
        contentLayout->setContentsMargins(28, 26, 28, 20);
        contentLayout->setSpacing(20);

        // ── TOPBAR ────────────────────────────────────────────────────────
        QHBoxLayout *topBar = new QHBoxLayout();

        QVBoxLayout *titleBlock = new QVBoxLayout();
        titleBlock->setSpacing(3);
        QLabel *pageTitle = new QLabel("Dasbor Keuangan");
        pageTitle->setStyleSheet("font-size:22px; font-weight:800; color:#ffffff;");
        QLabel *pageSub = new QLabel("Kelola pemasukan dan pengeluaran Anda dengan mudah");
        pageSub->setStyleSheet("font-size:12px; color:#3d4470;");
        titleBlock->addWidget(pageTitle);
        titleBlock->addWidget(pageSub);

        QLabel *dateLabel = new QLabel(QDate::currentDate().toString("dddd, dd MMMM yyyy"));
        dateLabel->setStyleSheet(R"(
            font-size: 12px;
            color: #818cf8;
            background: #0f1220;
            border: 1px solid #1e2544;
            border-radius: 10px;
            padding: 8px 16px;
        )");

        topBar->addLayout(titleBlock);
        topBar->addStretch();
        topBar->addWidget(dateLabel, 0, Qt::AlignVCenter);
        contentLayout->addLayout(topBar);

        // ── SUMMARY CARDS ─────────────────────────────────────────────────
        QHBoxLayout *cardsRow = new QHBoxLayout();
        cardsRow->setSpacing(14);

        incomeLabel  = buildSummaryCard("PEMASUKAN",   "↑", "#10b981", "#063828", "#065f46");
        expenseLabel = buildSummaryCard("PENGELUARAN", "↓", "#f43f5e", "#3b0a1a", "#881337");
        balanceLabel = buildSummaryCard("SALDO",       "≈", "#818cf8", "#16184a", "#3730a3");

        cardsRow->addWidget(incomeLabel);
        cardsRow->addWidget(expenseLabel);
        cardsRow->addWidget(balanceLabel);
        contentLayout->addLayout(cardsRow);

        // ── FORM PANEL ────────────────────────────────────────────────────
        QFrame *formPanel = new QFrame();
        formPanel->setStyleSheet(R"(
            QFrame {
                background: #0f1220;
                border-radius: 14px;
                border: 1px solid #1e2544;
            }
        )");

        QVBoxLayout *formOuter = new QVBoxLayout(formPanel);
        formOuter->setContentsMargins(20, 14, 20, 14);
        formOuter->setSpacing(10);

        QHBoxLayout *formHeader = new QHBoxLayout();
        QLabel *formTitle = new QLabel("+ Tambah Transaksi Baru");
        formTitle->setStyleSheet("font-size:13px; font-weight:700; color:#818cf8; background:transparent; border:none;");
        QFrame *formDivH = new QFrame();
        formDivH->setFrameShape(QFrame::HLine);
        formDivH->setStyleSheet("background:#1e2544; border:none; max-height:1px;");
        formHeader->addWidget(formTitle);
        formHeader->addWidget(formDivH, 1);
        formOuter->addLayout(formHeader);

        QHBoxLayout *formRow = new QHBoxLayout();
        formRow->setSpacing(10);

        typeBox = new QComboBox();
        typeBox->addItems({"Pemasukan", "Pengeluaran"});

        categoryBox = new QComboBox();
        categoryBox->addItems({
            "Kiriman Orang Tua","Makanan","Transportasi","Belanja",
            "Investasi","Hiburan","Tagihan","Kesehatan","Pendidikan","Lainnya"
        });

        amountInput = new QLineEdit();
        amountInput->setPlaceholderText("Jumlah (Rp)");

        noteInput = new QLineEdit();
        noteInput->setPlaceholderText("Keterangan / Catatan");

        dateInput = new QDateEdit();
        dateInput->setDate(QDate::currentDate());
        dateInput->setCalendarPopup(true);
        dateInput->setDisplayFormat("dd/MM/yyyy");

        addButton    = new QPushButton("+ Tambah");

        for (QWidget *w : {(QWidget*)typeBox,(QWidget*)categoryBox,
                           (QWidget*)amountInput,(QWidget*)noteInput,
                           (QWidget*)dateInput})
            styleFormInput(w);

        styleActionButton(addButton,
                          "background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #4f46e5,stop:1 #7c3aed);",
                          "background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #6057f5,stop:1 #8b4cfd);");

        formRow->addWidget(typeBox,      1);
        formRow->addWidget(categoryBox,  1);
        formRow->addWidget(amountInput,  1);
        formRow->addWidget(noteInput,    2);
        formRow->addWidget(dateInput,    1);
        formRow->addWidget(addButton,    1);

        formOuter->addLayout(formRow);
        contentLayout->addWidget(formPanel);

        // ── TRANSACTION HISTORY PANEL ─────────────────────────────────────
        QFrame *historyPanel = new QFrame();
        historyPanel->setStyleSheet(R"(
            QFrame {
                background: #0f1220;
                border-radius: 14px;
                border: 1px solid #1e2544;
            }
        )");

        QVBoxLayout *historyOuter = new QVBoxLayout(historyPanel);
        historyOuter->setContentsMargins(20, 14, 20, 14);
        historyOuter->setSpacing(12);

        // Header row: title + search + filter
        QHBoxLayout *historyHeader = new QHBoxLayout();
        QLabel *historyTitle = new QLabel("Riwayat Transaksi");
        historyTitle->setStyleSheet("font-size:13px; font-weight:700; color:#818cf8; background:transparent; border:none;");

        // Search input
        searchInput = new QLineEdit();
        searchInput->setPlaceholderText("🔍  Cari keterangan atau kategori...");
        searchInput->setFixedWidth(260);
        searchInput->setStyleSheet(R"(
            QLineEdit {
                background: #080a14;
                color: #c8d0e7;
                border: 1.5px solid #252865;
                border-radius: 8px;
                padding: 7px 12px;
                font-size: 12px;
                min-height: 32px;
                selection-background-color: #5a52e0;
            }
            QLineEdit:focus { border: 1.5px solid #5a52e0; background: #0b0d1e; }
        )");

        // Filter by type
        filterTypeBox = new QComboBox();
        filterTypeBox->addItems({"Semua Tipe", "Pemasukan", "Pengeluaran"});
        filterTypeBox->setFixedWidth(140);
        filterTypeBox->setStyleSheet(R"(
            QComboBox {
                background: #080a14;
                color: #c8d0e7;
                border: 1.5px solid #252865;
                border-radius: 8px;
                padding: 7px 12px;
                font-size: 12px;
                min-height: 32px;
            }
            QComboBox:focus { border: 1.5px solid #5a52e0; }
            QComboBox::drop-down { border: none; width: 24px; }
            QComboBox::down-arrow { image: none; border: none; }
        )");

        // Hint label (kanan)
        QLabel *hintLabel = new QLabel("Klik baris untuk memilih transaksi");
        hintLabel->setStyleSheet("font-size:11px; color:#3d4470; background:transparent; border:none;");

        historyHeader->addWidget(historyTitle);
        historyHeader->addSpacing(12);
        historyHeader->addWidget(searchInput);
        historyHeader->addSpacing(8);
        historyHeader->addWidget(filterTypeBox);
        historyHeader->addStretch();
        historyHeader->addWidget(hintLabel, 0, Qt::AlignVCenter);
        historyOuter->addLayout(historyHeader);

        // Table
        transactionTable = new QTableWidget();
        transactionTable->setColumnCount(6);
        transactionTable->setHorizontalHeaderLabels({"ID","Tipe","Kategori","Jumlah","Keterangan","Tanggal"});
        transactionTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        transactionTable->setSelectionMode(QAbstractItemView::SingleSelection);
        transactionTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        transactionTable->setShowGrid(false);
        transactionTable->verticalHeader()->setVisible(false);
        transactionTable->horizontalHeader()->setStretchLastSection(true);
        transactionTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
        transactionTable->setColumnWidth(0, 60);
        transactionTable->setColumnWidth(1, 110);
        transactionTable->setColumnWidth(2, 160);
        transactionTable->setColumnWidth(3, 120);
        transactionTable->setColumnWidth(5, 120);
        transactionTable->setMinimumHeight(220);
        transactionTable->setStyleSheet(R"(
            QTableWidget {
                background: transparent;
                border: none;
                color: #c8d0e7;
                font-size: 13px;
                gridline-color: transparent;
                outline: none;
            }
            QTableWidget::item {
                padding: 12px 8px;
                border-bottom: 1px solid #131525;
                background: transparent;
            }
            QTableWidget::item:selected {
                background: #1a1d3a;
                color: #ffffff;
            }
            QHeaderView::section {
                background: #080a14;
                color: #818cf8;
                font-size: 12px;
                font-weight: 700;
                padding: 10px 8px;
                border: none;
                border-bottom: 1px solid #1e2544;
                letter-spacing: 0.5px;
            }
            QHeaderView::section:first { border-radius: 0; }
            QScrollBar:vertical {
                background: #161929;
                width: 5px;
                border-radius: 3px;
            }
            QScrollBar::handle:vertical {
                background: #3d4166;
                border-radius: 3px;
                min-height: 30px;
            }
            QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }
        )");

        // Alternating row hint via delegate approach - use item background
        transactionTable->setAlternatingRowColors(false);

        historyOuter->addWidget(transactionTable);

        // ── BOTTOM ACTION ROW ─────────────────────────────────────────────
        QHBoxLayout *bottomRow = new QHBoxLayout();
        bottomRow->setContentsMargins(0, 4, 0, 0);

        QLabel *selHint = new QLabel("Pilih baris lalu klik tombol hapus");
        selHint->setStyleSheet("font-size:11px; color:#3d4470; background:transparent; border:none;");

        deleteButton = new QPushButton("🗑  Hapus Transaksi");
        deleteButton->setCursor(Qt::PointingHandCursor);
        deleteButton->setEnabled(false);
        deleteButton->setStyleSheet(R"(
            QPushButton {
                background: #1a0a10;
                color: #f43f5e;
                border: 1px solid #3b1a24;
                border-radius: 8px;
                padding: 9px 20px;
                font-size: 13px;
                font-weight: 700;
                min-height: 36px;
                letter-spacing: 0.2px;
            }
            QPushButton:hover {
                background: #2a0f18;
                border-color: #f43f5e;
            }
            QPushButton:pressed { padding-top:10px; padding-bottom:8px; }
            QPushButton:disabled {
                background: #0f1117;
                color: #3d2030;
                border-color: #1e1525;
            }
        )");

        bottomRow->addWidget(selHint, 0, Qt::AlignVCenter);
        bottomRow->addStretch();
        bottomRow->addWidget(deleteButton);
        historyOuter->addLayout(bottomRow);

        contentLayout->addWidget(historyPanel, 1);

        rootLayout->addWidget(content, 1);

        // Connections
        connect(addButton, &QPushButton::clicked, this, &MainWindow::addTransaction);
        connect(searchInput, &QLineEdit::textChanged, this, &MainWindow::filterTransactions);
        connect(filterTypeBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::filterTransactions);
        connect(deleteButton, &QPushButton::clicked, this, &MainWindow::deleteSelectedTransaction);
        connect(transactionTable, &QTableWidget::itemSelectionChanged, this, [this]() {
            deleteButton->setEnabled(transactionTable->currentRow() >= 0);
        });

        // Delete key shortcut
        QShortcut *deleteShortcut = new QShortcut(QKeySequence::Delete, transactionTable);
        connect(deleteShortcut, &QShortcut::activated, this, &MainWindow::deleteSelectedTransaction);
    }

    // ── HELPERS ───────────────────────────────────────────────────────────

    QFrame* makeDivider() {
        QFrame *d = new QFrame();
        d->setFrameShape(QFrame::HLine);
        d->setStyleSheet("background:#131525; border:none; max-height:1px; margin: 8px 0 16px 0;");
        return d;
    }

    QWidget* makeNavItem(const QString &label, bool active) {
        QWidget *nav = new QWidget();
        if (active) {
            nav->setStyleSheet(R"(
                background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                    stop:0 #1e1e3a, stop:1 #16182e);
                border-radius: 10px;
                border-left: 3px solid #5a52e0;
            )");
        } else {
            nav->setStyleSheet("background:transparent; border-radius:10px;");
        }
        QHBoxLayout *navLayout = new QHBoxLayout(nav);
        navLayout->setContentsMargins(12, 11, 12, 11);
        navLayout->setSpacing(10);

        QLabel *navText = new QLabel(label);
        navText->setStyleSheet(active
                                   ? "background:transparent; color:#a5b4fc; font-size:13px; font-weight:600; border:none;"
                                   : "background:transparent; color:#3d4470; font-size:13px; border:none;");

        navLayout->addWidget(navText);
        navLayout->addStretch();
        return nav;
    }

    QLabel* buildSummaryCard(const QString &title, const QString &symbol,
                             const QString &accent, const QString &bg, const QString &border) {
        QLabel *card = new QLabel();
        card->setMinimumHeight(118);
        card->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        card->setTextFormat(Qt::RichText);
        card->setStyleSheet(QString(R"(
            background: %1;
            border-radius: 14px;
            border: 1px solid %2;
            padding: 0 20px;
        )").arg(bg, border));

        QGraphicsDropShadowEffect *sh = new QGraphicsDropShadowEffect();
        sh->setBlurRadius(20);
        sh->setColor(QColor(0,0,0,80));
        sh->setOffset(0,4);
        card->setGraphicsEffect(sh);

        card->setProperty("accent",  accent);
        card->setProperty("symbol",  symbol);
        card->setProperty("title",   title);
        return card;
    }

    void updateCardText(QLabel *card, double amount) {
        QString accent = card->property("accent").toString();
        QString symbol = card->property("symbol").toString();
        QString title  = card->property("title").toString();

        QString formatted = QString("%L1").arg((long long)qAbs(amount));
        QString prefix    = (amount < 0) ? "- Rp " : "Rp ";

        card->setText(QString(
                          "<table cellspacing='0' cellpadding='0'>"
                          "<tr><td>"
                          "  <div style='font-size:34px; font-weight:900; color:%1; line-height:1;'>%2</div>"
                          "</td><td width='16'></td><td>"
                          "  <div style='font-size:10px; font-weight:700; letter-spacing:2px; color:%3; margin-bottom:5px;'>%4</div>"
                          "  <div style='font-size:20px; font-weight:800; color:#ffffff;'>%5%6</div>"
                          "</td></tr>"
                          "</table>"
                          ).arg(accent, symbol, accent, title, prefix, formatted));
    }

    void styleFormInput(QWidget *w) {
        w->setStyleSheet(R"(
            background: #080a14;
            color: #c8d0e7;
            border: 1.5px solid #252865;
            border-radius: 8px;
            padding: 8px 12px;
            min-height: 36px;
            font-size: 13px;
            selection-background-color: #5a52e0;
        )");
    }

    void styleActionButton(QPushButton *btn, const QString &bg, const QString &bgHover) {
        btn->setCursor(Qt::PointingHandCursor);
        btn->setStyleSheet(QString(R"(
            QPushButton {
                %1
                color: white;
                border: none;
                border-radius: 8px;
                padding: 9px 14px;
                font-size: 13px;
                font-weight: 700;
                min-height: 36px;
                letter-spacing: 0.3px;
            }
            QPushButton:hover { %2 }
            QPushButton:pressed { padding-top: 10px; padding-bottom: 8px; }
        )").arg(bg, bgHover));
    }

    // ── TRANSACTIONS ──────────────────────────────────────────────────────

    void addTransaction() {
        if (amountInput->text().isEmpty()) {
            QMessageBox::warning(this, "Kesalahan", "Jumlah tidak boleh kosong!");
            return;
        }
        double amount = amountInput->text().toDouble();
        QSqlQuery q;
        q.prepare(R"(
            INSERT INTO transactions (type,category,amount,note,date)
            VALUES (:type,:category,:amount,:note,:date)
        )");
        q.bindValue(":type",     typeBox->currentText());
        q.bindValue(":category", categoryBox->currentText());
        q.bindValue(":amount",   amount);
        q.bindValue(":note",     noteInput->text());
        q.bindValue(":date",     dateInput->date().toString("yyyy-MM-dd"));
        if (!q.exec()) {
            QMessageBox::critical(this, "Kesalahan Database", q.lastError().text());
            return;
        }
        amountInput->clear();
        noteInput->clear();
        filterTransactions();
        updateSummary();
    }

    void loadTransactions(const QString &search = "", const QString &typeFilter = "Semua Tipe") {
        transactionTable->setRowCount(0);

        QString sql = "SELECT id, type, category, amount, note, date FROM transactions";
        QStringList conditions;

        if (typeFilter != "Semua Tipe")
            conditions << "type = '" + typeFilter + "'";

        if (!search.isEmpty()) {
            QString s = search;
            s.replace("'", "''");
            conditions << "(note LIKE '%" + s + "%' OR category LIKE '%" + s + "%')";
        }

        if (!conditions.isEmpty())
            sql += " WHERE " + conditions.join(" AND ");

        sql += " ORDER BY id DESC";

        QSqlQuery q(sql);
        int row = 0;
        while (q.next()) {
            transactionTable->insertRow(row);

            int    id       = q.value(0).toInt();
            QString type    = q.value(1).toString();
            QString cat     = q.value(2).toString();
            double  amount  = q.value(3).toDouble();
            QString note    = q.value(4).toString();
            QString date    = q.value(5).toString();

            bool isIncome = (type == "Pemasukan");
            QString amountStr = QString("Rp %L1").arg((long long)amount);

            auto makeItem = [](const QString &text, Qt::Alignment align = Qt::AlignCenter) {
                QTableWidgetItem *item = new QTableWidgetItem(text);
                item->setTextAlignment(align | Qt::AlignVCenter);
                return item;
            };

            transactionTable->setItem(row, 0, makeItem(QString::number(id)));
            transactionTable->setItem(row, 2, makeItem(cat));
            transactionTable->setItem(row, 4, makeItem(note, Qt::AlignLeft));
            transactionTable->setItem(row, 5, makeItem(date));

            // Type item — colored
            QTableWidgetItem *typeItem = makeItem(type);
            typeItem->setForeground(QColor(isIncome ? "#22c55e" : "#f43f5e"));
            transactionTable->setItem(row, 1, typeItem);

            // Amount item — colored
            QTableWidgetItem *amtItem = makeItem(amountStr);
            amtItem->setForeground(QColor(isIncome ? "#22c55e" : "#f43f5e"));
            transactionTable->setItem(row, 3, amtItem);

            // Store DB id for deletion
            transactionTable->item(row, 0)->setData(Qt::UserRole, id);

            transactionTable->setRowHeight(row, 48);
            row++;
        }

        updateSummary();
    }

    void filterTransactions() {
        QString search = searchInput->text().trimmed();
        QString typeFilter = filterTypeBox->currentText();
        loadTransactions(search, typeFilter);
    }

    void deleteSelectedTransaction() {
        int selectedRow = transactionTable->currentRow();
        if (selectedRow < 0) return;

        int dbId = transactionTable->item(selectedRow, 0)->data(Qt::UserRole).toInt();
        QString note = transactionTable->item(selectedRow, 4)->text();

        QMessageBox box(this);
        box.setWindowTitle("Hapus Transaksi");
        box.setTextFormat(Qt::RichText);
        box.setText(
            "<b style='color:#f43f5e; font-size:15px;'>🗑 Hapus Transaksi?</b><br><br>"
            "Transaksi: <b style='color:#c8d0e7'>" + note.toHtmlEscaped() + "</b><br>"
                                     "<span style='color:#4f5a8a; font-size:12px;'>Tindakan ini tidak dapat dibatalkan.</span>"
            );
        box.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
        box.setDefaultButton(QMessageBox::Cancel);
        box.setStyleSheet("QMessageBox { background:#0f1220; color:#c8d0e7; }"
                          "QLabel { color:#c8d0e7; }"
                          "QPushButton { background:#4f46e5; color:white; border-radius:8px; padding:8px 20px; font-weight:700; }"
                          "QPushButton:hover { background:#6057f5; }");

        if (box.exec() == QMessageBox::Yes) {
            QSqlQuery q;
            q.prepare("DELETE FROM transactions WHERE id = :id");
            q.bindValue(":id", dbId);
            if (q.exec()) {
                filterTransactions();
            } else {
                QMessageBox::critical(this, "Kesalahan", q.lastError().text());
            }
        }
    }

    void updateSummary() {
        double income = 0, expense = 0;
        QSqlQuery q("SELECT type, amount FROM transactions");
        while (q.next()) {
            double a = q.value(1).toDouble();
            if (q.value(0).toString() == "Pemasukan") income += a;
            else expense += a;
        }
        updateCardText(incomeLabel,  income);
        updateCardText(expenseLabel, expense);
        updateCardText(balanceLabel, income - expense);
    }
};


// ═══════════════════════════════════════════════════════════════════════════════
//  REGISTER WINDOW
// ═══════════════════════════════════════════════════════════════════════════════

class RegisterWindow : public QWidget {
    Q_OBJECT
signals:
    void registerSuccess();
    void goToLogin();

public:
    RegisterWindow(QWidget *parent = nullptr) : QWidget(parent) {
        setWindowTitle("Daftar Akun – Money Tracker");
        resize(1280, 800);
        setMinimumSize(960, 640);
        setStyleSheet("background:#080a14;");
        setupUI();
    }

private:
    QLineEdit   *usernameInput;
    QLineEdit   *passInput;
    QLineEdit   *confirmInput;
    QLabel      *errorLabel;
    QPushButton *eyePass;
    QPushButton *eyeConfirm;

    // ── Helper: buat field password dengan tombol mata (QAction) ─────────
    QLineEdit* makePasswordField(const QString& placeholder, QPushButton*& eyeBtn) {
        QLineEdit *field = new QLineEdit();
        field->setPlaceholderText(placeholder);
        field->setEchoMode(QLineEdit::Password);
        field->setStyleSheet(R"(
            QLineEdit {
                background: #080a14;
                color: #c8d0e7;
                border: 1px solid #252865;
                border-radius: 10px;
                padding: 12px 14px;
                font-size: 13px;
                min-height: 42px;
                selection-background-color: #5a52e0;
            }
            QLineEdit:focus { border: 1.5px solid #5a52e0; background: #0b0d1e; }
        )");

        eyeBtn = new QPushButton("👁");
        eyeBtn->setCheckable(true);
        eyeBtn->setFlat(true);
        eyeBtn->setCursor(Qt::PointingHandCursor);
        eyeBtn->setStyleSheet("color:#4f5a8a; font-size:16px; border:none; background:transparent; padding:0 4px;");

        QWidgetAction *eyeAction = new QWidgetAction(field);
        eyeAction->setDefaultWidget(eyeBtn);
        field->addAction(eyeAction, QLineEdit::TrailingPosition);

        QLineEdit *f = field;
        connect(eyeBtn, &QPushButton::toggled, this, [f, &eyeBtn](bool checked){
            f->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
            eyeBtn->setStyleSheet(checked
                                      ? "color:#c8d0e7; font-size:16px; border:none; background:transparent; padding:0 4px;"
                                      : "color:#4f5a8a; font-size:16px; border:none; background:transparent; padding:0 4px;");
        });

        return field;
    }

    void setupUI() {
        QVBoxLayout *root = new QVBoxLayout(this);
        root->setContentsMargins(0,0,0,0);
        root->setSpacing(0);

        // TOP DECORATION
        QWidget *topDeco = new QWidget();
        topDeco->setFixedHeight(8);
        topDeco->setStyleSheet(R"(
            background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                stop:0 #4f46e5, stop:0.5 #7c3aed, stop:1 #ec4899);
        )");
        root->addWidget(topDeco);

        // CENTER
        QWidget *center = new QWidget();
        center->setStyleSheet("background:transparent;");
        QVBoxLayout *centerLayout = new QVBoxLayout(center);
        centerLayout->setContentsMargins(50, 32, 50, 32);
        centerLayout->setSpacing(0);

        // Logo
        QWidget *logoBlock = new QWidget();
        logoBlock->setFixedSize(72, 72);
        logoBlock->setStyleSheet(R"(
            background: qlineargradient(x1:0,y1:0,x2:1,y2:1,
                stop:0 #4f46e5, stop:1 #7c3aed);
            border-radius: 20px;
        )");
        QVBoxLayout *ll = new QVBoxLayout(logoBlock);
        ll->setContentsMargins(0,0,0,0);
        QLabel *lt = new QLabel("Rp");
        lt->setAlignment(Qt::AlignCenter);
        lt->setStyleSheet("color:white; font-size:24px; font-weight:900; background:transparent;");
        ll->addWidget(lt);

        QGraphicsDropShadowEffect *sh = new QGraphicsDropShadowEffect();
        sh->setBlurRadius(30);
        sh->setColor(QColor(79, 70, 229, 160));
        sh->setOffset(0, 8);
        logoBlock->setGraphicsEffect(sh);

        QWidget *logoWrap = new QWidget();
        logoWrap->setStyleSheet("background:transparent;");
        QHBoxLayout *lw = new QHBoxLayout(logoWrap);
        lw->setContentsMargins(0,0,0,0);
        lw->addStretch(); lw->addWidget(logoBlock); lw->addStretch();
        centerLayout->addWidget(logoWrap);
        centerLayout->addSpacing(20);

        // Title
        QLabel *title = new QLabel("Buat Akun Baru");
        title->setAlignment(Qt::AlignCenter);
        title->setStyleSheet("font-size:24px; font-weight:800; color:#ffffff; background:transparent; letter-spacing:-0.5px;");
        centerLayout->addWidget(title);
        centerLayout->addSpacing(6);

        QLabel *sub = new QLabel("Alpro Gang – Money Tracker");
        sub->setAlignment(Qt::AlignCenter);
        sub->setStyleSheet("font-size:12px; color:#4f5a8a; background:transparent;");
        centerLayout->addWidget(sub);
        centerLayout->addSpacing(28);

        // FORM CARD
        QFrame *card = new QFrame();
        card->setStyleSheet(R"(
            QFrame {
                background: #0f1220;
                border-radius: 18px;
                border: 1px solid #1e2544;
            }
        )");
        QGraphicsDropShadowEffect *cardSh = new QGraphicsDropShadowEffect();
        cardSh->setBlurRadius(40);
        cardSh->setColor(QColor(0,0,0,120));
        cardSh->setOffset(0, 12);
        card->setGraphicsEffect(cardSh);

        QVBoxLayout *cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(28, 28, 28, 28);
        cardLayout->setSpacing(14);

        // Username
        QLabel *uLabel = new QLabel("Username");
        uLabel->setStyleSheet("font-size:12px; font-weight:600; color:#818cf8; background:transparent;");
        usernameInput = new QLineEdit();
        usernameInput->setPlaceholderText("Buat username");
        usernameInput->setStyleSheet(R"(
            QLineEdit {
                background: #080a14;
                color: #c8d0e7;
                border: 1px solid #252865;
                border-radius: 10px;
                padding: 12px 14px;
                font-size: 13px;
                min-height: 42px;
                selection-background-color: #5a52e0;
            }
            QLineEdit:focus { border: 1.5px solid #5a52e0; background: #0b0d1e; }
        )");

        // Password
        QLabel *pLabel = new QLabel("Password");
        pLabel->setStyleSheet("font-size:12px; font-weight:600; color:#818cf8; background:transparent;");
        passInput = makePasswordField("Buat password", eyePass);

        // Ulangi Password
        QLabel *cLabel = new QLabel("Ulangi Password");
        cLabel->setStyleSheet("font-size:12px; font-weight:600; color:#818cf8; background:transparent;");
        confirmInput = makePasswordField("Masukkan ulang password", eyeConfirm);

        // Error
        errorLabel = new QLabel("");
        errorLabel->setAlignment(Qt::AlignCenter);
        errorLabel->setStyleSheet(R"(
            font-size:12px;
            color:#fb7185;
            background:rgba(251,113,133,0.08);
            border:1px solid rgba(251,113,133,0.2);
            border-radius:8px;
            padding:8px 12px;
        )");
        errorLabel->setVisible(false);

        // Daftar Button
        QPushButton *regBtn = new QPushButton("Daftar Sekarang  →");
        regBtn->setCursor(Qt::PointingHandCursor);
        regBtn->setStyleSheet(R"(
            QPushButton {
                background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                    stop:0 #4f46e5, stop:1 #7c3aed);
                color: white; border: none; border-radius: 10px;
                padding: 14px; font-size: 14px; font-weight: 700;
                letter-spacing: 0.3px; min-height: 46px;
            }
            QPushButton:hover {
                background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                    stop:0 #6057f5, stop:1 #8b4cfd);
            }
            QPushButton:pressed { padding-top:15px; padding-bottom:13px; }
        )");

        connect(regBtn, &QPushButton::clicked, this, &RegisterWindow::tryRegister);

        cardLayout->addWidget(uLabel);
        cardLayout->addWidget(usernameInput);
        cardLayout->addWidget(pLabel);
        cardLayout->addWidget(passInput);
        cardLayout->addWidget(cLabel);
        cardLayout->addWidget(confirmInput);
        cardLayout->addSpacing(4);
        cardLayout->addWidget(errorLabel);
        cardLayout->addWidget(regBtn);

        centerLayout->addWidget(card);
        centerLayout->addSpacing(20);

        // Kembali ke login
        QHBoxLayout *backRow = new QHBoxLayout();
        QLabel *backHint = new QLabel("Sudah punya akun?");
        backHint->setStyleSheet("font-size:12px; color:#4f5a8a; background:transparent;");
        QPushButton *loginLink = new QPushButton("Masuk");
        loginLink->setCursor(Qt::PointingHandCursor);
        loginLink->setStyleSheet(R"(
            QPushButton {
                background: transparent; color: #818cf8;
                border: none; font-size: 12px; font-weight: 700;
                padding: 0; text-decoration: underline;
            }
            QPushButton:hover { color: #a5b4fc; }
        )");
        backRow->setAlignment(Qt::AlignCenter);
        backRow->addWidget(backHint);
        backRow->addWidget(loginLink);
        centerLayout->addLayout(backRow);
        centerLayout->addStretch();

        connect(loginLink, &QPushButton::clicked, this, &RegisterWindow::goToLogin);

        root->addWidget(center, 1);

        // BOTTOM BAR
        QWidget *bottomBar = new QWidget();
        bottomBar->setFixedHeight(48);
        bottomBar->setStyleSheet("background:#080a14; border-top: 1px solid #131525;");
        QHBoxLayout *bbLayout = new QHBoxLayout(bottomBar);
        bbLayout->setContentsMargins(20,0,20,0);
        QLabel *footer = new QLabel("© 2026 Alpro Gang · Money Tracker Mahasiswa");
        footer->setStyleSheet("font-size:11px; color:#1e2544; background:transparent;");
        bbLayout->addStretch();
        bbLayout->addWidget(footer);
        bbLayout->addStretch();
        root->addWidget(bottomBar);
    }

    void tryRegister() {
        QString user = usernameInput->text().trimmed();
        QString pass = passInput->text();
        QString conf = confirmInput->text();

        if (user.isEmpty() || pass.isEmpty() || conf.isEmpty()) {
            showError("Semua field wajib diisi!");
            return;
        }
        if (user.length() < 3) {
            showError("Username minimal 3 karakter!");
            return;
        }
        if (pass.length() < 6) {
            showError("Password minimal 6 karakter!");
            return;
        }
        if (pass != conf) {
            showError("Password dan ulangi password tidak cocok!");
            confirmInput->clear();
            confirmInput->setFocus();
            return;
        }

        AccountStore::save(user, pass);

        QMessageBox box(this);
        box.setWindowTitle("Pendaftaran Berhasil!");
        box.setTextFormat(Qt::RichText);
        box.setText(
            "<b style='color:#22c55e; font-size:16px;'>✅ Akun berhasil dibuat!</b><br><br>"
            "Username: <b style='color:#818cf8'>" + user.toHtmlEscaped() + "</b><br><br>"
                                     "<span style='color:#4f5a8a; font-size:12px;'>Silakan login menggunakan username dan password yang Anda daftarkan.</span>"
            );
        box.setStyleSheet("QMessageBox { background:#0f1220; color:#c8d0e7; }"
                          "QLabel { color:#c8d0e7; }"
                          "QPushButton { background:#4f46e5; color:white; border-radius:8px;"
                          "              padding:8px 20px; font-weight:700; }"
                          "QPushButton:hover { background:#6057f5; }");
        box.exec();

        emit registerSuccess();
    }

    void showError(const QString &msg) {
        errorLabel->setText(msg);
        errorLabel->setVisible(true);
    }
};



class App : public QObject {
    Q_OBJECT
public:
    App() {
        // Pertama kali buka: belum ada akun → langsung ke Register
        if (!AccountStore::hasAccount()) {
            showRegister();
        } else {
            showLogin();
        }
    }

private slots:
    void showLogin() {
        if (registerWindow) {
            registerWindow->close();
            registerWindow->deleteLater();
            registerWindow = nullptr;
        }
        loginWindow = new LoginWindow();
        loginWindow->show();
        QObject::connect(loginWindow, &LoginWindow::loginSuccess, this, &App::showMainWindow);
        QObject::connect(loginWindow, &LoginWindow::goToRegister, this, &App::showRegister);
    }

    void showRegister() {
        if (loginWindow) {
            loginWindow->close();
            loginWindow->deleteLater();
            loginWindow = nullptr;
        }

        registerWindow = new RegisterWindow();
        registerWindow->show();
        QObject::connect(registerWindow, &RegisterWindow::registerSuccess, this, &App::showLogin);
        QObject::connect(registerWindow, &RegisterWindow::goToLogin,       this, &App::showLogin);
    }

    void showMainWindow() {
        mainWindow = new MainWindow();
        mainWindow->show();
        QObject::connect(mainWindow, &MainWindow::logoutRequested, this, &App::handleLogout);
        loginWindow->close();
        loginWindow->deleteLater();
        loginWindow = nullptr;
    }

    void handleLogout() {
        mainWindow->close();
        mainWindow->deleteLater();
        mainWindow = nullptr;
        showLogin();
    }

private:
    LoginWindow    *loginWindow    = nullptr;
    RegisterWindow *registerWindow = nullptr;
    MainWindow     *mainWindow     = nullptr;
};

#include "main.moc"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    App controller;
    return app.exec();
}