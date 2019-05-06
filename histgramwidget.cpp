#include "histgramwidget.hpp"
#include "ui_histgramwidget.h"

#include "utility.hpp"
#include "channel.hpp"

HistgramWidget::HistgramWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistgramWidget)
{
    ui->setupUi(this);

    createChart();

    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);
    ui->chartView->setContentsMargins(QMargins());

    connect(ui->rgbCheckBox, &QCheckBox::toggled, this, &HistgramWidget::setRGBVisible);

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
    drawRGB();

    setLuminanceVisible(true);
    setRGBVisible(ui->rgbCheckBox->isChecked());

    drawLuminanceText();
}

void HistgramWidget::scan(const QImage &image)
{
    Statistics statL, statR, statG, statB;

    for (int y = 0; y < image.height(); y++) {
        for (int x = 0; x < image.width(); x++) {
            QColor c = image.pixelColor(x, y);
            statL.append(Utility::calcLuminanceFromRGB(c));
            statR.append(c.red());
            statG.append(c.green());
            statB.append(c.blue());

        }
    }

    statL.update();
    statR.update();
    statG.update();
    statB.update();

    statMap.insert(Channel::luminance, statL);
    statMap.insert(Channel::red, statR);
    statMap.insert(Channel::green, statG);
    statMap.insert(Channel::blue, statB);
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

    statMap.clear();

    seriesListL.clear();
    seriesListRGB.clear();
}

void HistgramWidget::drawLuminance()
{
    Statistics statL = statMap.value(Channel::luminance);
    axisY->setRange(0, static_cast<qreal>(statL.histgramMax()));

    QLineSeries *seriesU = new QLineSeries();
    QLineSeries *seriesL = new QLineSeries();
    seriesU->setColor(Qt::gray);
    seriesL->setColor(Qt::gray);

    for (int i = 0; i < 256; i++) {
        seriesU->append(i, statL.histgram(i));
        seriesL->append(i, 0);
    }

    QAreaSeries *seriesA = new QAreaSeries(seriesU, seriesL);

    QPen pen(Qt::gray);
    pen.setWidth(1);
    seriesA->setPen(pen);
    seriesA->setBrush(QBrush(Qt::gray));

    chart->addSeries(seriesL);
    chart->addSeries(seriesU);
    chart->addSeries(seriesA);
    seriesL->attachAxis(axisX);
    seriesL->attachAxis(axisY);
    seriesU->attachAxis(axisX);
    seriesU->attachAxis(axisY);
    seriesA->attachAxis(axisX);
    seriesA->attachAxis(axisY);

    axisX->setLabelsVisible(true);
    axisY->setLabelsVisible(true);

    seriesListL.append(seriesL);
    seriesListL.append(seriesU);
    seriesListL.append(seriesA);
}

void HistgramWidget::drawRGB()
{
    Statistics statR = statMap.value(Channel::red);
    Statistics statG = statMap.value(Channel::green);
    Statistics statB = statMap.value(Channel::blue);

    QLineSeries *seriesRU = new QLineSeries();
    QLineSeries *seriesGU = new QLineSeries();
    QLineSeries *seriesBU = new QLineSeries();
//    QLineSeries *seriesRL = new QLineSeries();
//    QLineSeries *seriesGL = new QLineSeries();
//    QLineSeries *seriesBL = new QLineSeries();

    for (int i = 0; i < 256; i++) {
        seriesRU->append(i, statR.histgram(i));
        seriesGU->append(i, statG.histgram(i));
        seriesBU->append(i, statB.histgram(i));
//        seriesRL->append(i, 0);
//        seriesGL->append(i, 0);
//        seriesBL->append(i, 0);
    }

    QPen penR, penG, penB;
    penR.setColor(Qt::red);
    penG.setColor(Qt::green);
    penB.setColor(Qt::blue);
    penR.setWidth(1);
    penG.setWidth(1);
    penB.setWidth(1);

    seriesRU->setPen(penR);
    seriesGU->setPen(penG);
    seriesBU->setPen(penB);

//    QAreaSeries *seriesRA = new QAreaSeries(seriesRU, seriesRL);
//    QAreaSeries *seriesGA = new QAreaSeries(seriesGU, seriesGL);
//    QAreaSeries *seriesBA = new QAreaSeries(seriesBU, seriesBL);

//    seriesRA->setPen(penR);
//    seriesGA->setPen(penG);
//    seriesBA->setPen(penB);

    chart->addSeries(seriesRU);
    chart->addSeries(seriesGU);
    chart->addSeries(seriesBU);
//    chart->addSeries(seriesRL);
//    chart->addSeries(seriesGL);
//    chart->addSeries(seriesBL);
//    chart->addSeries(seriesRA);
//    chart->addSeries(seriesGA);
//    chart->addSeries(seriesBA);

    seriesRU->attachAxis(axisX);
    seriesRU->attachAxis(axisY);
    seriesGU->attachAxis(axisX);
    seriesGU->attachAxis(axisY);
    seriesBU->attachAxis(axisX);
    seriesBU->attachAxis(axisY);
//    seriesRL->attachAxis(axisX);
//    seriesRL->attachAxis(axisY);
//    seriesGL->attachAxis(axisX);
//    seriesGL->attachAxis(axisY);
//    seriesBL->attachAxis(axisX);
//    seriesBL->attachAxis(axisY);
//    seriesRA->attachAxis(axisX);
//    seriesRA->attachAxis(axisY);
//    seriesGA->attachAxis(axisX);
//    seriesGA->attachAxis(axisY);
//    seriesBA->attachAxis(axisX);
//    seriesBA->attachAxis(axisY);

    seriesListRGB.append(seriesRU);
//    seriesListRGB.append(seriesRL);
//    seriesListRGB.append(seriesRA);
    seriesListRGB.append(seriesGU);
//    seriesListRGB.append(seriesGL);
//    seriesListRGB.append(seriesGA);
    seriesListRGB.append(seriesBU);
//    seriesListRGB.append(seriesBL);
//    seriesListRGB.append(seriesBA);
}

void HistgramWidget::drawLuminanceText()
{
    Statistics statL = statMap.value(Channel::luminance);

    ui->editMean->setText(QString("%1").arg(statL.mean(), 0, 'f', 2));
    ui->editStdDev->setText(QString("%1").arg(statL.stddev(), 0, 'f', 2));
    ui->editMedian->setText(QString("%1").arg(statL.median()));
    ui->editPixels->setText(QString("%L1").arg(statL.count()));
}

void HistgramWidget::drawRGBText()
{
    Statistics statL = statMap.value(Channel::luminance);
    Statistics statR = statMap.value(Channel::red);
    Statistics statG = statMap.value(Channel::green);
    Statistics statB = statMap.value(Channel::blue);

    ui->editMean->setText(QString("%1-%2-%3")
                            .arg(statR.mean(), 0, 'f', 2)
                            .arg(statG.mean(), 0, 'f', 2)
                            .arg(statB.mean(), 0, 'f', 2));
    ui->editStdDev->setText(QString("%1-%2-%3")
                            .arg(statR.stddev(), 0, 'f', 2)
                            .arg(statG.stddev(), 0, 'f', 2)
                            .arg(statB.stddev(), 0, 'f', 2));
    ui->editMedian->setText(QString("%1-%2-%3")
                            .arg(statR.median(), 0, 'f', 2)
                            .arg(statG.median(), 0, 'f', 2)
                            .arg(statB.median(), 0, 'f', 2));
    ui->editPixels->setText(QString("%L1").arg(statL.count()));
}

Statistics HistgramWidget::statistics(Channel::Color channel) const
{
    return statMap.value(channel);
}

QMap<Channel::Color, Statistics> HistgramWidget::statistics() const
{
    return statMap;
}

void HistgramWidget::setLuminanceVisible(bool visible)
{
    foreach (QAbstractSeries *series, seriesListL)
        series->setVisible(visible);
}

void HistgramWidget::setRGBVisible(bool visible)
{
    foreach (QAbstractSeries *series, seriesListRGB)
        series->setVisible(visible);
}

