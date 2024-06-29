#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QWidget>

namespace Ui {
class UserManager;
}

class UserManager : public QWidget
{
    Q_OBJECT

public:
    explicit UserManager(QWidget *parent = nullptr);
    ~UserManager();

private:
    Ui::UserManager *ui;

private slots:
    void on_addUserButton_clicked();
    void on_editUserButton_clicked();
    void on_deleteUserButton_clicked();
    void loadUsers();
};

#endif // USERMANAGER_H
