#include "login.h"
#include <QApplication>

#include"database.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    initializeDatabase();
    Login  login;
    login.show();

    return a.exec();
}
