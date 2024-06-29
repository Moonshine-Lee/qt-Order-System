#include "tablemanager.h"
#include "ui_tablemanager.h"
#include "addtabledialog.h"
#include "edittabledialog.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>

TableManager::TableManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TableManager)
{
    ui->setupUi(this);
    loadTables();
}

TableManager::~TableManager()
{
    delete ui;
}

void TableManager::on_addTableButton_clicked()
{
    AddTableDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        loadTables();
    }
}

void TableManager::on_editTableButton_clicked()
{
    QList<QTableWidgetItem*> selectedItems = ui->tableTableWidget->selectedItems();

    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "警告", "请选择要编辑的餐桌");
        return;
    }

    int row = selectedItems.first()->row();
    int tableId = ui->tableTableWidget->item(row, 0)->text().toInt();

    EditTableDialog dialog(tableId, this);
    if (dialog.exec() == QDialog::Accepted) {
        loadTables();
    }
}

void TableManager::on_deleteTableButton_clicked()
{
    QList<QTableWidgetItem*> selectedItems = ui->tableTableWidget->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "警告", "请选择要删除的餐桌");
        return;
    }

    int row = selectedItems.first()->row();
    int tableId = ui->tableTableWidget->item(row, 0)->text().toInt();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认删除", "你确定要删除这个餐桌吗？",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("DELETE FROM tables WHERE id = :id");
        query.bindValue(":id", tableId);
        if (query.exec()) {
            loadTables();
        } else {
            QMessageBox::critical(this, "错误", "删除餐桌失败");
        }
    }
}

void TableManager::loadTables()
{
    ui->tableTableWidget->clearContents();
    ui->tableTableWidget->setRowCount(0);

    QSqlQuery query("SELECT id, status FROM tables");
    while (query.next()) {
        int row = ui->tableTableWidget->rowCount();
        ui->tableTableWidget->insertRow(row);

        QTableWidgetItem *item0 = new QTableWidgetItem(query.value(0).toString());
        item0->setFlags(item0->flags() & ~Qt::ItemIsEditable);
        ui->tableTableWidget->setItem(row, 0, item0);

        QTableWidgetItem *item1 = new QTableWidgetItem(query.value(1).toString());
        item1->setFlags(item1->flags() & ~Qt::ItemIsEditable);
        ui->tableTableWidget->setItem(row, 1, item1);
    }
}
