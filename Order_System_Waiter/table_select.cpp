#include "table_select.h"
#include "ui_table_select.h"
#include "order_window.h"
#include <QSqlError>
#include <QSqlQuery>
#include "database.h"
Table_Select::Table_Select(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Table_Select)
{
    ui->setupUi(this);

    // 连接数据库
    db=getDatabase();
    loadTables();

}

Table_Select::~Table_Select()
{


}

void Table_Select::loadTables()
{
    QSqlQuery query;
    query.prepare("SELECT id, status FROM tables");

    if (!query.exec()) {
        QMessageBox::critical(this, "数据库错误", query.lastError().text());
        return;
    }

    int row = 0, col = 0;
    while (query.next()) {
        int tableId = query.value(0).toInt();
        QString status = query.value(1).toString();
        QPushButton *btn = createTableButton(tableId, status);
        ui->gridLayout->addWidget(btn, row, col);
        col++;
        if (col == 4) { // 每行最多显示4个桌台
            col = 0;
            row++;
        }
    }
}

QPushButton* Table_Select::createTableButton(int tableId, const QString &status)
{
    QPushButton *btn = new QPushButton(QString("桌台 %1").arg(tableId), this);
    btn->setFixedSize(100, 100);
    if (status == "occupied") {
        btn->setStyleSheet("background-color: red;");
    } else {
        btn->setStyleSheet("background-color: green;");
    }
    btn->setProperty("tableId", tableId);
    btn->setProperty("status", status);
    connect(btn, &QPushButton::clicked, this, &Table_Select::handleTableButtonClicked);
    return btn;
}

void Table_Select::handleTableButtonClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    int tableId = btn->property("tableId").toInt();
    QString status = btn->property("status").toString();

    // 跳转到 OrderWindow 界面
    bool isAddDish = (status == "occupied");
    OrderWindow *orderWindow = new OrderWindow(tableId, isAddDish);
    orderWindow->show();

    this->close();
}
