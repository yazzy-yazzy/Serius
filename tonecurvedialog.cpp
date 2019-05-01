#include "tonecurvedialog.hpp"
#include "ui_tonecurvedialog.h"

const char *PROP_KEY = "PreviousChannelIndex";

ToneCurveDialog::ToneCurveDialog(QWidget *parent) :
    MemorizeDialog(parent),
    ui(new Ui::ToneCurveDialog)
{
    ui->setupUi(this);

    ui->channelComboBox->addItem(tr("RGB"), Channel::rgb);
    ui->channelComboBox->addItem(tr("Red"), Channel::red);
    ui->channelComboBox->addItem(tr("Green"), Channel::green);
    ui->channelComboBox->addItem(tr("Blue"), Channel::blue);

    colorMap.insert(Channel::rgb, QColor(Qt::gray));
    colorMap.insert(Channel::red, QColor(Qt::red));
    colorMap.insert(Channel::green, QColor(Qt::green));
    colorMap.insert(Channel::blue, QColor(Qt::blue));

    createAction();

    connect(ui->baselineCheckBox, &QCheckBox::toggled, ui->widget, &ToneCurveWidget::setBaselineVisible);
    connect(ui->histgramCheckBox, &QCheckBox::toggled, ui->widget, &ToneCurveWidget::setHistgramVisible);
    connect(ui->widget, &ToneCurveWidget::selectionChanged, this, &ToneCurveDialog::updatePoint);
    connect(ui->channelComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateChannel(int)));
    connect(ui->resetButton, &QPushButton::clicked, ui->widget, &ToneCurveWidget::clear);
}

ToneCurveDialog::~ToneCurveDialog()
{
    delete ui;
}

void ToneCurveDialog::createAction()
{
    deleteShortcut = new QShortcut(QKeySequence::Delete, this);
    setShortcutEnabled(deleteShortcut->id(), true);

    connect(deleteShortcut, &QShortcut::activated, ui->widget, &ToneCurveWidget::removeCurrentPoint);
}

void ToneCurveDialog::updatePoint(const QPointF &point)
{
    if (point.isNull()) {
        ui->inputEdit->clear();
        ui->outputEdit->clear();
    } else {
        ui->inputEdit->setText(QString(tr("%1").arg(point.toPoint().x())));
        ui->outputEdit->setText(QString(tr("%1").arg(point.toPoint().y())));
    }
}

void ToneCurveDialog::psave()
{
    bool success = false;
    int previousIndex = property(PROP_KEY).toInt(&success);
    if (success) {
        Channel::Color channel = ui->channelComboBox->itemData(previousIndex).value<Channel::Color>();
        pointsMap.insert(channel, ui->widget->points());
    }
}

void ToneCurveDialog::prestore()
{
    Channel::Color channel = ui->channelComboBox->currentData().value<Channel::Color>();
    ui->widget->setPoints(pointsMap.value(channel, QList<QPointF>()));

    setProperty(PROP_KEY, ui->channelComboBox->currentIndex());
}

void ToneCurveDialog::updateChannel(int index)
{
    psave();

    Channel::Color channel = ui->channelComboBox->itemData(index).value<Channel::Color>();
    ui->widget->setToneCurveColor(colorMap.value(channel, QColor(Qt::gray)));
    ui->widget->setHistgram(statMap.value(channel, Statistics()));

    prestore();
}

void ToneCurveDialog::insert(Channel::Color channel, const Statistics &stat)
{
    statMap.insert(channel, stat);
}

void ToneCurveDialog::setVisible(bool visible)
{
    if (visible)
        updateChannel(0);   //TBD

    QDialog::setVisible(visible);
}

QList<QPointF> ToneCurveDialog::points(Channel::Color color) const
{
    return pointsMap.value(color, QList<QPointF>());
}

void ToneCurveDialog::accept()
{
    psave();

    QDialog::accept();
}
