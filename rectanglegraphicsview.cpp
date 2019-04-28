#include "rectanglegraphicsview.hpp"

#include <QtWidgets>

RectangleGraphicsView::RectangleGraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    recentPen.setColor(Qt::red);
    recentPen.setWidth(2);
}

void RectangleGraphicsView::drawRectangle(const QRect &sceneRect)
{
//    qDebug() << __PRETTY_FUNCTION__ << sceneRect;

    recentSceneRect = sceneRect;

    if (scene() && scene()->isActive())
        scene()->update(scene()->sceneRect());
}

void RectangleGraphicsView::setPen(QPen pen)
{
    recentPen = pen;

    if (scene() && scene()->isActive())
        scene()->update(scene()->sceneRect());
}

QPen RectangleGraphicsView::pen() const
{
    return recentPen;
}

void RectangleGraphicsView::paintEvent(QPaintEvent *event)
{
//    qDebug() << __PRETTY_FUNCTION__ << _recentRect;

    QGraphicsView::paintEvent(event);

    if (scene() && scene()->isActive() && scene()->items().size() > 0) {
        int sw = scene()->sceneRect().toRect().width();
        int sh = scene()->sceneRect().toRect().height();

        int x = qMax(recentSceneRect.x(), 0);
        int y = qMax(recentSceneRect.y(), 0);
        int w = (recentSceneRect.x() + recentSceneRect.width() > sw ? sw - x : recentSceneRect.width());
        int h = (recentSceneRect.y() + recentSceneRect.height() > sh ? sh - y : recentSceneRect.height());

        QRect drawRect = QRect(x, y, w, h);

        QPainter painter(viewport());
        painter.setPen(pen());
        painter.drawRect(mapFromScene(drawRect).boundingRect());

        recentDrawRect = drawRect;
    }
}
