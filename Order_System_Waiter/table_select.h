#ifndef TABLE_SELECT_H
#define TABLE_SELECT_H

#include <QWidget>
#include <QSqlDatabase>
#include <QPushButton>
#include <QMessageBox>
#include <QGridLayout>
#include <QSqlError>
#include <QSqlQuery>

namespace Ui {
class Table_Select;
}

class Table_Select : public QWidget
{
    Q_OBJECT

public:
    explicit Table_Select(QWidget *parent = nullptr);
    ~Table_Select();

private slots:
    void handleTableButtonClicked();

private:
    Ui::Table_Select *ui;
    QSqlDatabase db;
    void loadTables();
    QPushButton* createTableButton(int tableId, const QString &status);
};

#endif // TABLE_SELECT_H
