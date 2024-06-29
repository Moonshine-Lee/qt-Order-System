#ifndef EDITUSERDIALOG_H
#define EDITUSERDIALOG_H

#include <QDialog>

namespace Ui {
class EditUserDialog;
}

class EditUserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditUserDialog(int userId, QWidget *parent = nullptr);
    ~EditUserDialog();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::EditUserDialog *ui;
    int m_userId;
};

#endif // EDITUSERDIALOG_H
