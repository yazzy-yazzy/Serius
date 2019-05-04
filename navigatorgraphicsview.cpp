#include "navigatorgraphicsview.hpp"

#include <QtWidgets>
#include "navigatorrubberband.hpp"

NavigatorGraphicsView::NavigatorGraphicsView(QWidget *parent) : QGraphicsView(parent), nowDrag(false)
{
    setMouseTracking(true);
    setRubberBandSelectionMode(Qt::IntersectsItemBoundingRect);
    setDragMode(QGraphicsView::RubberBandDrag);

    QPalette palette;
    palette.setColor(QPalette::Active, QPalette::Highlight, Qt::red);

    rubberBand = new NavigatorRubberBand(QRubberBand::Line, this);
    rubberBand->setPalette(palette);    //it doesn't work under Mac...
}

void NavigatorGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (rubberBand->geometry().contains(event->pos())) {
        offset = event->pos() - rubberBand->pos();
        nowDrag = true;
    }
}

void NavigatorGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    nowDrag = false;
}

void NavigatorGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if (nowDrag) {
        int x = event->pos().x() - offset.x();
        int y = event->pos().y() - offset.y();
        int w = rubberBand->width();
        int h = rubberBand->height();

        emit roiChanged(mapToScene(QRect(x, y, w, h)).boundingRect());
    }
}

void NavigatorGraphicsView::drawROI(const QRectF &sceneRect)
{
    if (scene() && scene()->isActive() && scene()->items().size() > 0) {
        QRectF r = scene()->sceneRect();

        qreal x = qMax(sceneRect.x(), r.x());
        qreal y = qMax(sceneRect.y(), r.y());
        qreal w = (sceneRect.x() + sceneRect.width() > x + r.width() ? r.width() - x : sceneRect.width());
        qreal h = (sceneRect.y() + sceneRect.height() > y + r.height() ? r.height() - y : sceneRect.height());

        rubberBand->setGeometry(mapFromScene(QRectF(x, y, w, h)).boundingRect());
        rubberBand->show();
    }
}
