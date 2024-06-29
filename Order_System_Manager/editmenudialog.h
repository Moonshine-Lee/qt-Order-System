#ifndef EDITMenuDIALOG_H
#define EDITMenuDIALOG_H

#include <QDialog>
#include<QCloseEvent>
namespace Ui {
class EditMenuDialog;
}

class EditMenuDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditMenuDialog(int Id, QWidget *parent = nullptr);
    ~EditMenuDialog();

protected:

private slots:
    void on_buttonBox_accepted();

private:
    Ui::EditMenuDialog *ui;
    int Id;
};

#endif
