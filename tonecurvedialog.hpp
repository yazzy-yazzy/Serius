#ifndef TONECURVEDIALOG_HPP
#define TONECURVEDIALOG_HPP

#include <QMap>

#include "memorizedialog.hpp"
#include "statistics.hpp"
#include "channel.hpp"

QT_FORWARD_DECLARE_CLASS(QShortcut);

namespace Ui {
class ToneCurveDialog;
}

class ToneCurveDialog : public MemorizeDialog
{
    Q_OBJECT

public:
    explicit ToneCurveDialog(QWidget *parent = nullptr);
    ~ToneCurveDialog() override;

    void setToneCurve(Channel::Color channel, const QList<QPointF> &points);
    void setToneCurves(const QMap<Channel::Color, QList<QPointF>> &map);
    void setHistgram(Channel::Color channel, const Statistics &stat);
    void setHistgrams(const QMap<Channel::Color, Statistics> &map);
    void setVisible(bool visible) override;

    QList<QPointF> points(Channel::Color) const;
    QMap<Channel::Color, QList<QPointF>> points() const;

public slots:
    void clear();

signals:
    void curveChanged(const QMap<Channel::Color, QList<QPointF>> &map);

private slots:
    void updateSelectPoint(const QPointF &point);
    void updateChannel(int index);
    void updateCurvePoints();

private:
    void createAction();

protected:
    void readSettings() override;
    void writeSettings() override;

private:
    Ui::ToneCurveDialog *ui;

    QShortcut *deleteShortcut;
    QMap<Channel::Color, Statistics> statMap;
    QMap<Channel::Color, QColor> colorMap;
    QMap<Channel::Color, QList<QPointF>> pointsMap;
};

#endif // TONECURVEDIALOG_HPP
