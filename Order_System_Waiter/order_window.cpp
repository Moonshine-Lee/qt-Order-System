#include "order_window.h"
#include "ui_order_window.h"
#include "table_select.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSpinBox>
#include "database.h"

OrderWindow::OrderWindow(int tableId, bool isAddDish, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrderWindow),
    tableId(tableId),
    isAddDish(isAddDish),
    totalAmount(0.0)
{
    ui->setupUi(this);

    // 设置窗口标题
    setWindowTitle(QString("桌台 %1 %2").arg(tableId).arg(isAddDish ? "加菜" : "点单"));

    // 设置 QSplitter 的比例
    QList<int> sizes;
    sizes << 600 << 400;
    ui->splitter->setSizes(sizes);

    // 连接数据库
    db=getDatabase();
    // 加载菜单和当前订单
    loadMenu();
    if (isAddDish) {
        loadCurrentOrder();
    }

    // 信号槽连接
    connect(ui->btn_confirm, &QPushButton::clicked, this, &OrderWindow::handleConfirm);
    connect(ui->btn_cancel, &QPushButton::clicked, this, &OrderWindow::handleCancel);
    connect(ui->btn_checkout, &QPushButton::clicked, this, &OrderWindow::handleCheckout);
}

OrderWindow::~OrderWindow()
{
    if (db.isOpen()) {
        db.close();
    }
    delete ui;
}

void OrderWindow::loadMenu()
{
    QSqlQuery query;
    query.prepare("SELECT id, name, price FROM menu");

    if (!query.exec()) {
        QMessageBox::critical(this, "数据库错误", query.lastError().text());
        return;
    }

    ui->tableWidget_menu->setRowCount(0);
    while (query.next()) {
        int row = ui->tableWidget_menu->rowCount();
        ui->tableWidget_menu->insertRow(row);

        QTableWidgetItem *itemName = new QTableWidgetItem(query.value(1).toString());
        QTableWidgetItem *itemPrice = new QTableWidgetItem(query.value(2).toString());
        itemPrice->setFlags(itemPrice->flags() & ~Qt::ItemIsEditable); // 设置价格不可修改

        QSpinBox *spinBox = new QSpinBox(this);
        spinBox->setRange(0, 100);
        spinBox->setValue(0);

        QPushButton *btnSelect = new QPushButton("选择", this);
        connect(btnSelect, &QPushButton::clicked, this, &OrderWindow::handleMenuSelection);
      //把需要的控件绑定到列表上
        ui->tableWidget_menu->setItem(row, 0, itemName);
        ui->tableWidget_menu->setItem(row, 1, itemPrice);
        ui->tableWidget_menu->setCellWidget(row, 2, spinBox);
        ui->tableWidget_menu->setCellWidget(row, 3, btnSelect);
    }
}

void OrderWindow::loadCurrentOrder()
{
    QSqlQuery query;
    query.prepare("SELECT menu.id, menu.name, menu.price, order_items.quantity "
                  "FROM order_items "
                  "JOIN menu ON order_items.menu_id = menu.id "
                  "WHERE order_items.order_id = (SELECT id FROM orders WHERE table_id = :tableId AND status = '待结账')");
    query.bindValue(":tableId", tableId);

    if (!query.exec()) {
        QMessageBox::critical(this, "数据库错误", query.lastError().text());
        return;
    }

    ui->tableWidget_order->setRowCount(0);
    while (query.next()) {
        int row = ui->tableWidget_order->rowCount();
        //在最后一行插入数据，把查出来的数据都插入进去
        ui->tableWidget_order->insertRow(row);

        QTableWidgetItem *itemName = new QTableWidgetItem(query.value(1).toString());
        QTableWidgetItem *itemQuantity = new QTableWidgetItem(query.value(3).toString());
        QTableWidgetItem *itemPrice = new QTableWidgetItem(query.value(2).toString());
        ui->tableWidget_order->setItem(row, 0, itemName);
        ui->tableWidget_order->setItem(row, 1, itemQuantity);
        ui->tableWidget_order->setItem(row, 2, itemPrice);

        double price = query.value(2).toDouble();
        int quantity = query.value(3).toInt();
        totalAmount += price * quantity;
    }
    updateTotalAmount();
}

void OrderWindow::handleMenuSelection()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    int row = ui->tableWidget_menu->indexAt(btn->pos()).row();
    QString name = ui->tableWidget_menu->item(row, 0)->text();
    double price = ui->tableWidget_menu->item(row, 1)->text().toDouble();
    int quantity = static_cast<QSpinBox*>(ui->tableWidget_menu->cellWidget(row, 2))->value();

    if (quantity <= 0) {
        QMessageBox::warning(this, "无效数量", "请选择有效的数量。");
        return;
    }

    // 检查当前订单是否已有该菜品
    for (int i = 0; i < ui->tableWidget_order->rowCount(); ++i) {
        if (ui->tableWidget_order->item(i, 0)->text() == name) {
            int newQuantity = ui->tableWidget_order->item(i, 1)->text().toInt() + quantity;
            ui->tableWidget_order->item(i, 1)->setText(QString::number(newQuantity));
            totalAmount += price * quantity;
            updateTotalAmount();
            return;
        }
    }

    // 如果没有，则添加到当前订单
    int rowOrder = ui->tableWidget_order->rowCount();
    ui->tableWidget_order->insertRow(rowOrder);
    ui->tableWidget_order->setItem(rowOrder, 0, new QTableWidgetItem(name));
    ui->tableWidget_order->setItem(rowOrder, 1, new QTableWidgetItem(QString::number(quantity)));
    ui->tableWidget_order->setItem(rowOrder, 2, new QTableWidgetItem(QString::number(price)));
    totalAmount += price * quantity;
    updateTotalAmount();
}

void OrderWindow::handleConfirm()
{
    //如果长度为0 则直接退出
    if(ui->tableWidget_order->rowCount()==0){
        Table_Select* tableSelect =new Table_Select;
        tableSelect->show();
        this->close();
        return;
    }

    QSqlQuery query;
    query.prepare("Select id from orders where orders.table_id = :tableId and orders.status='待结账'");
    query.bindValue(":tableId",tableId);
    int orderId;
    query.exec();
    //没有查询出订单来 那么就创建一个订单,默认状态待结账
    if(!query.next()){
        QSqlQuery insertOrderQuery;
        insertOrderQuery.prepare("Insert into orders (table_id,total_amount) values (:tableId,:totalAmount)");
        insertOrderQuery.bindValue(":tableId",tableId);
        insertOrderQuery.bindValue(":totalAmount",totalAmount);
        insertOrderQuery.exec();
        query.exec();
        query.next();
        orderId=query.value(0).toInt();
    }
    else {
        //如果查询出订单来了 那么更改订单的价格
        orderId=query.value(0).toInt();
        QSqlQuery updateOrderQuery;
        updateOrderQuery.prepare("update orders set total_amount= :totalAmount where id= :orderId");
        updateOrderQuery.bindValue(":orderId",orderId);
        updateOrderQuery.bindValue(":totalAmount",totalAmount);
        updateOrderQuery.exec();
    }
    //把原有订单里面的item都查出来 如果有该item ，那么更新数量 如果没有item 则把新的item给插入进去

    QSqlQuery selectItemquery;
    selectItemquery.prepare("select order_items.id,menu.name,order_items.quantity,order_items.menu_id from order_items join menu on menu.id=order_items.menu_id where order_id = :orderId");
    selectItemquery.bindValue(":orderId",orderId);
    selectItemquery.exec();

    //对于原订单中已包含的，如果数量不一致 则更新数量
    //设计一个数据结构 存储已被处理过的菜品名
    QSet<QString> Doneset;
    QSqlQuery updateItemquery;
    updateItemquery.prepare("update order_items set quantity = :quantity where id = :itemId");
    while(selectItemquery.next()){
        QString menuName= selectItemquery.value(1).toString();
        int quantity=selectItemquery.value(2).toInt();
        int itemId=selectItemquery.value(0).toInt();
        for (int i=0;i< ui->tableWidget_order->rowCount() ;i++ ) {
            if(ui->tableWidget_order->item(i,0)->text()==menuName){
                if(ui->tableWidget_order->item(i,1)->text().toInt() != quantity ){
                    updateItemquery.bindValue(":quantity",ui->tableWidget_order->item(i,1)->text().toInt());
                    updateItemquery.bindValue(":itemId",itemId);
                }
                updateItemquery.exec();
                Doneset.insert(menuName);
            }
        }
    }
    QSqlQuery selectMenuIdquery;
    selectMenuIdquery.prepare("select id from menu where name= :menuName");

    QSqlQuery insertItemquery;
    insertItemquery.prepare("insert into order_items(order_id,menu_id,quantity) values(:orderId,:menuId, :quantity) ");
    //对于原订单中未包含的 则插入数据
    for (int i=0;i< ui->tableWidget_order->rowCount() ;i++ ){
        if(!Doneset.contains(ui->tableWidget_order->item(i,0)->text())){
            selectMenuIdquery.bindValue(":menuName",ui->tableWidget_order->item(i,0)->text());
            selectMenuIdquery.exec();
            selectMenuIdquery.next();
            int menuId =selectMenuIdquery.value(0).toInt();
            insertItemquery.bindValue(":orderId",orderId);
            insertItemquery.bindValue(":menuId",menuId);
            int quantity= ui->tableWidget_order->item(i,1)->text().toInt();
            insertItemquery.bindValue(":quantity",quantity);
            insertItemquery.exec();
        }
    }
    //提交订单之后 更改桌台状态为occupied
    QSqlQuery updateTableQuery;
    updateTableQuery.prepare("update tables set status='occupied' where id= :tableId ");
    updateTableQuery.bindValue(":tableId",tableId);
    updateTableQuery.exec();

    Table_Select* tableSelect =new Table_Select;
    tableSelect->show();
    this->close();

}
void OrderWindow::precheckOut()
{
    if(ui->tableWidget_order->rowCount()==0){
        Table_Select* tableSelect =new Table_Select;
        tableSelect->show();
        this->close();
        return;
    }
    QSqlQuery query;
    query.prepare("Select id from orders where orders.table_id = :tableId and orders.status='待结账'");
    query.bindValue(":tableId",tableId);
    int orderId;
    query.exec();
    if(!query.next()){
        QSqlQuery insertOrderQuery;
        insertOrderQuery.prepare("Insert into orders (table_id,total_amount) values (:tableId,:totalAmount)");
        insertOrderQuery.bindValue(":tableId",tableId);
        insertOrderQuery.bindValue(":totalAmount",totalAmount);
        insertOrderQuery.exec();
        query.exec();
        query.next();
        orderId=query.value(0).toInt();
    }
    else {
        orderId=query.value(0).toInt();
        QSqlQuery updateOrderQuery;
        updateOrderQuery.prepare("update orders set total_amount= :totalAmount where id= :orderId");
        updateOrderQuery.bindValue(":orderId",orderId);
        updateOrderQuery.bindValue(":totalAmount",totalAmount);
        updateOrderQuery.exec();
    }

    QSqlQuery selectItemquery;
    selectItemquery.prepare("select order_items.id,menu.name,order_items.quantity,order_items.menu_id from order_items join menu on menu.id=order_items.menu_id where order_id = :orderId");
    selectItemquery.bindValue(":orderId",orderId);
    selectItemquery.exec();
    QSet<QString> Doneset;
    QSqlQuery updateItemquery;
    updateItemquery.prepare("update order_items set quantity = :quantity where id = :itemId");
    while(selectItemquery.next()){
        QString menuName= selectItemquery.value(1).toString();
        int quantity=selectItemquery.value(2).toInt();
        int itemId=selectItemquery.value(0).toInt();
        for (int i=0;i< ui->tableWidget_order->rowCount() ;i++ ) {
            if(ui->tableWidget_order->item(i,0)->text()==menuName){
                if(ui->tableWidget_order->item(i,1)->text().toInt() != quantity ){
                    updateItemquery.bindValue(":quantity",quantity);
                    updateItemquery.bindValue(":itemId",itemId);
                }
                updateItemquery.exec();
                Doneset.insert(menuName);
            }
        }
    }
    QSqlQuery selectMenuIdquery;
    selectMenuIdquery.prepare("select id from menu where name= :menuName");

    QSqlQuery insertItemquery;
    insertItemquery.prepare("insert into order_items(order_id,menu_id,quantity) values(:orderId,:menuId, :quantity) ");
    for (int i=0;i< ui->tableWidget_order->rowCount() ;i++ ){
        if(!Doneset.contains(ui->tableWidget_order->item(i,0)->text())){
            selectMenuIdquery.bindValue(":menuName",ui->tableWidget_order->item(i,0)->text());
            selectMenuIdquery.exec();
            selectMenuIdquery.next();
            int menuId =selectMenuIdquery.value(0).toInt();
            insertItemquery.bindValue(":orderId",orderId);
            insertItemquery.bindValue(":menuId",menuId);
            int quantity= ui->tableWidget_order->item(i,1)->text().toInt();
            insertItemquery.bindValue(":quantity",quantity);
            insertItemquery.exec();
        }
    }

}

void OrderWindow::handleCancel()
{
    Table_Select* tableSelect =new Table_Select;
    tableSelect->show();
    this->close();
}

void OrderWindow::handleCheckout()
{
    //首先 在结账之前，确认数据已被提交了
    precheckOut();

    //收款逻辑不再设计

    //结账之后 更改桌台状态为available，订单状态为已结账
    QSqlQuery updateTableQuery;
    updateTableQuery.prepare("update tables set status='available' where id= :tableId ");
    updateTableQuery.bindValue(":tableId",tableId);
    updateTableQuery.exec();

    //查询出orderId
    QSqlQuery query;
    query.prepare("Select id from orders where orders.table_id = :tableId and orders.status='待结账'");
    query.bindValue(":tableId",tableId);
    int orderId;
    query.exec();
    query.next();
    orderId=query.value(0).toInt();

    QSqlQuery updateOrderQuery;
    updateOrderQuery.prepare("update orders set status='已结账' where id= :orderId ");
    updateOrderQuery.bindValue(":orderId",orderId);
    updateOrderQuery.exec();

    Table_Select* tableSelect =new Table_Select;
    tableSelect->show();
    this->close();

}

void OrderWindow::updateTotalAmount()
{
    ui->label_totalAmount->setText(QString("总金额：%1 元").arg(totalAmount));
}
