#include "unfinished_show.h"
#include "ui_unfinished_show.h"
#include <QSpinBox>
#include<QTimer>
#include"database.h"

unfinished_show::unfinished_show(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::unfinished_show),
    timer(new QTimer(this))
{
    ui->setupUi(this);

    setWindowTitle("待完成菜品");
    // 连接数据库

    db=getDatabase();

    ui->unfinished_table->setColumnHidden(0, true);
    loadUnfinishedOrders();
    connect(timer, &QTimer::timeout, this, &unfinished_show::loadUnfinishedOrders);
        timer->start(5000);

    // 初始化 QTimer
}

unfinished_show::~unfinished_show()
{
    delete ui;
}

void unfinished_show:: loadUnfinishedOrders(){

    ui->unfinished_table->setRowCount(0);
    //查询待完成的订单
    QSqlQuery selectUnfinishedQuery("unfinished_show_connection");
    selectUnfinishedQuery.prepare("SELECT menu.name, order_items.quantity, order_items.finished, order_items.id, orders.table_id "
                                  "FROM menu "
                                  "JOIN order_items ON menu.id = order_items.menu_id "
                                  "JOIN orders ON orders.id = order_items.order_id "
                                  "WHERE order_items.quantity != order_items.finished");
    selectUnfinishedQuery.exec();
    while(selectUnfinishedQuery.next()){
        QString name= selectUnfinishedQuery.value(0).toString();
        int total=selectUnfinishedQuery.value(1).toInt();
        int finished=selectUnfinishedQuery.value(2).toInt();
        int order_items_id=selectUnfinishedQuery.value(3).toInt();
        ;
        int tableId=selectUnfinishedQuery.value(4).toInt();
        int row = ui->unfinished_table->rowCount();
        ui->unfinished_table->insertRow(row);


        QTableWidgetItem *Id = new QTableWidgetItem(QString::number(order_items_id));
        Id->setFlags(Id->flags() & ~Qt::ItemIsEditable);  // 设置为只读 flags是一个多位的布尔类型标志串 按位求交集

        QTableWidgetItem *menuName = new QTableWidgetItem(name);
        menuName->setFlags(menuName->flags() & ~Qt::ItemIsEditable);

        QTableWidgetItem *table= new QTableWidgetItem(QString::number(tableId));
        table->setFlags(table->flags() & ~Qt::ItemIsEditable);

        QTableWidgetItem *needAmount = new QTableWidgetItem(QString::number(total - finished));
        needAmount->setFlags(needAmount->flags() & ~Qt::ItemIsEditable);


        QPushButton *btnFinish = new QPushButton("出餐", this);
        connect(btnFinish, &QPushButton::clicked, this, &unfinished_show::handleFinish);
        //把需要的控件绑定到列表上

        ui->unfinished_table->setItem(row, 0, Id);
        ui->unfinished_table->setItem(row, 1, menuName);
        ui->unfinished_table->setItem(row, 2,  needAmount);
        ui->unfinished_table->setItem(row, 3, table);
        ui->unfinished_table->setCellWidget(row, 4, btnFinish);


    }



}
void unfinished_show::handleFinish(){
    QPushButton *btn=qobject_cast<QPushButton*>(sender());

    int row = ui->unfinished_table->indexAt(btn->pos()).row();

    int orderItemId=ui->unfinished_table->item(row,0)->text().toInt();

    QSqlQuery updateSetFinish("unfinished_show_connection");
    updateSetFinish.prepare("update order_items set finished=quantity where id= :orderItemId");
    updateSetFinish.bindValue(":orderItemId",orderItemId);
    updateSetFinish.exec();

    loadUnfinishedOrders();
}
