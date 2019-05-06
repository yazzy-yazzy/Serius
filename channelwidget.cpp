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

int ChannelWidget::state() const
{
    int state = r->isChecked() ? Channel::red : 0;
    state |= g->isChecked() ? Channel::green : 0;
    state |= b->isChecked() ? Channel::blue : 0;
    return state;
}

void ChannelWidget::select(int state)
{
//    QSignalBlocker blockerR(r);
//    QSignalBlocker blockerG(g);
//    QSignalBlocker blockerB(b);

    r->setChecked(contains(state, Channel::red));
    g->setChecked(contains(state, Channel::green));
    b->setChecked(contains(state, Channel::blue));
}

bool ChannelWidget::contains(int state, Channel::Color color) const
{
    return state & color;
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

