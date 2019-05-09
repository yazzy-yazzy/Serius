#ifndef HISTGRAMWIDGET_HPP
#define HISTGRAMWIDGET_HPP

#include <QWidget>
#include <QtCharts>
#include <QList>

#include "channel.hpp"
#include "statistics.hpp"

namespace Ui {
class HistgramWidget;
}

class HistgramWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HistgramWidget(QWidget *parent = nullptr);
    ~HistgramWidget();

public slots:
    void draw(const QMap<Channel::Color, Statistics> &statMap);
    void clear();

private slots:
    void setLuminanceVisible(bool visible);
    void setRGBVisible(bool visible);

private:
    void createChart();
    void drawLuminance(const QMap<Channel::Color, Statistics> &statMap);
    void drawRGB(const QMap<Channel::Color, Statistics> &statMap);
    void drawLuminanceText(const QMap<Channel::Color, Statistics> &statMap);
    void drawRGBText(const QMap<Channel::Color, Statistics> &statMap);

private:
    Ui::HistgramWidget *ui;

    QChart *chart;
    QValueAxis *axisX;
    QValueAxis *axisY;

    QList<QAbstractSeries *> seriesListL;
    QList<QAbstractSeries *> seriesListRGB;
};

#endif // HISTGRAMWIDGET_HPP
