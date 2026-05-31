#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLabel>
#include <QComboBox>
#include <QDateEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QTabWidget>
#include <QChart>
#include <QChartView>
#include <QPieSeries>
#include <QBarSeries>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QList>
#include <QDate>

struct Transaction {
    int id;
    QString type;       // "income" or "expense"
    QString category;
    QString description;
    double amount;
    QDate date;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addTransaction();
    void deleteTransaction();
    void filterTransactions();
    void updateSummary();
    void updateCharts();
    void onTabChanged(int index);
    void clearForm();

private:
    void setupUI();
    void setupMenuBar();
    void setupDashboard(QWidget *tab);
    void setupTransactions(QWidget *tab);
    void setupCharts(QWidget *tab);
    void loadSampleData();
    void refreshTable();
    void applyStyleSheet();

    // UI Elements - Dashboard
    QLabel *lblTotalBalance;
    QLabel *lblTotalIncome;
    QLabel *lblTotalExpense;
    QTableWidget *recentTable;

    // UI Elements - Transactions
    QComboBox *cmbType;
    QComboBox *cmbCategory;
    QLineEdit *txtDescription;
    QLineEdit *txtAmount;
    QDateEdit *dateEdit;
    QTableWidget *transactionTable;
    QComboBox *filterType;
    QComboBox *filterCategory;
    QPushButton *btnAdd;
    QPushButton *btnDelete;
    QPushButton *btnClear;

    // Charts
    QChartView *pieChartView;
    QChartView *barChartView;

    // Data
    QList<Transaction> transactions;
    int nextId = 1;

    QTabWidget *tabWidget;
};

#endif // MAINWINDOW_H
