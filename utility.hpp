#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <QMap>
#include <QList>
#include <QColor>
#include <QtMath>
#include <QImage>

#include "channel.hpp"
#include "kernel.hpp"

namespace Utility
{
    QList<QPointF> catmullRomCircuit(const QList<QPointF> &inputPoints);

    QMap<int, int> createLUT(qreal gamma);
    QMap<int, int> createLUT(const QList<QPointF> &points);

    int calcLuminanceFromRGB(const QColor &c);
    int findMedian(int histgram[], int totalCount);

    QImage contrast(QImage &source, int factor);
    QImage brightness(QImage &source, int factor);
    QImage convert(QImage &source, const QMap<int, int> &lut);
    QImage convert(QImage &source, const QMap<int, int> &lutR, const QMap<int, int> &lutG, const QMap<int, int> &lutB);
    QImage erase(QImage &source, const QList<Channel::Color> &channel);

    QImage convolute(QImage &dstImage, const QImage &srcImage, const Kernel &kernel);
}

#endif // UTILITY_HPP
