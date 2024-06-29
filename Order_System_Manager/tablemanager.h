#ifndef TABLEMANAGER_H
#define TABLEMANAGER_H

#include <QWidget>

namespace Ui {
class TableManager;
}

class TableManager : public QWidget
{
    Q_OBJECT

public:
    explicit TableManager(QWidget *parent = nullptr);
    ~TableManager();

private slots:
    void on_addTableButton_clicked();
    void on_editTableButton_clicked();
    void on_deleteTableButton_clicked();
    void loadTables();

private:
    Ui::TableManager *ui;
};

#endif // TABLEMANAGER_H
