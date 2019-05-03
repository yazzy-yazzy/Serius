#include "trackingsplineseries.hpp"

#include <QtWidgets>
#include "trackingchartevent.hpp"

static bool lessThan(const QPointF &l, const QPointF &r)
{
    return l.x() < r.x();
}

static const qreal INFINITE = std::numeric_limits<qreal>::max();
static const qreal TOLERANCE_DELTA = 5.0;

TrackingSplineSeries::TrackingSplineSeries(QObject *parent) : QSplineSeries(parent)
{
    startPos = QPointF();
    currentPos = QPointF();

    connect(this, &QSplineSeries::pointAdded, this, &TrackingSplineSeries::addPoint);
    connect(this, &QSplineSeries::pointRemoved, this, &TrackingSplineSeries::removePoint);
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

void TrackingSplineSeries::mousePressEvent(const QPointF &pos)
{
    startPos = nearestPoint(pos, TOLERANCE_DELTA);

    emit selectionChanged(pos);
    emit mousePressed(pos);
}

void TrackingSplineSeries::mouseMoveEvent(const QPointF &pos)
{
    if (!startPos.isNull()) {
        replace(startPos.x(), startPos.y(), pos.x(), pos.y());

        startPos = pos;
        currentPos = pos;

        emit selectionChanged(pos);
        emit mouseMoved(pos);
    }
}

void TrackingSplineSeries::mouseReleaseEvent(const QPointF &pos)
{
    QList<QPointF> list = points();
    if (startPos.isNull() && nearestPoint(pos, TOLERANCE_DELTA).isNull())
        list.append(pos);
    else
        list.replace(list.indexOf(startPos), pos);

    std::sort(list.begin(), list.end(), lessThan);

    startPos = QPointF();
    currentPos = pos;

    clear();
    append(list);

    emit selectionChanged(pos);
    emit mouseReleased(pos);
}

QPointF TrackingSplineSeries::nearestPoint(const QPointF &pos) const
{
    return nearestPoint(pos, INFINITE);
}

QPointF TrackingSplineSeries::nearestPoint(const QPointF &pos, qreal delta) const
{
    QMap<int, qreal> map;
    for (int i = 0; i < points().size(); i++) {
        QPointF diff = points().at(i) - pos;
        map.insert(i, diff.manhattanLength());
    }

    QList<qreal> list = map.values();
    std::sort(list.begin(), list.end());

    foreach (qreal manhattanLength, list) {
        if (manhattanLength < delta)
            return points().at(map.key(manhattanLength));
    }

    return QPointF();
}

bool TrackingSplineSeries::isPoint(const QPointF &pos) const
{
    return !nearestPoint(pos, TOLERANCE_DELTA).isNull();
}

const QPointF &TrackingSplineSeries::currentPoint() const
{
    return currentPos;
}

void TrackingSplineSeries::addPoint(int index)
{
    Q_UNUSED(index);

    emit selectionChanged(currentPos);
}

void TrackingSplineSeries::removePoint(int index)
{
    Q_UNUSED(index);

    currentPos = (points().size() > 0 ? nearestPoint(currentPos) : QPointF());
    emit selectionChanged(currentPos);
}
