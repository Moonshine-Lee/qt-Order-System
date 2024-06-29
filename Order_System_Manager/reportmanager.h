#ifndef REPORTMANAGER_H
#define REPORTMANAGER_H

#include <QWidget>

namespace Ui {
class ReportManager;
}

class ReportManager : public QWidget
{
    Q_OBJECT

public:
    explicit ReportManager(QWidget *parent = nullptr);
    ~ReportManager();

private slots:
    void generateDailySalesReport();
    void generateMenuItemSalesReport();

private:
    Ui::ReportManager *ui;
    void loadDailySalesReport();
    void loadMenuItemSalesReport();
};

#endif // REPORTMANAGER_H
