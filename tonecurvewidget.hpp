#ifndef TONECURVEWIDGET_HPP
#define TONECURVEWIDGET_HPP

#include <QWidget>
#include <QtCharts>

namespace Ui {
class ToneCurveWidget;
}

class ToneCurveWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ToneCurveWidget(QWidget *parent = nullptr);
    ~ToneCurveWidget();

public slots:
    void setBaselineVisible(bool visible);
    void setHistgramVisible(bool visible);
    void removeCurrentPoint();

private:
    void createChart();
    void createBaseline();

private:
    Ui::ToneCurveWidget *ui;

    QChart *chart;
    QValueAxis *axisHistgramX;
    QValueAxis *axisHistgramY;
    QValueAxis *axisToneCurveX;
    QValueAxis *axisToneCurveY;

    QSplineSeries *baselineSeries;
};

#endif // TONECURVEWIDGET_HPP
