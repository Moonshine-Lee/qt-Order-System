#include "usermanager.h"
#include "ui_usermanager.h"
#include "adduserdialog.h"
#include "edituserdialog.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>

UserManager::UserManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserManager)
{
    ui->setupUi(this);

    // 连接用户管理按钮
    // 加载用户列表
    loadUsers();
}

UserManager::~UserManager()
{
    delete ui;
}

void UserManager::on_addUserButton_clicked()
{
    AddUserDialog dialog(this);
    dialog.exec();
    loadUsers();
}

void UserManager::on_editUserButton_clicked()
{
    QList<QTableWidgetItem*> selectedItems = ui->userTableWidget->selectedItems();

    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "警告", "请选择要编辑的用户");
        return;
    }

    int row = selectedItems.first()->row();
    int userId = ui->userTableWidget->item(row, 0)->text().toInt();

    EditUserDialog dialog(userId, this);
    dialog.exec();
    loadUsers();

}
void UserManager::on_deleteUserButton_clicked()
{
    QList<QTableWidgetItem*> selectedItems = ui->userTableWidget->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "警告", "请选择要删除的用户");
        return;
    }

    int row = selectedItems.first()->row();
    int userId = ui->userTableWidget->item(row, 0)->text().toInt();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认删除", "你确定要删除这个用户吗？",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("DELETE FROM users WHERE id = :id");
        query.bindValue(":id", userId);
        if (query.exec()) {
            loadUsers();
        } else {
            QMessageBox::critical(this, "错误", "删除用户失败");
        }
    }

}

void UserManager::loadUsers()
{
    ui->userTableWidget->clearContents();
    ui->userTableWidget->setRowCount(0);

    QSqlQuery query("SELECT id,username,password,role FROM users");
    while (query.next()) {
        int row = ui->userTableWidget->rowCount();
        ui->userTableWidget->insertRow(row);

        // 创建并设置第一个 item 为只读
        QTableWidgetItem *item0 = new QTableWidgetItem(query.value(0).toString());
        item0->setFlags(item0->flags() & ~Qt::ItemIsEditable);
        ui->userTableWidget->setItem(row, 0, item0);

        // 创建并设置第二个 item 为只读
        QTableWidgetItem *item1 = new QTableWidgetItem(query.value(1).toString());
        item1->setFlags(item1->flags() & ~Qt::ItemIsEditable);
        ui->userTableWidget->setItem(row, 1, item1);

        // 创建并设置第三个 item 为只读
        QTableWidgetItem *item2 = new QTableWidgetItem(query.value(2).toString());
        item2->setFlags(item2->flags() & ~Qt::ItemIsEditable);
        ui->userTableWidget->setItem(row, 2, item2);

        // 创建并设置第四个 item 为只读
        QTableWidgetItem *item3 = new QTableWidgetItem(query.value(3).toString());
        item3->setFlags(item3->flags() & ~Qt::ItemIsEditable);
        ui->userTableWidget->setItem(row, 3, item3);

    }
}
