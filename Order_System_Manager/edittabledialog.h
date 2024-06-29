#ifndef EDITTABLEDIALOG_H
#define EDITTABLEDIALOG_H

#include <QDialog>

namespace Ui {
class EditTableDialog;
}

class EditTableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditTableDialog(int tableId, QWidget *parent = nullptr);
    ~EditTableDialog();

protected:

private slots:
    void on_buttonBox_accepted();

private:
    Ui::EditTableDialog *ui;
    int m_tableId;
};

#endif // EDITTABLEDIALOG_H
