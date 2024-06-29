#ifndef ORDER_WINDOW_H
#define ORDER_WINDOW_H

#include <QWidget>
#include <QSqlDatabase>
#include <QPushButton>
#include <QMessageBox>
#include <QGridLayout>
#include <QTableWidget>

namespace Ui {
class OrderWindow;
}

class OrderWindow : public QWidget
{
    Q_OBJECT

public:
    explicit OrderWindow(int tableId, bool isAddDish = false, QWidget *parent = nullptr);
    ~OrderWindow();

private slots:
    void handleMenuSelection();
    void handleConfirm();
    void handleCancel();
    void handleCheckout();

private:
    Ui::OrderWindow *ui;
    QSqlDatabase db;
    int tableId;
    bool isAddDish;
    double totalAmount;
    void loadMenu();
    void loadCurrentOrder();
    void updateTotalAmount();
    void precheckOut();
};

#endif // ORDER_WINDOW_H
