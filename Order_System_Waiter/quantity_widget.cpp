#include "quantity_widget.h"

QuantityWidget::QuantityWidget(QWidget *parent) : QWidget(parent), m_quantity(0)
{
    QPushButton *btnIncrease = new QPushButton("+", this);
    QPushButton *btnDecrease = new QPushButton("-", this);
    label = new QLabel(QString::number(m_quantity), this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(btnDecrease);
    layout->addWidget(label);
    layout->addWidget(btnIncrease);

    connect(btnIncrease, &QPushButton::clicked, this, &QuantityWidget::increaseQuantity);
    connect(btnDecrease, &QPushButton::clicked, this, &QuantityWidget::decreaseQuantity);
}

int QuantityWidget::quantity() const
{
    return m_quantity;
}

void QuantityWidget::setQuantity(int quantity)
{
    if (quantity != m_quantity) {
        m_quantity = quantity;
        label->setText(QString::number(m_quantity));
        emit quantityChanged(m_quantity);
    }
}

void QuantityWidget::increaseQuantity()
{
    setQuantity(m_quantity + 1);
}

void QuantityWidget::decreaseQuantity()
{
    if (m_quantity > 0) {
        setQuantity(m_quantity - 1);
    }
}

