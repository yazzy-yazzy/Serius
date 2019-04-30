#include "tonecurvewidget.hpp"
#include "ui_tonecurvewidget.h"

ToneCurveWidget::ToneCurveWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToneCurveWidget)
{
    ui->setupUi(this);

    createChart();
    createBaseline();

    //setup gradation label
    ui->vGradationLabel->setStyleSheet("background:qlineargradient(x1:1, y1:0, x2:0, y2:0,"
                                  "stop:0 white, stop:1.0 black);");
    ui->hGradationLabel->setStyleSheet("background:qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                                  "stop:0 white, stop:1.0 black);");

    //setup chart
    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);
    ui->chartView->setContentsMargins(QMargins());
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

void ToneCurveWidget::setBaselineVisible(bool visible)
{
    baselineSeries->setVisible(visible);
}

void ToneCurveWidget::setHistgramVisible(bool visible)
{
    //TODO
}

void ToneCurveWidget::removeCurrentPoint()
{

}
