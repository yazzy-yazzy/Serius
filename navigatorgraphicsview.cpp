#include "navigatorgraphicsview.hpp"

#include <QtWidgets>
#include "navigatorrubberband.hpp"

NavigatorGraphicsView::NavigatorGraphicsView(QWidget *parent) : QGraphicsView(parent), _nowDrag(false)
{
    setMouseTracking(true);
    setRubberBandSelectionMode(Qt::IntersectsItemBoundingRect);
    setDragMode(QGraphicsView::RubberBandDrag);

    QPalette palette;
    palette.setColor(QPalette::Active, QPalette::Highlight, Qt::red);

    _rubberBand = new NavigatorRubberBand(QRubberBand::Line, this);
    _rubberBand->setPalette(palette);    //it doesn't work under Mac...
}

void NavigatorGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (_rubberBand->geometry().contains(event->pos())) {
        _offset = event->pos() - _rubberBand->pos();
        _nowDrag = true;
    }
}

void NavigatorGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    _nowDrag = false;
}

void NavigatorGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if (_nowDrag) {
        int x = event->pos().x() - _offset.x();
        int y = event->pos().y() - _offset.y();
        int w = _rubberBand->width();
        int h = _rubberBand->height();

        emit roiChanged(mapToScene(QRect(x, y, w, h)).boundingRect());
    }
}

void NavigatorGraphicsView::drawROI(const QRectF &roi)
{
    if (scene() && scene()->isActive() && scene()->items().size() > 0) {
        QRectF r = scene()->sceneRect();

        qreal x = qMax(roi.x(), r.x());
        qreal y = qMax(roi.y(), r.y());
        qreal w = (roi.x() + roi.width() > x + r.width() ? r.width() - x : roi.width());
        qreal h = (roi.y() + roi.height() > y + r.height() ? r.height() - y : roi.height());

        _rubberBand->setGeometry(mapFromScene(QRectF(x, y, w, h)).boundingRect());
        _rubberBand->show();
    }
}

void NavigatorGraphicsView::fitToWindow(const QImage &image)
{
    const int margin = 10;

    qreal factorX = static_cast<qreal>(width() - margin) / static_cast<qreal>(image.width());
    qreal factorY = static_cast<qreal>(height() - margin) / static_cast<qreal>(image.height());
    qreal factor = qMin(factorX, factorY);

    resetMatrix();
    scale(factor, factor);
}

QRubberBand *NavigatorGraphicsView::rubberBand() const
{
    return _rubberBand;
}
