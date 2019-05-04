#include "brightnesscontrastdialog.hpp"
#include "ui_brightnesscontrastdialog.h"

BrightnessContrastDialog::BrightnessContrastDialog(QWidget *parent) :
    MemorizeDialog(parent),
    ui(new Ui::BrightnessContrastDialog)
{
    ui->setupUi(this);

    connect(ui->resetButton, &QPushButton::clicked, this, &BrightnessContrastDialog::clear);

    clear();
}

BrightnessContrastDialog::~BrightnessContrastDialog()
{
    delete ui;
}

void BrightnessContrastDialog::clear()
{
    ui->brightnessSlider->setValue(0);
    ui->contrastSlider->setValue(0);
}

int BrightnessContrastDialog::brightness() const
{
    return ui->brightnessSlider->value();
}

int BrightnessContrastDialog::contrast() const
{
    return ui->contrastSlider->value();
}

void BrightnessContrastDialog::setBrightness(int value)
{
    ui->brightnessSlider->setValue(value);
}

void BrightnessContrastDialog::setContrast(int value)
{
    ui->contrastSlider->setValue(value);
}
