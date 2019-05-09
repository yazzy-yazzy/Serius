#include "navigatorwidget.hpp"
#include "ui_navigatorwidget.h"

#include <QDebug>

NavigatorWidget::NavigatorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NavigatorWidget)
{
    ui->setupUi(this);

    connect(ui->zoomSlider, &QSlider::valueChanged, this, &NavigatorWidget::update);

    clear();
}

NavigatorWidget::~NavigatorWidget()
{
    delete ui;
}

NavigatorGraphicsView *NavigatorWidget::view() const
{
    return ui->thumbnailView;
}

int NavigatorWidget::zoom() const
{
    return ui->zoomSlider->value();
}

qreal NavigatorWidget::zoomF() const
{
    return static_cast<qreal>(ui->zoomSlider->value()) / 100.0;
}

void NavigatorWidget::setZoom(int factor)
{
    int min = ui->zoomSlider->minimum();
    int max = ui->zoomSlider->maximum();

    ui->zoomSlider->setValue(qBound(min, factor, max));
}

void NavigatorWidget::setZoomF(qreal factor)
{
    setZoom(static_cast<int>(qRound(factor * 100)));
}

void NavigatorWidget::update()
{
    ui->zoomEdit->setText(QString("%1%").arg(ui->zoomSlider->value()));

    emit zoomChanged(zoom());
    emit zoomChangedF(zoomF());
}

void NavigatorWidget::clear()
{
    QSignalBlocker blocker(ui->zoomSlider);

    ui->zoomEdit->clear();
    ui->zoomSlider->setValue(ui->zoomSlider->minimum());
}
