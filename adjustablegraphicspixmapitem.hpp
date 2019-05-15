#ifndef ADJUSTABLEGRAPHICSPIXMAPITEM_HPP
#define ADJUSTABLEGRAPHICSPIXMAPITEM_HPP

#include <QGraphicsPixmapItem>
#include <QMap>

#include "channel.hpp"
#include "statistics.hpp"

class AdjustableGraphicsPixmapItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    explicit AdjustableGraphicsPixmapItem(QGraphicsItem *parent = nullptr);
    AdjustableGraphicsPixmapItem(const QPixmap &pixmap, QGraphicsItem *parent = nullptr);
    ~AdjustableGraphicsPixmapItem() override;

    const QImage *image() const;
    void setImage(const QImage *image);
    void setImage(const QImage &image);

    bool channelVisibles(Channel::Color channel) const;
    QMap<Channel::Color, bool> channelVisibles() const;

    int brightness() const;
    int contrast() const;

    QList<QPointF> toneCurve(Channel::Color channel) const;
    QMap<Channel::Color, QList<QPointF>> toneCurves() const;

    Statistics statistics(Channel::Color channel) const;
    QMap<Channel::Color, Statistics> statistics() const;

public slots:
    void redraw();
    void clear();

    void setBrightness(int value);
    void setContrast(int value);
    void setChannelVisible(const QMap<Channel::Color, bool> &map);
    void setToneCurves(const QMap<Channel::Color, QList<QPointF>> &map);
    void setStatistics(const QMap<Channel::Color, Statistics> &map);

signals:
    void imageChanged(const QImage &image);
    void pixmapChanged(const QPixmap &pixmap);
    void statisticsChanged(const QMap<Channel::Color, Statistics> &map);

private:
    QMap<Channel::Color, Statistics> scan(const QImage &image);

private:
    QMap<Channel::Color, bool> _visibleMap;
    int _brightness;
    int _contrast;
    QImage _original;
    QMap<Channel::Color, QList<QPointF>> _curveMap;
    QMap<Channel::Color, Statistics> _statMap;
};

#endif // ADJUSTABLEGRAPHICSPIXMAPITEM_HPP
