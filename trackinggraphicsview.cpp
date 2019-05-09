#include "trackinggraphicsview.hpp"

#include <QtWidgets>

TrackingGraphicsView::TrackingGraphicsView(QWidget *parent) : QGraphicsView (parent)
{
    setMouseTracking(true);
}

void TrackingGraphicsView::paintEvent(QPaintEvent *event)
{
    if (scene() && scene()->isActive()) {
        QRectF roi = mapToScene(viewport()->geometry()).boundingRect();

        if (_roi != roi) {
            emit roiChanged(roi);
            _roi = roi;
        }

        event->accept();
    }

    QGraphicsView::paintEvent(event);
}

void TrackingGraphicsView::scaleEx(qreal s)
{
    QGraphicsView::scale(s, s);

    Q_ASSERT(qFuzzyCompare(matrix().m11(), matrix().m22()));
    emit scaleChanged(matrix().m11());
}

void TrackingGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if (scene() && scene()->isActive() && scene()->items().size() > 0) {
//        if (viewport()->geometry().contains(event->pos()))
            emit entered(mapToScene(event->pos()));
    }

    QGraphicsView::mouseMoveEvent(event);
}

void TrackingGraphicsView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
}

void TrackingGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
}

bool TrackingGraphicsView::event(QEvent *event)
{
    if (event->type() == QEvent::Leave)
        emit leaved();

    return QGraphicsView::event(event);
}

QRectF TrackingGraphicsView::roi() const
{
    return _roi;
}
