#include "trackingchartview.hpp"

#include <QtWidgets>
#include <QtCharts/QAbstractSeries>
#include "trackingchartevent.hpp"

TrackingChartView::TrackingChartView(QWidget *parent) : QChartView(parent)
{
}

void TrackingChartView::mouseMoveEvent(QMouseEvent *event)
{
    if (chart()) {
        QPointF p2 = mapToScene(event->pos());
        QPointF p3 = chart()->mapFromScene(p2);
        QPointF p4 = chart()->mapToValue(p3);

        foreach (QAbstractSeries *series, chart()->series())
            QApplication::postEvent(series, new ChartMoveEvent(p4));
    }

    QChartView::mouseMoveEvent(event);
}

void TrackingChartView::mousePressEvent(QMouseEvent *event)
{
    if (chart()) {
        QPointF p2 = mapToScene(event->pos());
        QPointF p3 = chart()->mapFromScene(p2);
        QPointF p4 = chart()->mapToValue(p3);

        foreach (QAbstractSeries *series, chart()->series())
            QApplication::postEvent(series, new ChartPressEvent(p4));
    }

    QChartView::mousePressEvent(event);
}

void TrackingChartView::mouseReleaseEvent(QMouseEvent *event)
{
    if (chart()) {
        QPointF p2 = mapToScene(event->pos());
        QPointF p3 = chart()->mapFromScene(p2);
        QPointF p4 = chart()->mapToValue(p3);

        foreach (QAbstractSeries *series, chart()->series())
            QApplication::postEvent(series, new ChartReleaseEvent(p4));
    }

    QChartView::mouseReleaseEvent(event);
}
