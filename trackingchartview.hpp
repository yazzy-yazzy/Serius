#ifndef TRACKINGCHARTVIEW_HPP
#define TRACKINGCHARTVIEW_HPP

#include <QChartView>

QT_CHARTS_USE_NAMESPACE

class TrackingChartView : public QChartView
{
    Q_OBJECT
public:
    explicit TrackingChartView(QWidget *parent = nullptr);

    void setTargetSeries(QAbstractSeries *series);

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    QAbstractSeries *series;
};

#endif // TRACKINGCHARTVIEW_HPP
