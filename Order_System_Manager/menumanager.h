#ifndef MENUMANAGER_H
#define MENUMANAGER_H

#include <QWidget>

namespace Ui {
class MenuManager;
}

class MenuManager : public QWidget
{
    Q_OBJECT

public:
    explicit MenuManager(QWidget *parent = nullptr);
    ~MenuManager();

private:
    Ui::MenuManager *ui;

private slots:
    void on_addMenuButton_clicked();
    void on_editMenuButton_clicked();
    void on_deleteMenuButton_clicked();
    void loadMenu();

};

#endif
