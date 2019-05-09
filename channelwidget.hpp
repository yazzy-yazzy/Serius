#ifndef CHANNELWIDGET_HPP
#define CHANNELWIDGET_HPP

#include <QWidget>
#include "channel.hpp"

QT_FORWARD_DECLARE_CLASS(QCheckBox);

namespace Ui {
class ChannelWidget;
}

class ChannelWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChannelWidget(QWidget *parent = nullptr);
    ~ChannelWidget();

    QMap<Channel::Color, bool> state() const;

public slots:
    void select(const QMap<Channel::Color, bool> &map);
    void selectAll();

signals:
    void stateChanged(const QMap<Channel::Color, bool> &map);

private slots:
    void updateState();

private:
    Ui::ChannelWidget *ui;

    QCheckBox *r;
    QCheckBox *g;
    QCheckBox *b;
};

#endif // CHANNELWIDGET_HPP
