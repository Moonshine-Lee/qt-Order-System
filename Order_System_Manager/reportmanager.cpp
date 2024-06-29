#include "reportmanager.h"
#include "ui_reportmanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

ReportManager::ReportManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReportManager)
{
    ui->setupUi(this);
    loadDailySalesReport();
    loadMenuItemSalesReport();
    connect(ui->dailySalesButton, &QPushButton::clicked, this, &ReportManager::generateDailySalesReport);
    connect(ui->menuItemSalesButton, &QPushButton::clicked, this, &ReportManager::generateMenuItemSalesReport);
}

ReportManager::~ReportManager()
{
    delete ui;
}

void ReportManager::generateDailySalesReport()
{
    loadDailySalesReport();
}

void ReportManager::generateMenuItemSalesReport()
{
    loadMenuItemSalesReport();
}

void ReportManager::loadDailySalesReport()
{
    ui->dailySalesTableWidget->clearContents();
    ui->dailySalesTableWidget->setRowCount(0);

    QSqlQuery query;
    query.prepare("SELECT DATE(timestamp) as date, SUM(total_amount) as total_sales FROM orders GROUP BY date");

    if (!query.exec()) {
        QString errorMsg = query.lastError().text();
        QMessageBox::critical(this, "错误", "加载每日销售额报表失败: " + errorMsg);
        return;
    }

    while (query.next()) {
        int row = ui->dailySalesTableWidget->rowCount();
        ui->dailySalesTableWidget->insertRow(row);

        QTableWidgetItem *item0 = new QTableWidgetItem(query.value(0).toString());
        item0->setFlags(item0->flags() & ~Qt::ItemIsEditable);
        ui->dailySalesTableWidget->setItem(row, 0, item0);

        QTableWidgetItem *item1 = new QTableWidgetItem(query.value(1).toString());
        item1->setFlags(item1->flags() & ~Qt::ItemIsEditable);
        ui->dailySalesTableWidget->setItem(row, 1, item1);
    }
}

void ReportManager::loadMenuItemSalesReport()
{
    ui->menuItemSalesTableWidget->clearContents();
    ui->menuItemSalesTableWidget->setRowCount(0);

    QSqlQuery query;
    query.prepare("SELECT menu.name, SUM(order_items.quantity) as total_quantity FROM order_items join menu on menu.id=order_items.menu_id  GROUP BY menu.name");

    if (!query.exec()) {
        QString errorMsg = query.lastError().text();
        QMessageBox::critical(this, "错误", "加载菜品销量报表失败: " + errorMsg);
        return;
    }

    while (query.next()) {
        int row = ui->menuItemSalesTableWidget->rowCount();
        ui->menuItemSalesTableWidget->insertRow(row);

        QTableWidgetItem *item0 = new QTableWidgetItem(query.value(0).toString());
        item0->setFlags(item0->flags() & ~Qt::ItemIsEditable);
        ui->menuItemSalesTableWidget->setItem(row, 0, item0);

        QTableWidgetItem *item1 = new QTableWidgetItem(query.value(1).toString());
        item1->setFlags(item1->flags() & ~Qt::ItemIsEditable);
        ui->menuItemSalesTableWidget->setItem(row, 1, item1);
    }
}
