#ifndef ADJUSTABLEGRAPHICSPIXMAPITEM_HPP
#define ADJUSTABLEGRAPHICSPIXMAPITEM_HPP

#include <QGraphicsPixmapItem>
#include <QMap>

#include "channel.hpp"

class AdjustableGraphicsPixmapItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    explicit AdjustableGraphicsPixmapItem(QGraphicsItem *parent = nullptr);
    AdjustableGraphicsPixmapItem(const QPixmap &pixmap, QGraphicsItem *parent = nullptr);
    ~AdjustableGraphicsPixmapItem() override;

    void setImage(const QImage *image);

    bool channelVisibles(Channel::Color channel) const;
    QMap<Channel::Color, bool> channelVisibles() const;
    void setChannelVisible(const QMap<Channel::Color, bool> &map);

    int brightness() const;
    void setBrightness(int value);

    int contrast() const;
    void setContrast(int value);

    QList<QPointF> toneCurve(Channel::Color channel) const;
    QMap<Channel::Color, QList<QPointF>> toneCurves() const;
    void setToneCurves(const QMap<Channel::Color, QList<QPointF>> &map);

public slots:
    void redraw();
    void clear();

signals:
    void pixmapChanged(const QPixmap &pixmap);

private:
    QMap<Channel::Color, bool> _visibleMap;
    int _brightness;
    int _contrast;
    const QImage *_original;
    QMap<Channel::Color, QList<QPointF>> _curveMap;
};

#endif // ADJUSTABLEGRAPHICSPIXMAPITEM_HPP
