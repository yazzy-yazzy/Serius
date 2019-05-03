#ifndef HISTGRAMWIDGET_HPP
#define HISTGRAMWIDGET_HPP

#include <QWidget>
#include <QtCharts>
#include <QList>
#include <QMap>

#include "statistics.hpp"
#include "channel.hpp"

namespace Ui {
class HistgramWidget;
}

class HistgramWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HistgramWidget(QWidget *parent = nullptr);
    ~HistgramWidget();

    Statistics statistics(Channel::Color) const;
    QMap<Channel::Color, Statistics> statistics() const;

public slots:
    void draw(const QImage &image);
    void clear();

private slots:
    void setLuminanceVisible(bool visible);
    void setRGBVisible(bool visible);

private:
    void createChart();
    void scan(const QImage &image);
    void drawLuminance();
    void drawRGB();
    void drawLuminanceText();
    void drawRGBText();

private:
    Ui::HistgramWidget *ui;

    QChart *chart;
    QValueAxis *axisX;
    QValueAxis *axisY;

    QMap<Channel::Color, Statistics> statMap;

    QList<QAbstractSeries *> seriesListL;
    QList<QAbstractSeries *> seriesListRGB;
};

#endif // HISTGRAMWIDGET_HPP
