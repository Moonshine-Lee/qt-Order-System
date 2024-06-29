#ifndef QUANTITY_WIDGET_H
#define QUANTITY_WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>

class QuantityWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QuantityWidget(QWidget *parent = nullptr);
    int quantity() const;
    void setQuantity(int quantity);

signals:
    void quantityChanged(int quantity);

private slots:
    void increaseQuantity();
    void decreaseQuantity();

private:
    QLabel *label;
    int m_quantity;
};

#endif // QUANTITY_WIDGET_H
