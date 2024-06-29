#include "addtabledialog.h"
#include "ui_addtabledialog.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>

AddTableDialog::AddTableDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddTableDialog)
{
    ui->setupUi(this);

    ui->comboBox->addItems({"available", "occupied"});
}

AddTableDialog::~AddTableDialog()
{
    delete ui;
}

void AddTableDialog::on_buttonBox_accepted()
{

    QString status = ui->comboBox->currentText();
    bool ok;
    int tableId = ui->lineEditTableId->text().toInt(&ok);
        if (!ok) {
            // 转换失败，处理脏数据的情况
            QMessageBox::warning(nullptr, "输入错误", "请输入一个有效的整数");
            return;
        }
    if (status.isEmpty()) {
        QMessageBox::warning(this, "警告", "状态不能为空");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO tables (status,id) VALUES (:status,:Id)");
    query.bindValue(":status", status);
    query.bindValue(":Id",tableId);

    if (!query.exec()) {
        QString errorMsg = query.lastError().text();
        QMessageBox::critical(this, "错误", "添加餐桌失败: " + errorMsg);
    } else {
        QMessageBox::information(this, "成功", "餐桌添加成功");
        accept();
    }
}
