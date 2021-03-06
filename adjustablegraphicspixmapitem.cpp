#include "adjustablegraphicspixmapitem.hpp"

#include <QtWidgets>

#include "utility.hpp"

AdjustableGraphicsPixmapItem::AdjustableGraphicsPixmapItem(QGraphicsItem *parent) : QGraphicsPixmapItem(parent)
{
    qRegisterMetaType<QMap<Channel::Color, Statistics>>();

    clear();
}

AdjustableGraphicsPixmapItem::AdjustableGraphicsPixmapItem(const QPixmap & pixmap, QGraphicsItem *parent) : QGraphicsPixmapItem (pixmap, parent)
{
    qRegisterMetaType<QMap<Channel::Color, Statistics>>();

    clear();
}

AdjustableGraphicsPixmapItem::~AdjustableGraphicsPixmapItem()
{
}

void AdjustableGraphicsPixmapItem::clear()
{
    _original = QImage();

    _visibleMap.insert(Channel::red, true);
    _visibleMap.insert(Channel::green, true);
    _visibleMap.insert(Channel::blue, true);

    _brightness = 0;
    _contrast = 0;

    _curveMap.clear();
    _kernel.fill(0);
}

const QImage *AdjustableGraphicsPixmapItem::image() const
{
    return &_original;
}

void AdjustableGraphicsPixmapItem::setImage(const QImage &image)
{
    _original = image.copy();

    emit imageChanged(_original);
}

void AdjustableGraphicsPixmapItem::setImage(const QImage *image)
{
    if (!image)
        return;

    setImage(*image);
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
    if (_original.isNull())
        return;

    QImage tmp;
    if (_kernel.isValid()) {
        QImage src = _original.copy();
        tmp = QImage(src.size(), src.format());
        Utility::convolute(tmp, src, _kernel);
    } else {
        tmp = _original.copy();
    }

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

    setStatistics(scan(tmp));
}

QImage AdjustableGraphicsPixmapItem::convert()
{
    if (_original.isNull())
        return QImage();

    QImage tmp;
    if (_kernel.isValid()) {
        QImage src = _original.copy();
        tmp = QImage(src.size(), src.format());
        Utility::convolute(tmp, src, _kernel);
    } else {
        tmp = _original.copy();
    }

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

    setStatistics(scan(tmp));

    return tmp;
}

void AdjustableGraphicsPixmapItem::drawPixmap(const QImage &image)
{
    if (image.isNull())
        return;

    QPixmap pixmap = QPixmap::fromImage(image);

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

QMap<Channel::Color, Statistics> AdjustableGraphicsPixmapItem::scan(const QImage &image)
{
    QMap<Channel::Color, Statistics> map;
    Statistics statL, statR, statG, statB;

    for (int y = 0; y < image.height(); y++) {
        for (int x = 0; x < image.width(); x++) {
            QColor c = image.pixelColor(x, y);
            statL.append(Utility::calcLuminanceFromRGB(c));
            statR.append(c.red());
            statG.append(c.green());
            statB.append(c.blue());

        }
    }

    statL.update();
    statR.update();
    statG.update();
    statB.update();

    map.insert(Channel::luminance, statL);
    map.insert(Channel::red, statR);
    map.insert(Channel::green, statG);
    map.insert(Channel::blue, statB);

    return map;
}

Statistics AdjustableGraphicsPixmapItem::statistics(Channel::Color channel) const
{
    return _statMap.value(channel);
}

QMap<Channel::Color, Statistics> AdjustableGraphicsPixmapItem::statistics() const
{
    return _statMap;
}

void AdjustableGraphicsPixmapItem::setStatistics(const QMap<Channel::Color, Statistics> &map)
{
    _statMap = map;
    emit statisticsChanged(_statMap);
}

Kernel AdjustableGraphicsPixmapItem::kernel() const
{
    return _kernel;
}

void AdjustableGraphicsPixmapItem::setKernel(const Kernel &kernel)
{
    _kernel = kernel;
}

