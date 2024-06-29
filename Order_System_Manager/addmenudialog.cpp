#include "addmenudialog.h"
#include "ui_addmenudialog.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
AddMenuDialog::AddMenuDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddMenuDialog)
{
    ui->setupUi(this);

}

AddMenuDialog::~AddMenuDialog()
{
    delete ui;
}

void AddMenuDialog::on_buttonBox_accepted()
{
    QString menuName = ui->lineEditMenuName->text();
    QString price = ui->lineEditPrice->text();

    if (menuName.isEmpty() || price.isEmpty()) {
        QMessageBox::warning(this, "警告", "菜品名和价格不能为空");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO menu (name, price) VALUES (:name, :price)");
    query.bindValue(":name", menuName);
    query.bindValue(":price", price);

    if (!query.exec()) {
        QString errorMsg = query.lastError().text();
        QMessageBox::critical(this, "错误", "添加菜品失败: " + errorMsg);
    } else {
        QMessageBox::information(this, "成功", "菜品添加成功");
    }
    this->close();

}
