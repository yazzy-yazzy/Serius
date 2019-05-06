#include "tonecurvedialog.hpp"
#include "ui_tonecurvedialog.h"

ToneCurveDialog::ToneCurveDialog(QWidget *parent) :
    MemorizeDialog(parent),
    ui(new Ui::ToneCurveDialog)
{
    ui->setupUi(this);

    ui->channelComboBox->addItem(tr("RGB"), Channel::luminance);
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
    connect(ui->widget, &ToneCurveWidget::selectionChanged, this, &ToneCurveDialog::updateSelectPoint);
    connect(ui->channelComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateChannel(int)));
    connect(ui->resetButton, &QPushButton::clicked, this, &ToneCurveDialog::clear);

    connect(ui->widget, &ToneCurveWidget::pointAdded, this, &ToneCurveDialog::updateCurvePoints);
    connect(ui->widget, &ToneCurveWidget::pointRemoved, this, &ToneCurveDialog::updateCurvePoints);
    connect(ui->resetButton, &QPushButton::clicked, this, &ToneCurveDialog::updateCurvePoints);
}

ToneCurveDialog::~ToneCurveDialog()
{
    delete ui;
}

void ToneCurveDialog::clear()
{
    ui->widget->clear();
    pointsMap.clear();
}

void ToneCurveDialog::createAction()
{
    deleteShortcut = new QShortcut(QKeySequence::Delete, this);
    setShortcutEnabled(deleteShortcut->id(), true);

    connect(deleteShortcut, &QShortcut::activated, ui->widget, &ToneCurveWidget::removeCurrentPoint);
}

void ToneCurveDialog::updateSelectPoint(const QPointF &point)
{
    if (point.isNull()) {
        ui->inputEdit->clear();
        ui->outputEdit->clear();
    } else {
        ui->inputEdit->setText(QString(tr("%1").arg(point.toPoint().x())));
        ui->outputEdit->setText(QString(tr("%1").arg(point.toPoint().y())));
    }
}

void ToneCurveDialog::updateChannel(int index)
{
    Channel::Color channel = ui->channelComboBox->itemData(index).value<Channel::Color>();
    ui->widget->setToneCurveColor(colorMap.value(channel, QColor(Qt::gray)));
    ui->widget->setHistgram(statMap.value(channel, Statistics()));
    ui->widget->setPoints(pointsMap.value(channel, QList<QPointF>()));
}

void ToneCurveDialog::setHistgram(Channel::Color channel, const Statistics &stat)
{
    statMap.insert(channel, stat);
}

void ToneCurveDialog::setHistgrams(const QMap<Channel::Color, Statistics> &map)
{
    statMap = map;
}

void ToneCurveDialog::setToneCurve(Channel::Color channel, const QList<QPointF> &points)
{
    pointsMap.insert(channel, points);
}

void ToneCurveDialog::setToneCurves(const QMap<Channel::Color, QList<QPointF>> &map)
{
    pointsMap = map;
}

void ToneCurveDialog::setVisible(bool visible)
{
    if (visible)
        updateChannel(ui->channelComboBox->currentIndex());

    QDialog::setVisible(visible);
}

QList<QPointF> ToneCurveDialog::points(Channel::Color color) const
{
    return pointsMap.value(color, QList<QPointF>());
}

QMap<Channel::Color, QList<QPointF>> ToneCurveDialog::points() const
{
    return pointsMap;
}

void ToneCurveDialog::readSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    bool baseline = settings.value(tr("%1/display/baseline").arg(objectName()), true).toBool();
    bool histgram = settings.value(tr("%1/display/histgram").arg(objectName()), true).toBool();
    bool preview = settings.value(tr("%1/display/preview").arg(objectName()), true).toBool();
    int channelIndex = settings.value(tr("%1/recent/channel").arg(objectName()), 0).toInt();

    ui->baselineCheckBox->setChecked(baseline);
    ui->histgramCheckBox->setChecked(histgram);
    ui->previewCheckBox->setChecked(preview);
    ui->channelComboBox->setCurrentIndex(channelIndex);

    MemorizeDialog::readSettings();
}

void ToneCurveDialog::writeSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue(tr("%1/display/baseline").arg(objectName()), ui->baselineCheckBox->isChecked());
    settings.setValue(tr("%1/display/histgram").arg(objectName()), ui->histgramCheckBox->isChecked());
    settings.setValue(tr("%1/display/preview").arg(objectName()), ui->previewCheckBox->isChecked());
    settings.setValue(tr("%1/recent/channel").arg(objectName()), ui->channelComboBox->currentIndex());

    MemorizeDialog::writeSettings();
}

void ToneCurveDialog::updateCurvePoints()
{
    Channel::Color channel = ui->channelComboBox->currentData().value<Channel::Color>();
    pointsMap.insert(channel, ui->widget->points());

//    if (ui->previewCheckBox->isChecked())
//        emit curveChanged(pointsMap);
}

