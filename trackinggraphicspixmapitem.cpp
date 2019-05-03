#include "trackinggraphicspixmapitem.hpp"

#include <QtWidgets>

TrackingGraphicsPixmapItem::TrackingGraphicsPixmapItem(QGraphicsItem *parent) : QGraphicsPixmapItem(parent)
{
}

TrackingGraphicsPixmapItem::TrackingGraphicsPixmapItem(const QPixmap & pixmap, QGraphicsItem *parent) : QGraphicsPixmapItem (pixmap, parent)
{
}

TrackingGraphicsPixmapItem::~TrackingGraphicsPixmapItem()
{
}

void TrackingGraphicsPixmapItem::setPixmapEx(const QPixmap &pixmap)
{
    QGraphicsPixmapItem::setPixmap(pixmap);
    emit pixmapChanged(pixmap);
}
