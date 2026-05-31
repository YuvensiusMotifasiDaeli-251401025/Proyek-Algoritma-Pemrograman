#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QFrame>
#include <QFont>
#include <QRandomGenerator>
#include <QSplitter>
#include <QScrollArea>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("💰 Money Tracker");
    setMinimumSize(1000, 700);
    resize(1200, 800);
    applyStyleSheet();
    setupUI();
    loadSampleData();
    updateSummary();
    updateCharts();
}

MainWindow::~MainWindow() {}

void MainWindow::applyStyleSheet() {
    setStyleSheet(R"(
        QMainWindow {
            background-color: #0f1117;
        }
        QWidget {
            background-color: #0f1117;
            color: #e8eaf6;
            font-family: 'Segoe UI', sans-serif;
            font-size: 13px;
        }
        QTabWidget::pane {
            border: 1px solid #2a2d3e;
            border-radius: 8px;
            background: #0f1117;
        }
        QTabBar::tab {
            background: #1a1d2e;
            color: #9e9e9e;
            padding: 10px 24px;
            border: 1px solid #2a2d3e;
            border-bottom: none;
            border-top-left-radius: 8px;
            border-top-right-radius: 8px;
            margin-right: 2px;
            font-weight: 600;
            font-size: 13px;
        }
        QTabBar::tab:selected {
            background: #1e2235;
            color: #7c83fd;
            border-bottom: 2px solid #7c83fd;
        }
        QTabBar::tab:hover:!selected {
            background: #1e2235;
            color: #e8eaf6;
        }
        QGroupBox {
            background-color: #1a1d2e;
            border: 1px solid #2a2d3e;
            border-radius: 12px;
            margin-top: 10px;
            padding: 14px;
            font-weight: 600;
            font-size: 13px;
        }
        QGroupBox::title {
            color: #7c83fd;
            subcontrol-origin: margin;
            left: 14px;
            padding: 0 6px;
        }
        QPushButton {
            background-color: #7c83fd;
            color: white;
            border: none;
            border-radius: 8px;
            padding: 9px 20px;
            font-weight: 600;
            font-size: 13px;
        }
        QPushButton:hover {
            background-color: #6c72fc;
        }
        QPushButton:pressed {
            background-color: #5a60e8;
        }
        QPushButton#btnDelete {
            background-color: #e53935;
        }
        QPushButton#btnDelete:hover {
            background-color: #c62828;
        }
        QPushButton#btnClear {
            background-color: #37474f;
        }
        QPushButton#btnClear:hover {
            background-color: #455a64;
        }
        QLineEdit, QComboBox, QDateEdit {
            background-color: #1e2235;
            border: 1px solid #2a2d3e;
            border-radius: 8px;
            padding: 8px 12px;
            color: #e8eaf6;
            font-size: 13px;
        }
        QLineEdit:focus, QComboBox:focus, QDateEdit:focus {
            border: 1px solid #7c83fd;
        }
        QComboBox::drop-down {
            border: none;
            width: 24px;
        }
        QComboBox::down-arrow {
            image: none;
            border-left: 5px solid transparent;
            border-right: 5px solid transparent;
            border-top: 6px solid #7c83fd;
            margin-right: 6px;
        }
        QComboBox QAbstractItemView {
            background: #1e2235;
            border: 1px solid #2a2d3e;
            selection-background-color: #7c83fd;
            border-radius: 6px;
        }
        QTableWidget {
            background-color: #1a1d2e;
            border: 1px solid #2a2d3e;
            border-radius: 10px;
            gridline-color: #2a2d3e;
            selection-background-color: #2a2d5e;
        }
        QTableWidget::item {
            padding: 10px 12px;
            border-bottom: 1px solid #2a2d3e;
        }
        QTableWidget::item:selected {
            background-color: #2a2d5e;
            color: #e8eaf6;
        }
        QHeaderView::section {
            background-color: #12152a;
            color: #7c83fd;
            padding: 10px 12px;
            border: none;
            border-bottom: 2px solid #2a2d3e;
            font-weight: 700;
            font-size: 12px;
            text-transform: uppercase;
            letter-spacing: 1px;
        }
        QScrollBar:vertical {
            background: #1a1d2e;
            width: 8px;
            border-radius: 4px;
        }
        QScrollBar::handle:vertical {
            background: #2a2d3e;
            border-radius: 4px;
            min-height: 30px;
        }
        QScrollBar::handle:vertical:hover {
            background: #7c83fd;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0;
        }
        QLabel#cardBalance {
            font-size: 32px;
            font-weight: 700;
            color: #ffffff;
        }
        QLabel#cardIncome {
            font-size: 26px;
            font-weight: 700;
            color: #4caf50;
        }
        QLabel#cardExpense {
            font-size: 26px;
            font-weight: 700;
            color: #f44336;
        }
        QDateEdit::drop-down {
            border: none;
            width: 24px;
        }
        QDateEdit::down-arrow {
            border-left: 5px solid transparent;
            border-right: 5px solid transparent;
            border-top: 6px solid #7c83fd;
        }
    )");
}

void MainWindow::setupUI() {
    tabWidget = new QTabWidget(this);
    connect(tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);

    QWidget *dashTab = new QWidget();
    QWidget *transTab = new QWidget();
    QWidget *chartTab = new QWidget();

    setupDashboard(dashTab);
    setupTransactions(transTab);
    setupCharts(chartTab);

    tabWidget->addTab(dashTab, "📊  Dashboard");
    tabWidget->addTab(transTab, "💳  Transaksi");
    tabWidget->addTab(chartTab, "📈  Grafik");

    QWidget *central = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->addWidget(tabWidget);

    setCentralWidget(central);
}

QFrame* makeCard(const QString &bgColor) {
    QFrame *card = new QFrame();
    card->setStyleSheet(QString("QFrame { background: %1; border-radius: 14px; border: 1px solid #2a2d3e; }").arg(bgColor));
    card->setMinimumHeight(120);
    return card;
}

void MainWindow::setupDashboard(QWidget *tab) {
    QVBoxLayout *mainLayout = new QVBoxLayout(tab);
    mainLayout->setSpacing(16);
    mainLayout->setContentsMargins(16, 16, 16, 16);

    // Header
    QLabel *header = new QLabel("Ringkasan Keuangan");
    header->setStyleSheet("font-size: 22px; font-weight: 700; color: #e8eaf6; margin-bottom: 4px;");
    mainLayout->addWidget(header);

    QLabel *subheader = new QLabel("Pantau pemasukan dan pengeluaran Anda");
    subheader->setStyleSheet("font-size: 13px; color: #616161; margin-bottom: 8px;");
    mainLayout->addWidget(subheader);

    // Summary cards row
    QHBoxLayout *cardsRow = new QHBoxLayout();
    cardsRow->setSpacing(14);

    // Balance card
    QFrame *balanceCard = makeCard("#1a2035");
    QVBoxLayout *balLayout = new QVBoxLayout(balanceCard);
    QLabel *balTitle = new QLabel("💼  Total Saldo");
    balTitle->setStyleSheet("color: #9e9e9e; font-size: 12px; font-weight: 600; letter-spacing: 1px;");
    lblTotalBalance = new QLabel("Rp 0");
    lblTotalBalance->setObjectName("cardBalance");
    balLayout->addWidget(balTitle);
    balLayout->addWidget(lblTotalBalance);
    balLayout->addStretch();

    // Income card
    QFrame *incomeCard = makeCard("#1a2a1a");
    QVBoxLayout *incLayout = new QVBoxLayout(incomeCard);
    QLabel *incTitle = new QLabel("📈  Total Pemasukan");
    incTitle->setStyleSheet("color: #9e9e9e; font-size: 12px; font-weight: 600; letter-spacing: 1px;");
    lblTotalIncome = new QLabel("Rp 0");
    lblTotalIncome->setObjectName("cardIncome");
    incLayout->addWidget(incTitle);
    incLayout->addWidget(lblTotalIncome);
    incLayout->addStretch();

    // Expense card
    QFrame *expenseCard = makeCard("#2a1a1a");
    QVBoxLayout *expLayout = new QVBoxLayout(expenseCard);
    QLabel *expTitle = new QLabel("📉  Total Pengeluaran");
    expTitle->setStyleSheet("color: #9e9e9e; font-size: 12px; font-weight: 600; letter-spacing: 1px;");
    lblTotalExpense = new QLabel("Rp 0");
    lblTotalExpense->setObjectName("cardExpense");
    expLayout->addWidget(expTitle);
    expLayout->addWidget(lblTotalExpense);
    expLayout->addStretch();

    cardsRow->addWidget(balanceCard);
    cardsRow->addWidget(incomeCard);
    cardsRow->addWidget(expenseCard);
    mainLayout->addLayout(cardsRow);

    // Recent transactions
    QGroupBox *recentGroup = new QGroupBox("Transaksi Terbaru");
    QVBoxLayout *recLayout = new QVBoxLayout(recentGroup);

    recentTable = new QTableWidget(0, 5);
    recentTable->setHorizontalHeaderLabels({"Tanggal", "Tipe", "Kategori", "Deskripsi", "Jumlah"});
    recentTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    recentTable->verticalHeader()->setVisible(false);
    recentTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    recentTable->setAlternatingRowColors(true);
    recentTable->setStyleSheet(recentTable->styleSheet() + "QTableWidget { alternate-background-color: #1e2235; }");
    recentTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    recentTable->setShowGrid(false);
    recLayout->addWidget(recentTable);

    mainLayout->addWidget(recentGroup);
}

void MainWindow::setupTransactions(QWidget *tab) {
    QHBoxLayout *mainLayout = new QHBoxLayout(tab);
    mainLayout->setSpacing(16);
    mainLayout->setContentsMargins(16, 16, 16, 16);

    // --- Form Panel (left) ---
    QGroupBox *formGroup = new QGroupBox("Tambah Transaksi");
    formGroup->setMaximumWidth(320);
    QVBoxLayout *formLayout = new QVBoxLayout(formGroup);
    formLayout->setSpacing(12);

    auto makeLabel = [](const QString &text) {
        QLabel *l = new QLabel(text);
        l->setStyleSheet("color: #9e9e9e; font-size: 12px; font-weight: 600;");
        return l;
    };

    formLayout->addWidget(makeLabel("TIPE TRANSAKSI"));
    cmbType = new QComboBox();
    cmbType->addItem("📈 Pemasukan", "income");
    cmbType->addItem("📉 Pengeluaran", "expense");
    connect(cmbType, &QComboBox::currentIndexChanged, [this](int) {
        cmbCategory->clear();
        if (cmbType->currentData() == "income") {
            cmbCategory->addItems({"Gaji", "Freelance", "Investasi", "Bonus", "Lainnya"});
        } else {
            cmbCategory->addItems({"Makanan", "Transportasi", "Hiburan", "Kesehatan", "Belanja", "Tagihan", "Pendidikan", "Lainnya"});
        }
    });
    formLayout->addWidget(cmbType);

    formLayout->addWidget(makeLabel("KATEGORI"));
    cmbCategory = new QComboBox();
    cmbCategory->addItems({"Gaji", "Freelance", "Investasi", "Bonus", "Lainnya"});
    formLayout->addWidget(cmbCategory);

    formLayout->addWidget(makeLabel("DESKRIPSI"));
    txtDescription = new QLineEdit();
    txtDescription->setPlaceholderText("Contoh: Gaji Bulan Mei...");
    formLayout->addWidget(txtDescription);

    formLayout->addWidget(makeLabel("JUMLAH (Rp)"));
    txtAmount = new QLineEdit();
    txtAmount->setPlaceholderText("Contoh: 5000000");
    txtAmount->setValidator(new QDoubleValidator(0, 999999999, 2, this));
    formLayout->addWidget(txtAmount);

    formLayout->addWidget(makeLabel("TANGGAL"));
    dateEdit = new QDateEdit(QDate::currentDate());
    dateEdit->setCalendarPopup(true);
    dateEdit->setDisplayFormat("dd MMMM yyyy");
    formLayout->addWidget(dateEdit);

    formLayout->addStretch();

    QHBoxLayout *btnRow = new QHBoxLayout();
    btnAdd = new QPushButton("➕ Tambah");
    btnClear = new QPushButton("🗑 Reset");
    btnClear->setObjectName("btnClear");
    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::addTransaction);
    connect(btnClear, &QPushButton::clicked, this, &MainWindow::clearForm);
    btnRow->addWidget(btnAdd);
    btnRow->addWidget(btnClear);
    formLayout->addLayout(btnRow);

    mainLayout->addWidget(formGroup);

    // --- Transaction List (right) ---
    QGroupBox *listGroup = new QGroupBox("Daftar Transaksi");
    QVBoxLayout *listLayout = new QVBoxLayout(listGroup);

    // Filter bar
    QHBoxLayout *filterRow = new QHBoxLayout();
    filterRow->addWidget(new QLabel("Filter:"));
    filterType = new QComboBox();
    filterType->addItems({"Semua", "Pemasukan", "Pengeluaran"});
    filterType->setMaximumWidth(140);
    filterCategory = new QComboBox();
    filterCategory->addItems({"Semua Kategori", "Gaji", "Freelance", "Investasi", "Bonus",
                               "Makanan", "Transportasi", "Hiburan", "Kesehatan", "Belanja", "Tagihan", "Pendidikan", "Lainnya"});
    filterCategory->setMaximumWidth(180);
    connect(filterType, &QComboBox::currentIndexChanged, this, &MainWindow::filterTransactions);
    connect(filterCategory, &QComboBox::currentIndexChanged, this, &MainWindow::filterTransactions);
    filterRow->addWidget(filterType);
    filterRow->addWidget(filterCategory);
    filterRow->addStretch();
    btnDelete = new QPushButton("🗑 Hapus");
    btnDelete->setObjectName("btnDelete");
    btnDelete->setMaximumWidth(100);
    connect(btnDelete, &QPushButton::clicked, this, &MainWindow::deleteTransaction);
    filterRow->addWidget(btnDelete);
    listLayout->addLayout(filterRow);

    transactionTable = new QTableWidget(0, 6);
    transactionTable->setHorizontalHeaderLabels({"ID", "Tanggal", "Tipe", "Kategori", "Deskripsi", "Jumlah"});
    transactionTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    transactionTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    transactionTable->setColumnWidth(0, 50);
    transactionTable->verticalHeader()->setVisible(false);
    transactionTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    transactionTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    transactionTable->setAlternatingRowColors(true);
    transactionTable->setStyleSheet(transactionTable->styleSheet() + "QTableWidget { alternate-background-color: #1e2235; }");
    transactionTable->setShowGrid(false);
    listLayout->addWidget(transactionTable);

    mainLayout->addWidget(listGroup);
}

void MainWindow::setupCharts(QWidget *tab) {
    QVBoxLayout *mainLayout = new QVBoxLayout(tab);
    mainLayout->setSpacing(16);
    mainLayout->setContentsMargins(16, 16, 16, 16);

    QLabel *title = new QLabel("Visualisasi Keuangan");
    title->setStyleSheet("font-size: 20px; font-weight: 700; color: #e8eaf6;");
    mainLayout->addWidget(title);

    QHBoxLayout *chartsRow = new QHBoxLayout();
    chartsRow->setSpacing(16);

    // Pie chart
    QGroupBox *pieGroup = new QGroupBox("Pengeluaran per Kategori");
    QVBoxLayout *pieLayout = new QVBoxLayout(pieGroup);
    pieChartView = new QChartView();
    pieChartView->setRenderHint(QPainter::Antialiasing);
    pieChartView->setMinimumHeight(360);
    pieLayout->addWidget(pieChartView);
    chartsRow->addWidget(pieGroup);

    // Bar chart
    QGroupBox *barGroup = new QGroupBox("Pemasukan vs Pengeluaran per Bulan");
    QVBoxLayout *barLayout = new QVBoxLayout(barGroup);
    barChartView = new QChartView();
    barChartView->setRenderHint(QPainter::Antialiasing);
    barChartView->setMinimumHeight(360);
    barLayout->addWidget(barChartView);
    chartsRow->addWidget(barGroup);

    mainLayout->addLayout(chartsRow);
}

void MainWindow::loadSampleData() {
    QList<Transaction> samples = {

    };

    for (auto &t : samples) transactions.append(t);
    refreshTable();
}

void MainWindow::refreshTable() {
    transactionTable->setRowCount(0);

    QString typeFilter = filterType ? filterType->currentText() : "Semua";
    QString catFilter = filterCategory ? filterCategory->currentText() : "Semua Kategori";

    for (const auto &t : transactions) {
        if (typeFilter == "Pemasukan" && t.type != "income") continue;
        if (typeFilter == "Pengeluaran" && t.type != "expense") continue;
        if (catFilter != "Semua Kategori" && t.category != catFilter) continue;

        int row = transactionTable->rowCount();
        transactionTable->insertRow(row);

        auto makeItem = [](const QString &text, Qt::AlignmentFlag align = Qt::AlignLeft) {
            QTableWidgetItem *item = new QTableWidgetItem(text);
            item->setTextAlignment(align | Qt::AlignVCenter);
            return item;
        };

        transactionTable->setItem(row, 0, makeItem(QString::number(t.id), Qt::AlignCenter));
        transactionTable->setItem(row, 1, makeItem(t.date.toString("dd MMM yyyy")));

        QTableWidgetItem *typeItem;
        if (t.type == "income") {
            typeItem = makeItem("📈 Pemasukan");
            typeItem->setForeground(QColor("#4caf50"));
        } else {
            typeItem = makeItem("📉 Pengeluaran");
            typeItem->setForeground(QColor("#f44336"));
        }
        transactionTable->setItem(row, 2, typeItem);
        transactionTable->setItem(row, 3, makeItem(t.category));
        transactionTable->setItem(row, 4, makeItem(t.description));

        QString amountStr = (t.type == "income" ? "+" : "-") +
                            QString("Rp %L1").arg((long long)t.amount);
        QTableWidgetItem *amtItem = makeItem(amountStr, Qt::AlignRight);
        amtItem->setForeground(t.type == "income" ? QColor("#4caf50") : QColor("#f44336"));
        amtItem->setData(Qt::UserRole, t.id);
        transactionTable->setItem(row, 5, amtItem);

        transactionTable->setRowHeight(row, 42);
    }
}

void MainWindow::updateSummary() {
    double totalIncome = 0, totalExpense = 0;
    int count = qMin(transactions.size(), 8);

    for (const auto &t : transactions) {
        if (t.type == "income") totalIncome += t.amount;
        else totalExpense += t.amount;
    }

    double balance = totalIncome - totalExpense;

    lblTotalBalance->setText(QString("Rp %L1").arg((long long)balance));
    lblTotalIncome->setText(QString("Rp %L1").arg((long long)totalIncome));
    lblTotalExpense->setText(QString("Rp %L1").arg((long long)totalExpense));

    // Fill recent table (last 8)
    recentTable->setRowCount(0);
    int start = qMax(0, transactions.size() - 8);
    for (int i = transactions.size()-1; i >= start; i--) {
        const auto &t = transactions[i];
        int row = recentTable->rowCount();
        recentTable->insertRow(row);

        recentTable->setItem(row, 0, new QTableWidgetItem(t.date.toString("dd MMM yyyy")));

        QTableWidgetItem *typeItem;
        if (t.type == "income") {
            typeItem = new QTableWidgetItem("📈 Pemasukan");
            typeItem->setForeground(QColor("#4caf50"));
        } else {
            typeItem = new QTableWidgetItem("📉 Pengeluaran");
            typeItem->setForeground(QColor("#f44336"));
        }
        recentTable->setItem(row, 1, typeItem);
        recentTable->setItem(row, 2, new QTableWidgetItem(t.category));
        recentTable->setItem(row, 3, new QTableWidgetItem(t.description));

        QString amtStr = (t.type == "income" ? "+" : "-") + QString("Rp %L1").arg((long long)t.amount);
        QTableWidgetItem *amtItem = new QTableWidgetItem(amtStr);
        amtItem->setForeground(t.type == "income" ? QColor("#4caf50") : QColor("#f44336"));
        amtItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        recentTable->setItem(row, 4, amtItem);
        recentTable->setRowHeight(row, 40);
    }
}

void MainWindow::updateCharts() {
    // --- PIE CHART: expense by category ---
    QMap<QString, double> catMap;
    for (const auto &t : transactions) {
        if (t.type == "expense") {
            catMap[t.category] += t.amount;
        }
    }

    QPieSeries *pieSeries = new QPieSeries();
    QList<QColor> colors = {
        QColor("#7c83fd"), QColor("#f44336"), QColor("#4caf50"),
        QColor("#ff9800"), QColor("#00bcd4"), QColor("#e91e63"),
        QColor("#8bc34a"), QColor("#9c27b0")
    };
    int ci = 0;
    for (auto it = catMap.begin(); it != catMap.end(); ++it) {
        QPieSlice *slice = pieSeries->append(it.key(), it.value());
        slice->setColor(colors[ci % colors.size()]);
        slice->setLabelVisible(true);
        slice->setLabel(QString("%1\n%2%").arg(it.key()).arg(
            QString::number(it.value() / catMap.values().constFirst() * 100 , 'f', 0)));
        ci++;
    }

    QChart *pieChart = new QChart();
    pieChart->addSeries(pieSeries);
    pieChart->setTitle("");
    pieChart->legend()->setVisible(true);
    pieChart->legend()->setAlignment(Qt::AlignBottom);
    pieChart->setBackgroundBrush(QBrush(QColor("#1a1d2e")));
    pieChart->setTitleBrush(QBrush(QColor("#e8eaf6")));
    pieChart->legend()->setLabelColor(QColor("#e8eaf6"));
    pieChart->setAnimationOptions(QChart::AllAnimations);
    pieChartView->setChart(pieChart);
    pieChartView->setBackgroundBrush(QBrush(QColor("#1a1d2e")));

    // --- BAR CHART: income vs expense by month ---
    QMap<int, double> monthIncome, monthExpense;
    for (const auto &t : transactions) {
        int m = t.date.month();
        if (t.type == "income") monthIncome[m] += t.amount;
        else monthExpense[m] += t.amount;
    }

    QBarSet *incSet = new QBarSet("Pemasukan");
    QBarSet *expSet = new QBarSet("Pengeluaran");
    incSet->setColor(QColor("#4caf50"));
    expSet->setColor(QColor("#f44336"));

    QStringList months;
    QSet<int> allMonths;
    for (auto k : monthIncome.keys()) allMonths.insert(k);
    for (auto k : monthExpense.keys()) allMonths.insert(k);
    QList<int> sortedMonths = allMonths.values();
    std::sort(sortedMonths.begin(), sortedMonths.end());

    QStringList monthNames = {"Jan","Feb","Mar","Apr","Mei","Jun","Jul","Agu","Sep","Okt","Nov","Des"};
    for (int m : sortedMonths) {
        *incSet << monthIncome.value(m, 0);
        *expSet << monthExpense.value(m, 0);
        months << monthNames[m-1];
    }

    QBarSeries *barSeries = new QBarSeries();
    barSeries->append(incSet);
    barSeries->append(expSet);

    QChart *barChart = new QChart();
    barChart->addSeries(barSeries);
    barChart->setTitle("");
    barChart->setBackgroundBrush(QBrush(QColor("#1a1d2e")));
    barChart->legend()->setVisible(true);
    barChart->legend()->setAlignment(Qt::AlignBottom);
    barChart->legend()->setLabelColor(QColor("#e8eaf6"));
    barChart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(months);
    axisX->setLabelsColor(QColor("#9e9e9e"));
    axisX->setGridLineColor(QColor("#2a2d3e"));
    barChart->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setLabelsColor(QColor("#9e9e9e"));
    axisY->setGridLineColor(QColor("#2a2d3e"));
    barChart->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);

    barChartView->setChart(barChart);
    barChartView->setBackgroundBrush(QBrush(QColor("#1a1d2e")));
}

void MainWindow::addTransaction() {
    if (txtAmount->text().isEmpty() || txtDescription->text().isEmpty()) {
        QMessageBox::warning(this, "Input Tidak Lengkap", "Harap isi semua field sebelum menambah transaksi.");
        return;
    }

    Transaction t;
    t.id = nextId++;
    t.type = cmbType->currentData().toString();
    t.category = cmbCategory->currentText();
    t.description = txtDescription->text();
    t.amount = txtAmount->text().toDouble();
    t.date = dateEdit->date();

    transactions.append(t);
    refreshTable();
    updateSummary();
    clearForm();
}

void MainWindow::deleteTransaction() {
    int row = transactionTable->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Pilih Transaksi", "Pilih transaksi yang ingin dihapus.");
        return;
    }

    int id = transactionTable->item(row, 5)->data(Qt::UserRole).toInt();
    for (int i = 0; i < transactions.size(); i++) {
        if (transactions[i].id == id) {
            transactions.removeAt(i);
            break;
        }
    }

    refreshTable();
    updateSummary();
}

void MainWindow::filterTransactions() {
    refreshTable();
}

void MainWindow::clearForm() {
    txtDescription->clear();
    txtAmount->clear();
    cmbType->setCurrentIndex(0);
    dateEdit->setDate(QDate::currentDate());
}

void MainWindow::onTabChanged(int index) {
    if (index == 0) updateSummary();
    if (index == 2) updateCharts();
}
