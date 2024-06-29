#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QListWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_sidebar_item_clicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
    void adjustListWidgetItemStyle();
};

#endif // MAINWINDOW_H
