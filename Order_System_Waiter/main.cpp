#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QDebug>
#include "login.h"
#include <QTextCodec>
#include <database.h>

// 函数声明
bool initializeDatabase_first();
bool insertInitialData();

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    // 初始化数据库
    initializeDatabase();
    Login login;
    login.show();

    return app.exec();
}

// 函数定义
bool initializeDatabase_first() {

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("restaurant.db");

    if (!db.open()) {
        qDebug() << "Error: Unable to open database:" << db.lastError();
        return false;
    }

    QSqlQuery query;

    // 创建菜单表
    query.exec("CREATE TABLE IF NOT EXISTS menu ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "name TEXT NOT NULL,"
               "price REAL NOT NULL,"
               "description TEXT,"
               "image_path TEXT)");

    // 创建订单表
    query.exec("CREATE TABLE IF NOT EXISTS orders ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "table_id INTEGER NOT NULL UNIQUE,"
               "total_amount REAL DEFAULT 0,"
               "status TEXT NOT NULL DEFAULT '待结账',"
               "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP)");

    // 创建订单-菜品表
    //finished 代表已出餐的数量
    query.exec("CREATE TABLE IF NOT EXISTS order_items ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "order_id INTEGER NOT NULL,"
               "menu_id INTEGER NOT NULL,"
               "quantity INTEGER NOT NULL,"
               "finished INTEGER NOT NULL DEFAULT 0"
               "FOREIGN KEY(order_id) REFERENCES orders(id),"
               "FOREIGN KEY(menu_id) REFERENCES menu(id))");

    // 创建用户表
    query.exec("CREATE TABLE IF NOT EXISTS users ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "username TEXT NOT NULL UNIQUE,"
               "password TEXT NOT NULL,"
               "role TEXT NOT NULL)");

    // 创建日志表
    query.exec("CREATE TABLE IF NOT EXISTS logs ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "user_id INTEGER NOT NULL,"
               "action TEXT NOT NULL,"
               "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,"
               "FOREIGN KEY(user_id) REFERENCES users(id))");

    // 创建桌台表
    query.exec("CREATE TABLE IF NOT EXISTS tables ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "status TEXT NOT NULL)");

    return true;
}

