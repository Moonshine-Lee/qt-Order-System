#include "database.h"
#include <QSqlDatabase>
#include <QDebug>

void initializeDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

#ifdef QT_DEBUG
    // Debug 模式下的数据库路径
    db.setDatabaseName("restaurant.db");
    qDebug() << "Using debug database path";
#else
    // Release 模式下的数据库路径
    db.setDatabaseName("restaurant.db");
    qDebug() << "Using release database path";
#endif

    if (!db.open()) {
        qDebug() << "Failed to open the database!";
    } else {
        qDebug() << "Database opened successfully!";
    }
}

QSqlDatabase getDatabase()
{
    return QSqlDatabase::database();
}
