#include "edituserdialog.h"
#include "ui_edituserdialog.h"
#include <QSqlQuery>
#include <QMessageBox>
#include<QCloseEvent>
EditUserDialog::EditUserDialog(int userId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditUserDialog),
    m_userId(userId)
{
    ui->setupUi(this);
    ui->comboBoxRole->addItems({"waiter", "admin", "cooker"});

    QSqlQuery query;
    query.prepare("SELECT username, role,password FROM users WHERE id = :id");
    query.bindValue(":id", m_userId);
    query.exec(); query.next();
        ui->lineEditUsername->setText(query.value(0).toString());
        ui->comboBoxRole->setCurrentText(query.value(1).toString());
        ui->lineEditPassword->setText(query.value(2).toString());

}

EditUserDialog::~EditUserDialog()
{
    delete ui;
}

void EditUserDialog::on_buttonBox_accepted()
{
    QString username = ui->lineEditUsername->text();
    QString role = ui->comboBoxRole->currentText();

    if (username.isEmpty()) {
        QMessageBox::warning(this, "警告", "用户名不能为空");
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE users SET username = :username, role = :role WHERE id = :id");
    query.bindValue(":username", username);
    query.bindValue(":role", role);
    query.bindValue(":id", m_userId);

    if (!query.exec()) {
        QMessageBox::critical(this, "错误", "更新用户失败");
    } else {
        accept(); // 确保对话框正常关闭
    }
}

void EditUserDialog::closeEvent(QCloseEvent *event)
{
        event->accept();
}
