#ifndef ADDMenuDIALOG_H
#define ADDMenuDIALOG_H

#include <QDialog>

namespace Ui {
class AddMenuDialog;
}

class AddMenuDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddMenuDialog(QWidget *parent = nullptr);
    ~AddMenuDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AddMenuDialog *ui;
};

#endif
