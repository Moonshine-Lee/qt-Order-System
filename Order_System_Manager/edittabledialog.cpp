#include "edittabledialog.h"
#include "ui_edittabledialog.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>

EditTableDialog::EditTableDialog(int tableId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditTableDialog),
    m_tableId(tableId)
{
    ui->setupUi(this);
    ui->comboBox->addItems({"available","occupied"});
    QSqlQuery query;
    query.prepare("SELECT status FROM tables WHERE id = :id");
    query.bindValue(":id", m_tableId);
    if (query.exec() && query.next()) {
        ui->comboBox->setCurrentText(query.value(0).toString());
    }

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &EditTableDialog::on_buttonBox_accepted);
}

EditTableDialog::~EditTableDialog()
{
    delete ui;
}

void EditTableDialog::on_buttonBox_accepted()
{
    QString status = ui->comboBox->currentText();

    if (status.isEmpty()) {
        QMessageBox::warning(this, "警告", "状态不能为空");
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE tables SET status = :status WHERE id = :id");
    query.bindValue(":status", status);
    query.bindValue(":id", m_tableId);

    if (!query.exec()) {
        QString errorMsg = query.lastError().text();
        QMessageBox::critical(this, "错误", "更新餐桌失败: " + errorMsg);
    } else {
        QMessageBox::information(this, "成功", "餐桌更新成功");
        accept();
    }
}

