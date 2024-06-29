#include "menumanager.h"
#include "ui_menumanager.h"
#include "addmenudialog.h"
#include "editmenudialog.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>

MenuManager::MenuManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MenuManager)

{
    ui->setupUi(this);

    // 连接用户管理按钮
    // 加载用户列表
    loadMenu();
}

MenuManager::~MenuManager()
{
    delete ui;
}

void MenuManager::on_addMenuButton_clicked()
{
    AddMenuDialog dialog(this);
    dialog.exec();
    loadMenu();
}

void MenuManager::on_editMenuButton_clicked()
{
    QList<QTableWidgetItem*> selectedItems = ui->menuTableWidget->selectedItems();

    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "警告", "请选择要编辑的菜品");
        return;
    }

    int row = selectedItems.first()->row();
    int userId = ui->menuTableWidget->item(row, 0)->text().toInt();

    EditMenuDialog dialog(userId, this);
    dialog.exec();
    loadMenu();

}
void MenuManager::on_deleteMenuButton_clicked()
{
    QList<QTableWidgetItem*> selectedItems = ui->menuTableWidget->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "警告", "请选择要删除的菜品");
        return;
    }

    int row = selectedItems.first()->row();
    int menuId = ui->menuTableWidget->item(row, 0)->text().toInt();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认删除", "你确定要删除这个菜品吗？",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("DELETE FROM menu WHERE id = :id");
        query.bindValue(":id", menuId);
        if (query.exec()) {
            loadMenu();
        } else {
            QMessageBox::critical(this, "错误", "删除菜品失败");
        }
    }

}

void MenuManager::loadMenu()
{
    ui->menuTableWidget->clearContents();
    ui->menuTableWidget->setRowCount(0);

    QSqlQuery query("SELECT id,name,price FROM menu");
    while (query.next()) {
        int row = ui->menuTableWidget->rowCount();
        ui->menuTableWidget->insertRow(row);

        // 创建并设置第一个 item 为只读
        QTableWidgetItem *item0 = new QTableWidgetItem(query.value(0).toString());
        item0->setFlags(item0->flags() & ~Qt::ItemIsEditable);
        ui->menuTableWidget->setItem(row, 0, item0);

        // 创建并设置第二个 item 为只读
        QTableWidgetItem *item1 = new QTableWidgetItem(query.value(1).toString());
        item1->setFlags(item1->flags() & ~Qt::ItemIsEditable);
        ui->menuTableWidget->setItem(row, 1, item1);

        // 创建并设置第三个 item 为只读
        QTableWidgetItem *item2 = new QTableWidgetItem(query.value(2).toString());
        item2->setFlags(item2->flags() & ~Qt::ItemIsEditable);
        ui->menuTableWidget->setItem(row, 2, item2);

    }
}
