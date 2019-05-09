#include "channelwidget.hpp"
#include "ui_channelwidget.h"

#include <QtWidgets>

ChannelWidget::ChannelWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChannelWidget)
{
    ui->setupUi(this);

    r = new QCheckBox(tr("R"));
    g = new QCheckBox(tr("G"));
    b = new QCheckBox(tr("B"));
    r->setChecked(true);
    g->setChecked(true);
    b->setChecked(true);

    QListWidgetItem *ir = new QListWidgetItem();
    QListWidgetItem *ig = new QListWidgetItem();
    QListWidgetItem *ib = new QListWidgetItem();

    ui->listWidget->addItem(ir);
    ui->listWidget->addItem(ig);
    ui->listWidget->addItem(ib);

    ui->listWidget->setItemWidget(ir, r);
    ui->listWidget->setItemWidget(ig, g);
    ui->listWidget->setItemWidget(ib, b);

    connect(r, &QCheckBox::stateChanged, this, &ChannelWidget::updateState);
    connect(g, &QCheckBox::stateChanged, this, &ChannelWidget::updateState);
    connect(b, &QCheckBox::stateChanged, this, &ChannelWidget::updateState);

    connect(ui->listWidget, &QListWidget::doubleClicked, this, &ChannelWidget::selectAll);
}

ChannelWidget::~ChannelWidget()
{
    delete ui;
}

void ChannelWidget::updateState()
{
    emit stateChanged(state());
}

QMap<Channel::Color, bool> ChannelWidget::state() const
{
    QMap<Channel::Color, bool> map;
    map.insert(Channel::red, r->isChecked());
    map.insert(Channel::green, g->isChecked());
    map.insert(Channel::blue, b->isChecked());
    return map;
}

void ChannelWidget::select(const QMap<Channel::Color, bool> &map)
{
//    QSignalBlocker blockerR(r);
//    QSignalBlocker blockerG(g);
//    QSignalBlocker blockerB(b);

    r->setChecked(map.value(Channel::red, false));
    g->setChecked(map.value(Channel::green, false));
    b->setChecked(map.value(Channel::blue, false));
}

void ChannelWidget::selectAll()
{
//    QSignalBlocker blockerR(r);
//    QSignalBlocker blockerG(g);
//    QSignalBlocker blockerB(b);

    r->setChecked(true);
    g->setChecked(true);
    b->setChecked(true);
}

