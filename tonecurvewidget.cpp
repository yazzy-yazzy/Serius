#include "tonecurvewidget.hpp"
#include "ui_tonecurvewidget.h"

#include "trackingsplineseries.hpp"

ToneCurveWidget::ToneCurveWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToneCurveWidget)
{
    ui->setupUi(this);

    createChart();
    createHistgram();
    createBaseline();
    createToneCurve();

    //setup gradation label
    ui->vGradationLabel->setStyleSheet("background:qlineargradient(x1:1, y1:0, x2:0, y2:0,"
                                  "stop:0 white, stop:1.0 black);");
    ui->hGradationLabel->setStyleSheet("background:qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                                  "stop:0 white, stop:1.0 black);");

    //setup chart
    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);
    ui->chartView->setContentsMargins(QMargins());

    ui->chartView->setTargetSeries(toneCurveSeries);
}

ToneCurveWidget::~ToneCurveWidget()
{
    delete ui;
}

void ToneCurveWidget::createChart()
{
    axisToneCurveX = new QValueAxis();
    axisToneCurveX->setTickCount(5);
    axisToneCurveX->setRange(0, 255);
    axisToneCurveX->setLabelFormat("%d");
    axisToneCurveX->setLinePenColor(Qt::darkGray);
    axisToneCurveX->setGridLineColor(Qt::darkGray);
    axisToneCurveX->setGridLineVisible(true);
    axisToneCurveX->setLineVisible(false);

    axisToneCurveY = new QValueAxis();
    axisToneCurveY->setTickCount(5);
    axisToneCurveY->setRange(0, 255);
    axisToneCurveY->setLabelFormat("%d");
    axisToneCurveY->setLinePenColor(Qt::darkGray);
    axisToneCurveY->setGridLineColor(Qt::darkGray);
    axisToneCurveY->setGridLineVisible(true);
    axisToneCurveY->setLineVisible(false);

    axisHistgramX = new QValueAxis();
    axisHistgramX->setTickCount(5);
    axisHistgramX->setRange(0, 255);
    axisHistgramX->setLabelFormat("%d");
    axisHistgramX->setLinePenColor(Qt::darkGray);
    axisHistgramX->setGridLineColor(Qt::darkGray);
    axisHistgramX->setGridLineVisible(true);
    axisHistgramX->setLineVisible(false);
    axisHistgramX->setLabelsVisible(false);

    axisHistgramY = new QValueAxis();
    axisHistgramY->setTickCount(5);
    axisHistgramY->setRange(0, 255);
    axisHistgramY->setLabelFormat("%d");
    axisHistgramY->setLinePenColor(Qt::darkGray);
    axisHistgramY->setGridLineColor(Qt::darkGray);
    axisHistgramY->setGridLineVisible(false);
    axisHistgramY->setLineVisible(false);
    axisHistgramY->setLabelsVisible(false);

    chart = new QChart();
//    chart->setAnimationOptions(QChart::AllAnimations);  //TBD
    chart->addAxis(axisToneCurveX, Qt::AlignBottom);
    chart->addAxis(axisToneCurveY, Qt::AlignLeft);
    chart->addAxis(axisHistgramX, Qt::AlignBottom);
    chart->addAxis(axisHistgramY, Qt::AlignLeft);
    chart->setMargins(QMargins(0, 0, 0, 0));
    chart->setBackgroundRoundness(0);
    chart->legend()->hide();
    chart->setBackgroundBrush(ui->chartView->backgroundBrush());
}

void ToneCurveWidget::createBaseline()
{
    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(2);

    baselineSeries = new QSplineSeries();
    baselineSeries->setPen(pen);
    baselineSeries->append(0, 0);
    baselineSeries->append(255, 255);

    chart->addSeries(baselineSeries);
    baselineSeries->attachAxis(axisToneCurveX);
    baselineSeries->attachAxis(axisToneCurveY);
}

void ToneCurveWidget::createToneCurve()
{
    QPen pen;
    pen.setColor(Qt::gray);
    pen.setWidth(2);

    toneCurveSeries = new TrackingSplineSeries();
    toneCurveSeries->setPen(pen);
    toneCurveSeries->append(0, 0);
    toneCurveSeries->append(255, 255);
    toneCurveSeries->setPointsVisible(true);
//    toneCurveSeries->setPointLabelsVisible(true);
//    toneCurveSeries->setPointLabelsFormat("(@xPoint, @yPoint)");

    chart->addSeries(toneCurveSeries);
    toneCurveSeries->attachAxis(axisToneCurveX);
    toneCurveSeries->attachAxis(axisToneCurveY);

    connect(toneCurveSeries, SIGNAL(selectionChanged(QPointF)), this, SIGNAL(selectionChanged(QPointF)));
    connect(toneCurveSeries, SIGNAL(pointAdded(int)), this, SIGNAL(pointAdded(int)));
    connect(toneCurveSeries, SIGNAL(pointRemoved(int)), this, SIGNAL(pointRemoved(int)));
}

void ToneCurveWidget::createHistgram()
{
    histgramSeriesU = new QLineSeries();
    histgramSeriesL = new QLineSeries();
    histgramSeriesU->setColor(Qt::darkGray);
    histgramSeriesL->setColor(Qt::darkGray);

    QPen pen(Qt::darkGray);
    pen.setWidth(1);

    histgramSeriesA = new QAreaSeries(histgramSeriesU, histgramSeriesL);
    histgramSeriesA->setPen(pen);
    histgramSeriesA->setColor(Qt::darkGray);

    chart->addSeries(histgramSeriesU);
    chart->addSeries(histgramSeriesL);
    chart->addSeries(histgramSeriesA);
    histgramSeriesU->attachAxis(axisHistgramX);
    histgramSeriesU->attachAxis(axisHistgramY);
    histgramSeriesL->attachAxis(axisHistgramX);
    histgramSeriesL->attachAxis(axisHistgramY);
    histgramSeriesA->attachAxis(axisHistgramX);
    histgramSeriesA->attachAxis(axisHistgramY);
}

void ToneCurveWidget::setBaselineVisible(bool visible)
{
    baselineSeries->setVisible(visible);
}

void ToneCurveWidget::setHistgramVisible(bool visible)
{
    histgramSeriesU->setVisible(visible);
    histgramSeriesL->setVisible(visible);
    histgramSeriesA->setVisible(visible);
}

void ToneCurveWidget::removeCurrentPoint()
{
    if (!toneCurveSeries->currentPoint().isNull())
        toneCurveSeries->remove(toneCurveSeries->currentPoint());
}

void ToneCurveWidget::clear()
{
    toneCurveSeries->clear();
    toneCurveSeries->append(0, 0);
    toneCurveSeries->append(255, 255);

    emit selectionChanged(QPointF());
}

QList<QPointF> ToneCurveWidget::points() const
{
    return toneCurveSeries->points();
}

void ToneCurveWidget::setBaselineColor(QColor color)
{
    baselineSeries->setColor(color);
}

QColor ToneCurveWidget::baselineColor() const
{
    return baselineSeries->color();
}

void ToneCurveWidget::setToneCurveColor(QColor color)
{
    toneCurveSeries->setColor(color);
}

QColor ToneCurveWidget::toneCurveColor() const
{
    return toneCurveSeries->color();
}

void ToneCurveWidget::setHistgramColor(QColor color)
{
    histgramSeriesU->setColor(color);
    histgramSeriesL->setColor(color);
    histgramSeriesA->setColor(color);
}

QColor ToneCurveWidget::histgramColor() const
{
    return histgramSeriesA->color();
}

void ToneCurveWidget::setHistgram(const Statistics &statistics)
{
    axisHistgramY->setRange(0, static_cast<qreal>(statistics.histgramMax()));

    histgramSeriesU->clear();
    histgramSeriesL->clear();

    for (int i = 0; i < 256; i++) {
        histgramSeriesU->append(i, statistics.histgram(i));
        histgramSeriesL->append(i, 0);
    }
}

void ToneCurveWidget::setPoints(const QList<QPointF> &points)
{
    toneCurveSeries->clear();

    if (points.empty()) {
        toneCurveSeries->append(0, 0);
        toneCurveSeries->append(255, 255);
    } else {
        toneCurveSeries->append(points);
    }

    emit selectionChanged(QPointF());
}
