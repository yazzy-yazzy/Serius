#include "navigatorrubberband.hpp"

#include <QtWidgets>

NavigatorRubberBand::NavigatorRubberBand(Shape s, QWidget *parent) : QRubberBand(s, parent)
{
    setAttribute(Qt::WA_TranslucentBackground, true);
}

void NavigatorRubberBand::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    if (isVisible()) {
        QPen pen;
        pen.setColor(Qt::red);
        pen.setWidth(3);

        QPainter painter(this);
        painter.setPen(pen);
        painter.drawRect(rect());
    }
}
