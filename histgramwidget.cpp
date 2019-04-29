#include "histgramwidget.hpp"
#include "ui_histgramwidget.h"

inline int luminance(const QColor &c)
{
    const auto cr = 0.298912;
    const auto cg = 0.586611;
    const auto cb = 0.114477;

    int luminance = static_cast<int>(c.red() * cr + c.green() * cg + c.blue() * cb);
    return qBound(0, luminance, 255);
}

HistgramWidget::HistgramWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistgramWidget)
{
    ui->setupUi(this);

    createChart();

    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);
    ui->chartView->setContentsMargins(QMargins());

    clear();
}

HistgramWidget::~HistgramWidget()
{
    delete ui;
}

void HistgramWidget::createChart()
{
    axisX = new QValueAxis();
    axisX->setTickCount(5);
    axisX->setRange(0, 255);
    axisX->setLabelFormat("%d");
    axisX->setLinePenColor(Qt::darkGray);
    axisX->setGridLineColor(Qt::darkGray);
    axisX->setGridLineVisible(true);
    axisX->setLineVisible(false);

    axisY = new QValueAxis();
    axisY->setTickCount(5);
    axisY->setRange(0, 10000);
    axisY->setLabelFormat("%d");
    axisY->setLinePenColor(Qt::darkGray);
    axisY->setGridLineColor(Qt::darkGray);
    axisY->setGridLineVisible(true);
    axisY->setLineVisible(false);

    chart = new QChart();
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    chart->setMargins(QMargins(0, 0, 0, 0));
    chart->setBackgroundRoundness(0);
    chart->legend()->hide();
    chart->setBackgroundBrush(ui->chartView->backgroundBrush());

//    chart->setTitle(tr("Histgram"));
//    chart->setAnimationOptions(QChart::AllAnimations);
//    chart->setPlotAreaBackgroundBrush(Qt::darkGray);
//    chart->setPlotAreaBackgroundVisible(true);
}

void HistgramWidget::draw(const QImage &image)
{
    clear();

    scan(image);

    drawLuminance();
//    drawRGB();
    drawText();
}

void HistgramWidget::scan(const QImage &image)
{
    for (int y = 0; y < image.height(); y++) {
        for (int x = 0; x < image.width(); x++) {
            QColor c = image.pixelColor(x, y);
            statisticsL.append(luminance(c));
            statisticsR.append(c.red());
            statisticsG.append(c.green());
            statisticsB.append(c.blue());

        }
    }

    statisticsL.update();
    statisticsR.update();
    statisticsG.update();
    statisticsB.update();
}

void HistgramWidget::clear()
{
    chart->removeAllSeries();
    axisX->setLabelsVisible(false);
    axisY->setLabelsVisible(false);

    ui->editMean->clear();
    ui->editStdDev->clear();
    ui->editMedian->clear();
    ui->editPixels->clear();

    statisticsL.clear();
    statisticsR.clear();
    statisticsG.clear();
    statisticsB.clear();
}

void HistgramWidget::drawLuminance()
{
    axisY->setRange(0, static_cast<qreal>(statisticsL.histgramMax()));

    QBarSet *bar = new QBarSet("Luminance");
    bar->setColor(Qt::gray);

    for (int i = 0; i < 256; i++)
        bar->insert(i, statisticsL.histgram(i));

    QBarSeries *series = new QBarSeries();
    series->setBarWidth(1);
    series->append(bar);

    chart->addSeries(series);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    axisX->setLabelsVisible(true);
    axisY->setLabelsVisible(true);
}

void HistgramWidget::drawRGB()
{
    QPen penR, penG, penB;
    penR.setColor(Qt::red);
    penG.setColor(Qt::green);
    penB.setColor(Qt::blue);
    penR.setWidth(2);
    penG.setWidth(2);
    penB.setWidth(2);

    QSplineSeries *seriesR = new QSplineSeries();
    QSplineSeries *seriesG = new QSplineSeries();
    QSplineSeries *seriesB = new QSplineSeries();

    seriesR->setPen(penR);
    seriesG->setPen(penG);
    seriesB->setPen(penB);

    for (int i = 0; i < 256; i++) {
        seriesR->append(i, statisticsR.histgram(i));
        seriesG->append(i, statisticsG.histgram(i));
        seriesB->append(i, statisticsB.histgram(i));
    }

    chart->addSeries(seriesR);
    chart->addSeries(seriesG);
    chart->addSeries(seriesB);

    seriesR->attachAxis(axisX);
    seriesR->attachAxis(axisY);
    seriesG->attachAxis(axisX);
    seriesG->attachAxis(axisY);
    seriesB->attachAxis(axisX);
    seriesB->attachAxis(axisY);
}

void HistgramWidget::drawText()
{
    ui->editMean->setText(QString("%1").arg(statisticsL.mean(), 0, 'f', 2));
    ui->editStdDev->setText(QString("%1").arg(statisticsL.stddev(), 0, 'f', 2));
    ui->editMedian->setText(QString("%1").arg(statisticsL.median()));
    ui->editPixels->setText(QString("%L1").arg(statisticsL.count()));
}
