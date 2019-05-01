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

    void insert(Channel::Color channel, const Statistics &stat);
    void setVisible(bool visible) override;

    QList<QPointF> points(Channel::Color) const;

public slots:
    void accept() override;

private slots:
    void updatePoint(const QPointF &point);
    void updateChannel(int index);

private:
    void createAction();
    void psave();
    void prestore();

private:
    Ui::ToneCurveDialog *ui;

    QShortcut *deleteShortcut;
    QMap<Channel::Color, Statistics> statMap;
    QMap<Channel::Color, QColor> colorMap;
    QMap<Channel::Color, QList<QPointF>> pointsMap;
};

#endif // TONECURVEDIALOG_HPP
