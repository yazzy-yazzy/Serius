#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <QMap>
#include <QList>
#include <QColor>
#include <QtMath>

namespace Utility
{
    QList<QPointF> catmullRomCircuit(const QList<QPointF> &inputPoints);

    QMap<int, int> createLUT(qreal gamma);
    QMap<int, int> createLUT(const QList<QPointF> &points);

    int calcLuminanceFromRGB(const QColor &c);
    int findMedian(int histgram[], int totalCount);
}

#endif // UTILITY_HPP
