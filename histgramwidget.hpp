#ifndef HISTGRAMWIDGET_HPP
#define HISTGRAMWIDGET_HPP

#include <QWidget>
#include <QtCharts>

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
    void draw(const QImage &image);
    void clear();

private:
    void createChart();
    void scan(const QImage &image);
    void drawLuminance();
    void drawRGB();
    void drawText();

private:
    Ui::HistgramWidget *ui;

    QChart *chart;
    QValueAxis *axisX;
    QValueAxis *axisY;

    Statistics statisticsL;
    Statistics statisticsR;
    Statistics statisticsG;
    Statistics statisticsB;
};

#endif // HISTGRAMWIDGET_HPP
