#ifndef UNFINISHED_SHOW_H
#define UNFINISHED_SHOW_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

#include <QPushButton>
#include <QGridLayout>
#include <QTableWidget>
#include <QTimer>
namespace Ui {
class unfinished_show;
}

class unfinished_show : public QWidget
{
    Q_OBJECT

public:
    explicit unfinished_show(QWidget *parent = nullptr);
    ~unfinished_show();

private:
    Ui::unfinished_show *ui;

    void loadUnfinishedOrders();
    QSqlDatabase db;
    void handleFinish();
    QTimer* timer;
};

#endif // UNFINISHED_SHOW_H
