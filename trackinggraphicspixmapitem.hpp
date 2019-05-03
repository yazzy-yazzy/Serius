#ifndef TRACKINGGRAPHICSPIXMAPITEM_HPP
#define TRACKINGGRAPHICSPIXMAPITEM_HPP

#include <QGraphicsPixmapItem>

class TrackingGraphicsPixmapItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    explicit TrackingGraphicsPixmapItem(QGraphicsItem *parent = nullptr);
    TrackingGraphicsPixmapItem(const QPixmap & pixmap, QGraphicsItem *parent = nullptr);
    ~TrackingGraphicsPixmapItem() override;

    void setPixmapEx(const QPixmap &pixmap);

signals:
    void pixmapChanged(const QPixmap &pixmap);
};

#endif // TRACKINGGRAPHICSPIXMAPITEM_HPP
