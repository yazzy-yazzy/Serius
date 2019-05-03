#include "utility.hpp"

#include <QtCore>

QList<QPointF> Utility::catmullRomCircuit(const QList<QPointF> &inputPoints)
{
    auto n = inputPoints.size();

    QList<QPointF> points;
    QList<QPointF> outputPoints;

    points.append(inputPoints.at(n - 1));
    points.append(inputPoints.at(0));
    for (auto i = 1; i < n; i++)
        points.append(inputPoints.at(i));
    points.append(inputPoints.at(0));

    for (auto i = 1; i < n ; i++) {
        const QPointF &p0 = points.at(i - 1);
        const QPointF &p1 = points.at(i);
        const QPointF &p2 = points.at(i + 1);
        const QPointF &p3 = points.at(i + 2);

        qreal ax = -1 * p0.x() + 3 * p1.x() - 3 * p2.x() + 1 * p3.x();
        qreal bx =  2 * p0.x() - 5 * p1.x() + 4 * p2.x() - 1 * p3.x();
        qreal cx = -1 * p0.x() + 0 * p1.x() + 1 * p2.x() + 0 * p3.x();
        qreal dx =  0 * p0.x() + 2 * p1.x() + 0 * p2.x() + 0 * p3.x();

        qreal ay = -1 * p0.y() + 3 * p1.y() - 3 * p2.y() + 1 * p3.y();
        qreal by =  2 * p0.y() - 5 * p1.y() + 4 * p2.y() - 1 * p3.y();
        qreal cy = -1 * p0.y() + 0 * p1.y() + 1 * p2.y() + 0 * p3.y();
        qreal dy =  0 * p0.y() + 2 * p1.y() + 0 * p2.y() + 0 * p3.y();

        auto partition = qAbs(p2.x() - p1.x()) * 2;

        for (auto j = 0; j <= partition; j++) {
            qreal t = static_cast<qreal>(j) / static_cast<qreal>(partition);
            qreal x = (((ax * t * t * t) + (bx * t * t) + (cx * t) + dx) / 2);
            qreal y = (((ay * t * t * t) + (by * t * t) + (cy * t) + dy) / 2);
            outputPoints.append(QPointF(x, y));
        }
    }

    return outputPoints;
}

QMap<int, int> createLUT(qreal gamma)
{
    QMap<int, int> lut;
    for (int i = 0; i < 256; i++) {
        qreal x = static_cast<qreal>(i) / 255;
        int value = qBound(0, static_cast<int>(255 * (pow(x, 1 / gamma))), 255);
        lut.insert(i, value);
    }
    return lut;
}

QMap<int, int> Utility::createLUT(const QList<QPointF> &points)
{
    if (3 > points.size())
        return QMap<int, int>();

    QMap<int, int> lut;
    QList<QPointF> sdpoints = catmullRomCircuit(points);

    int nearesetIndex = 0;
    for (int i = 0; i < 256; i++) {
        qreal deltaMin = std::numeric_limits<qreal>::max();
        for (int n = nearesetIndex; n < sdpoints.size(); n++) {
            qreal delta = qAbs(sdpoints.at(n).x() - static_cast<qreal>(i));
            if (delta > deltaMin)
                break;

            deltaMin = delta;
            nearesetIndex = n;
        }

        nearesetIndex = qBound(0, nearesetIndex, sdpoints.size() - 1);
        int value = qBound(0, static_cast<int>(sdpoints.at(nearesetIndex).y()), 255);

        lut.insert(i, value);
        ++nearesetIndex;
    }

    return lut;
}

int Utility::calcLuminanceFromRGB(const QColor &c)
{
    const auto cr = 0.298912;
    const auto cg = 0.586611;
    const auto cb = 0.114477;

    int luminance = static_cast<int>(c.red() * cr + c.green() * cg + c.blue() * cb);
    return qBound(0, luminance, 255);
}

int Utility::findMedian(int histgram[], int totalCount)
{
    const auto medianIndex = totalCount / 2;
    auto refCount = 0;

    for (auto i = 0; i < 256; i++) {
        refCount += histgram[i];
        if (refCount >= medianIndex)
            return i;
    }
    return 0;
}

QImage Utility::contrast(QImage &source, int factor)
{
    if (factor == 0)
        return source;

    const qreal contrast = (100.0 + factor) / 100.0;

    for (int y = 0; y < source.height(); y++) {
        for (int x = 0; x < source.width(); x++) {
            QColor c = source.pixelColor(x, y);
            c.setRedF(qBound(0.0, 0.5 + contrast * (c.redF() - 0.5), 1.0));
            c.setGreenF(qBound(0.0, 0.5 + contrast * (c.greenF() - 0.5), 1.0));
            c.setBlueF(qBound(0.0, 0.5 + contrast * (c.blueF() - 0.5), 1.0));
            source.setPixelColor(x, y, c);
        }
    }

    return source;
}

QImage Utility::brightness(QImage &source, int factor)
{
    if (factor == 0)
        return source;

    const qreal brightness = (100.0 + factor) / 100.0;

    for (int y = 0; y < source.height(); y++) {
        for (int x = 0; x < source.width(); x++) {
            QColor c = source.pixelColor(x, y);
            c.setRedF(qBound(0.0, c.redF() * brightness, 1.0));
            c.setGreenF(qBound(0.0, c.greenF() * brightness, 1.0));
            c.setBlueF(qBound(0.0, c.blueF() * brightness, 1.0));
            source.setPixelColor(x, y, c);
        }
    }

    return source;
}

QImage Utility::convert(QImage &source, const QMap<int, int> &lut)
{
    if (lut.isEmpty())
        return source;

    for (int y = 0; y < source.height(); y++) {
        for (int x = 0; x < source.width(); x++) {
            QColor c = source.pixelColor(x, y);
            c.setRed(lut.value(c.red(), c.red()));
            c.setGreen(lut.value(c.green(), c.green()));
            c.setBlue(lut.value(c.blue(), c.blue()));
            source.setPixelColor(x, y, c);
        }
    }

    return source;
}

QImage Utility::convert(QImage &source, const QMap<int, int> &lutR, const QMap<int, int> &lutG, const QMap<int, int> &lutB)
{
    if (lutR.isEmpty() && lutG.isEmpty() && lutB.isEmpty())
        return source;

    for (int y = 0; y < source.height(); y++) {
        for (int x = 0; x < source.width(); x++) {
            QColor c = source.pixelColor(x, y);
            c.setRed(lutR.value(c.red(), c.red()));
            c.setGreen(lutG.value(c.green(), c.green()));
            c.setBlue(lutB.value(c.blue(), c.blue()));
            source.setPixelColor(x, y, c);
        }
    }

    return source;
}

QImage Utility::erase(QImage &source, const QList<Channel::Color> &channel)
{
    if (channel.isEmpty())
        return source;

    const bool eraseR = channel.contains(Channel::red);
    const bool eraseG = channel.contains(Channel::green);
    const bool eraseB = channel.contains(Channel::blue);

    for (int y = 0; y < source.height(); y++) {
        for (int x = 0; x < source.width(); x++) {
            QColor c = source.pixelColor(x, y);
            if (eraseR)
                c.setRed(0);
            if (eraseG)
                c.setGreen(0);
            if (eraseB)
                c.setBlue(0);
            source.setPixelColor(x, y, c);
        }
    }

    return source;
}
