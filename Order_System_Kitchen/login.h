#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_btn_login_clicked();
    void on_btn_exit_clicked();
    void on_check_password_clicked(bool checked);

private:
    Ui::Login *ui;
    QSqlDatabase db;
};

#endif // LOGIN_H
