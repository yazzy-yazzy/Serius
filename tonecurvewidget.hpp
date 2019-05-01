#ifndef TONECURVEWIDGET_HPP
#define TONECURVEWIDGET_HPP

#include <QWidget>
#include <QtCharts>

#include "statistics.hpp"

QT_FORWARD_DECLARE_CLASS(TrackingSplineSeries);

namespace Ui {
class ToneCurveWidget;
}

class ToneCurveWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ToneCurveWidget(QWidget *parent = nullptr);
    ~ToneCurveWidget();

    QList<QPointF> points() const;
    QColor baselineColor() const;
    QColor toneCurveColor() const;
    QColor histgramColor() const;

public slots:
    void setBaselineVisible(bool visible);
    void setHistgramVisible(bool visible);
    void removeCurrentPoint();
    void clear();
    void setBaselineColor(QColor color);
    void setToneCurveColor(QColor color);
    void setHistgramColor(QColor color);
    void setHistgram(const Statistics &statistics);
    void setPoints(const QList<QPointF> &points);

signals:
    void selectionChanged(const QPointF &point);

private:
    void createChart();
    void createBaseline();
    void createToneCurve();
    void createHistgram();

private:
    Ui::ToneCurveWidget *ui;

    QChart *chart;
    QValueAxis *axisHistgramX;
    QValueAxis *axisHistgramY;
    QValueAxis *axisToneCurveX;
    QValueAxis *axisToneCurveY;

    QSplineSeries *baselineSeries;
    TrackingSplineSeries *toneCurveSeries;
    QBarSeries *histgramSeries;
};

#endif // TONECURVEWIDGET_HPP
