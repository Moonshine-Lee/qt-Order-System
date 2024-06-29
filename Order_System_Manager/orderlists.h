#ifndef ORDERLISTS_H
#define ORDERLISTS_H

#include <QWidget>

namespace Ui {
class OrderLists;
}

class OrderLists : public QWidget
{
    Q_OBJECT

public:
    explicit OrderLists(QWidget *parent = nullptr);
    ~OrderLists();

private:
    void updateValue();
    Ui::OrderLists *ui;
    QTimer* timer;
};

#endif // ORDERLISTS_H
