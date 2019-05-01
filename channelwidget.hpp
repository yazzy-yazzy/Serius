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

    int state() const;

    bool contains(int state, Channel::Color color) const;

public slots:
    void select(int state);
    void selectAll();

signals:
    void stateChanged(int state);

private slots:
    void updateState();

private:
    Ui::ChannelWidget *ui;

    QCheckBox *r;
    QCheckBox *g;
    QCheckBox *b;
};

#endif // CHANNELWIDGET_HPP
