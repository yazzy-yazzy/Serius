#include "adjustablegraphicspixmapitem.hpp"

#include <QtWidgets>

#include "utility.hpp"

AdjustableGraphicsPixmapItem::AdjustableGraphicsPixmapItem(QGraphicsItem *parent) : QGraphicsPixmapItem(parent)
{
    clear();
}

AdjustableGraphicsPixmapItem::AdjustableGraphicsPixmapItem(const QPixmap & pixmap, QGraphicsItem *parent) : QGraphicsPixmapItem (pixmap, parent)
{
    clear();
}

AdjustableGraphicsPixmapItem::~AdjustableGraphicsPixmapItem()
{
}

void AdjustableGraphicsPixmapItem::clear()
{
    _original = nullptr;

    _visibleMap.insert(Channel::red, true);
    _visibleMap.insert(Channel::green, true);
    _visibleMap.insert(Channel::blue, true);

    _brightness = 0;
    _contrast = 0;

    _curveMap.clear();
}

void AdjustableGraphicsPixmapItem::setImage(const QImage *image)
{
    _original = image;
}

bool AdjustableGraphicsPixmapItem::channelVisibles(Channel::Color channel) const
{
    return _visibleMap.value(channel, true);
}

QMap<Channel::Color, bool> AdjustableGraphicsPixmapItem::channelVisibles() const
{
    return _visibleMap;
}

void AdjustableGraphicsPixmapItem::setChannelVisible(const QMap<Channel::Color, bool> &map)
{
    _visibleMap = map;
}

void AdjustableGraphicsPixmapItem::redraw()
{
    if (!_original)
        return;

    QImage tmp = _original->copy();

    QList<Channel::Color> list;
    if (!channelVisibles(Channel::red))
        list.append(Channel::red);
    if (!channelVisibles(Channel::green))
        list.append(Channel::green);
    if (!channelVisibles(Channel::blue))
        list.append(Channel::blue);

    Utility::erase(tmp, list);

    QMap<int, int> lutL = Utility::createLUT(_curveMap.value(Channel::luminance));
    QMap<int, int> lutR = Utility::createLUT(_curveMap.value(Channel::red));
    QMap<int, int> lutG = Utility::createLUT(_curveMap.value(Channel::green));
    QMap<int, int> lutB = Utility::createLUT(_curveMap.value(Channel::blue));

    if (lutR.isEmpty() && lutG.isEmpty() && lutB.isEmpty())
        Utility::convert(tmp, lutL);
    else
        Utility::convert(tmp, lutR, lutG, lutB);

    Utility::brightness(tmp, brightness());
    Utility::contrast(tmp, contrast());

    QPixmap pixmap = QPixmap::fromImage(tmp);
    QGraphicsPixmapItem::setPixmap(pixmap);

    emit pixmapChanged(pixmap);
}

int AdjustableGraphicsPixmapItem::brightness() const
{
    return _brightness;
}

void AdjustableGraphicsPixmapItem::setBrightness(int value)
{
    _brightness = value;
}

int AdjustableGraphicsPixmapItem::contrast() const
{
    return _contrast;
}

void AdjustableGraphicsPixmapItem::setContrast(int value)
{
    _contrast = value;
}

QList<QPointF> AdjustableGraphicsPixmapItem::toneCurve(Channel::Color channel) const
{
    return _curveMap.value(channel);
}

QMap<Channel::Color, QList<QPointF>> AdjustableGraphicsPixmapItem::toneCurves() const
{
    return _curveMap;
}

void AdjustableGraphicsPixmapItem::setToneCurves(const QMap<Channel::Color, QList<QPointF>> &map)
{
    _curveMap = map;
}

