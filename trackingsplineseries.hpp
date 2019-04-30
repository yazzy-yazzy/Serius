#ifndef TRACKINGSPLINESERIES_HPP
#define TRACKINGSPLINESERIES_HPP

#include <QObject>
#include <QtCharts/QSplineSeries>

QT_CHARTS_USE_NAMESPACE

class TrackingSplineSeries : public QSplineSeries
{
    Q_OBJECT

public:
    explicit TrackingSplineSeries(QObject *parent = nullptr);

    const QPointF &currentPoint() const;

protected:
    void customEvent(QEvent *event) override;

signals:
    void mousePressed(const QPointF &pos);
    void mouseMoved(const QPointF &pos);
    void mouseReleased(const QPointF &pos);

private:
    void mousePressEvent(const QPointF &pos);
    void mouseMoveEvent(const QPointF &pos);
    void mouseReleaseEvent(const QPointF &pos);
    bool isPoint(const QPointF &pos) const;
    QPointF nearestPoint(const QPointF &pos) const;
    bool isEditable(const QPointF &pos) const;

private:
    QPointF startPos;
    QPointF currentPos;
};

#endif // TRACKINGSPLINESERIES_HPP
