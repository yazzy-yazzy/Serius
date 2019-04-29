#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <QMap>
#include <QList>
#include <QElapsedTimer>
#include <QtMath>

namespace
{
    QList<QPointF> catmullRomCircuit(const QList<QPointF> &inputPoints)
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

    QMap<int, int> createLUT(const QList<QPointF> &points)
    {
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

            int value = qBound(0, static_cast<int>(sdpoints.at(nearesetIndex).y()), 255);

            lut.insert(i, value);
            ++nearesetIndex;
        }

        return lut;
    }
}

#endif // UTILITY_HPP
