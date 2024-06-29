#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "usermanager.h"
#include "menumanager.h"
#include "tablemanager.h"
#include "orderlists.h"
#include "reportmanager.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 设置每个项的高度
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        QListWidgetItem* item = ui->listWidget->item(i);
        item->setSizeHint(QSize(item->sizeHint().width(), 40));  // 40 是你想要的高度
    }

    // 设置字体居中
    ui->listWidget->setStyleSheet("QListWidget::item { text-align: center; }");

    // 连接侧边栏点击信号到槽函数
    connect(ui->listWidget, &QListWidget::itemClicked, this, &MainWindow::on_sidebar_item_clicked);

    // 创建并添加UserManager
    UserManager *userManager = new UserManager(this);
    ui->stackedWidget->addWidget(userManager); // 这个顺序很重要

    // 添加其他模块的Widget
    // 假设MenuManager是菜单管理模块的Widget
    MenuManager *menuManager = new MenuManager; // 替换为实际的菜单管理模块
    ui->stackedWidget->addWidget(menuManager);

    TableManager *tableManager =new TableManager;
    ui->stackedWidget->addWidget(tableManager);

    OrderLists *orderlist=new OrderLists;
    ui->stackedWidget->addWidget(orderlist);

    ReportManager *reportManager =new ReportManager;
    ui->stackedWidget->addWidget(reportManager);

    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_sidebar_item_clicked(QListWidgetItem *item)
{
    QString text = item->text();

    if (text == "用户管理")
    {
        ui->stackedWidget->setCurrentIndex(0); // 这里的索引应与添加UserManager时的顺序一致
    }
    else if (text == "菜单管理")
    {
        ui->stackedWidget->setCurrentIndex(1); // 这里的索引应与添加MenuManager时的顺序一致
    }
    else if (text == "餐桌管理")
    {
        ui->stackedWidget->setCurrentIndex(2);

    }
    else if (text == "订单列表")
    {
         ui->stackedWidget->setCurrentIndex(3);
        // 添加餐桌管理模块的逻辑
    }

    else if (text == "报表")
    {
        ui->stackedWidget->setCurrentIndex(4);
    }

}
