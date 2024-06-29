#include "orderlists.h"
#include "ui_orderlists.h"
#include <QSqlQuery>
#include<QTimer>
OrderLists::OrderLists(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrderLists),
    timer(new QTimer(this))
{
    ui->setupUi(this);

    updateValue();
    connect(timer, &QTimer::timeout, this, &OrderLists::updateValue);
        timer->start(5000);

}
void OrderLists::updateValue(){
    ui->tableWidget->setRowCount(0);
    QSqlQuery query;
    query.prepare("select id,timestamp,table_id,total_amount,status from orders");
    query.exec();
    while (query.next()) {
           int row = ui->tableWidget->rowCount();
           ui->tableWidget->insertRow(row);

           int Id = query.value(0).toInt();
           QString time = query.value(1).toString();
           int tableId = query.value(2).toInt();
           double totalAmount = query.value(3).toDouble();
           QString status = query.value(4).toString();

           ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(Id)));
           ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(tableId)));
           ui->tableWidget->setItem(row, 2, new QTableWidgetItem(time));
           ui->tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(totalAmount, 'f', 2)));
           ui->tableWidget->setItem(row, 4, new QTableWidgetItem(status));
       }

       // 设置单元格为只读
       for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
           for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
               ui->tableWidget->item(row, col)->setFlags(ui->tableWidget->item(row, col)->flags() & ~Qt::ItemIsEditable);
           }
       }

}

OrderLists::~OrderLists()
{
    delete ui;
}
