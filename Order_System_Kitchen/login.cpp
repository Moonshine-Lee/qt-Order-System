#include "login.h"
#include "ui_login.h"
#include"unfinished_show.h"
#include"database.h"
Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    setWindowTitle("点菜系统（后厨端）");
    // 连接数据库
    db=getDatabase();
    ui->Edit_password->setEchoMode(QLineEdit::Password);
    if (!db.open()) {
        QMessageBox::critical(this, "Database Error", db.lastError().text());
    }


}

Login::~Login()
{
    delete ui;
}

void Login::on_btn_login_clicked()
{
    QString username = ui->Edit_username->text();
    QString password = ui->Edit_password->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "用户名或密码不能为空");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT role FROM users WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", query.lastError().text());
        return;
    }

    if (query.next()) {
        QString role = query.value("role").toString();
        if (role == "admin" || role == "waiter")
       {
        QMessageBox::information(this, "登录成功", "欢迎, " + username + "! 你登陆的权限是 " + role + ".");
        unfinished_show* unfinishedShow =new unfinished_show;
        unfinishedShow->show();
        this->close();
        }
        else{
            QMessageBox::warning(this, "登陆失败", "权限不足");
        }

    } else {
        QMessageBox::warning(this, "登陆失败", "用户名或密码错误");
    }


}

void Login::on_btn_exit_clicked()
{
    QApplication::quit();
}

void Login::on_check_password_clicked(bool checked)
{
    if (checked) {
        ui->Edit_password->setEchoMode(QLineEdit::Normal);
    } else {
        ui->Edit_password->setEchoMode(QLineEdit::Password);
    }
}
