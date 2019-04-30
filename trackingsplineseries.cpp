#include "trackingsplineseries.hpp"

#include <QtWidgets>
#include "trackingchartevent.hpp"

static bool lessThan(const QPointF &l, const QPointF &r)
{
    return l.x() < r.x();
}

TrackingSplineSeries::TrackingSplineSeries(QObject *parent) : QSplineSeries(parent)
{
    startPos = QPointF();
    currentPos = QPointF();
}

void TrackingSplineSeries::customEvent(QEvent *event)
{
    if (event->type() == ChartPressEvent::staticType()) {
        ChartPressEvent *event2 = static_cast<ChartPressEvent *>(event);
        mousePressEvent(event2->pos());
    } else if (event->type() == ChartMoveEvent::staticType()) {
        ChartMoveEvent *event2 = static_cast<ChartMoveEvent *>(event);
        mouseMoveEvent(event2->pos());
    } else if (event->type() == ChartReleaseEvent::staticType()) {
        ChartReleaseEvent *event2 = static_cast<ChartReleaseEvent *>(event);
        mouseReleaseEvent(event2->pos());
    }

    event->accept();
}

void TrackingSplineSeries::mousePressEvent(const QPointF &point)
{
    startPos = nearestPoint(point);

    emit mousePressed(point);
}

void TrackingSplineSeries::mouseMoveEvent(const QPointF &point)
{
    if (!startPos.isNull()) {
        replace(startPos.x(), startPos.y(), point.x(), point.y());

        startPos = point;
        currentPos = point;

        emit mouseMoved(point);
    }
}

void TrackingSplineSeries::mouseReleaseEvent(const QPointF &pos)
{
    QList<QPointF> list = points();
    if (startPos.isNull() && nearestPoint(pos).isNull())
        list.append(pos);
    else
        list.replace(list.indexOf(startPos), pos);

    std::sort(list.begin(), list.end(), lessThan);

    clear();
    append(list);

    startPos = QPointF();
    currentPos = pos;

    emit mouseReleased(pos);
}

QPointF TrackingSplineSeries::nearestPoint(const QPointF &point) const
{
    foreach (QPointF r, points()) {
        QPointF delta = r - point;
        if (delta.manhattanLength() < 5.0)
            return r;
    }
    return QPointF();
}

bool TrackingSplineSeries::isPoint(const QPointF &point) const
{
    return !nearestPoint(point).isNull();
}

const QPointF &TrackingSplineSeries::currentPoint() const
{
    return currentPos;
}
