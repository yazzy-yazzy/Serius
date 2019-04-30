#include "trackingchartevent.hpp"

bool ChartReleaseEvent::initialized = false;
QEvent::Type ChartReleaseEvent::type = QEvent::User;

bool ChartPressEvent::initialized = false;
QEvent::Type ChartPressEvent::type = QEvent::User;

bool ChartMoveEvent::initialized = false;
QEvent::Type ChartMoveEvent::type = QEvent::User;

ChartReleaseEvent::ChartReleaseEvent(QPointF pos) : QEvent(staticType()), mousePos(pos)
{
}

const QPointF &ChartReleaseEvent::pos() const
{
    return mousePos;
}

QEvent::Type ChartReleaseEvent::staticType()
{
    if (!initialized) {
        type = static_cast<QEvent::Type>(registerEventType());
        initialized = true;
    }
    return type;
}

bool ChartReleaseEvent::is(const QEvent * ev)
{
   return ev->type() == staticType();
}

ChartPressEvent::ChartPressEvent(QPointF pos) : QEvent(staticType()), mousePos(pos)
{
}

const QPointF &ChartPressEvent::pos() const
{
    return mousePos;
}

QEvent::Type ChartPressEvent::staticType()
{
    if (!initialized) {
        type = static_cast<QEvent::Type>(registerEventType());
        initialized = true;
    }
    return type;
}

bool ChartPressEvent::is(const QEvent * ev)
{
   return ev->type() == staticType();
}

ChartMoveEvent::ChartMoveEvent(QPointF pos) : QEvent(staticType()), mousePos(pos)
{
}

const QPointF &ChartMoveEvent::pos() const
{
    return mousePos;
}

QEvent::Type ChartMoveEvent::staticType()
{
    if (!initialized) {
        type = static_cast<QEvent::Type>(registerEventType());
        initialized = true;
    }
    return type;
}

bool ChartMoveEvent::is(const QEvent * ev)
{
   return ev->type() == staticType();
}
