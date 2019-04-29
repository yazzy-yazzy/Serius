#ifndef CHANNELWIDGET_HPP
#define CHANNELWIDGET_HPP

#include <QWidget>

QT_FORWARD_DECLARE_CLASS(QCheckBox);

namespace Ui {
class ChannelWidget;
}

namespace Channel {
    enum Color {
        red = 0x1,
        green = 0x2,
        blue = 0x4,
        cyan = 0x10,
        yellow = 0x20,
        magenta = 0x40,
        black = 0x80
    };
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
