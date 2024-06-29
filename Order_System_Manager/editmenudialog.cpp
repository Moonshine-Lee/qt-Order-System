#include "editmenudialog.h"
#include "ui_editmenudialog.h"
#include <QSqlQuery>
#include <QMessageBox>
#include<QCloseEvent>
EditMenuDialog::EditMenuDialog(int Id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditMenuDialog),
    Id(Id)
{
    ui->setupUi(this);

    QSqlQuery query;
    query.prepare("SELECT name, price FROM Menu WHERE id = :id");
    query.bindValue(":id", Id);
    query.exec(); query.next();
        ui->lineEditMenuName->setText(query.value(0).toString());
        ui->lineEditPrice->setText(query.value(1).toString());

}

EditMenuDialog::~EditMenuDialog()
{
    delete ui;
}

void EditMenuDialog::on_buttonBox_accepted()
{
    QString name = ui->lineEditMenuName->text();
    QString price =ui->lineEditPrice->text();

    if (name.isEmpty()||price.isEmpty()) {
        QMessageBox::warning(this, "警告", "菜品名");
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE menu SET name = :menuName, price = :price WHERE id = :id");
    query.bindValue(":menuName", name);
    query.bindValue(":role", price.toInt());
    query.bindValue(":id", Id);

    query.exec();
}

