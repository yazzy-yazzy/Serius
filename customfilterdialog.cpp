#include "customfilterdialog.hpp"
#include "ui_customfilterdialog.h"

CustomFilterDialog::CustomFilterDialog(QWidget *parent) :
    MemorizeDialog(parent),
    ui(new Ui::CustomFilterDialog)
{
    ui->setupUi(this);

    clear();
}

CustomFilterDialog::~CustomFilterDialog()
{
    delete ui;
}

Kernel CustomFilterDialog::kernel() const
{
    Kernel kernel;
    kernel(0, 0) = ui->spinBox11->value();
    kernel(1, 0) = ui->spinBox21->value();
    kernel(2, 0) = ui->spinBox31->value();
    kernel(3, 0) = ui->spinBox41->value();
    kernel(4, 0) = ui->spinBox51->value();
    kernel(0, 1) = ui->spinBox12->value();
    kernel(1, 1) = ui->spinBox22->value();
    kernel(2, 1) = ui->spinBox32->value();
    kernel(3, 1) = ui->spinBox42->value();
    kernel(4, 1) = ui->spinBox52->value();
    kernel(0, 2) = ui->spinBox13->value();
    kernel(1, 2) = ui->spinBox23->value();
    kernel(2, 2) = ui->spinBox33->value();
    kernel(3, 2) = ui->spinBox43->value();
    kernel(4, 2) = ui->spinBox53->value();
    kernel(0, 3) = ui->spinBox14->value();
    kernel(1, 3) = ui->spinBox24->value();
    kernel(2, 3) = ui->spinBox34->value();
    kernel(3, 3) = ui->spinBox44->value();
    kernel(4, 3) = ui->spinBox54->value();
    kernel(0, 4) = ui->spinBox15->value();
    kernel(1, 4) = ui->spinBox25->value();
    kernel(2, 4) = ui->spinBox35->value();
    kernel(3, 4) = ui->spinBox45->value();
    kernel(4, 4) = ui->spinBox55->value();

    kernel.setScale(ui->scaleSpinBox->value());
    kernel.setOffset(ui->offsetSpinBox->value());

    return kernel;
}

void CustomFilterDialog::setKernel(const Kernel &kernel)
{
    ui->spinBox11->setValue(kernel(0, 0));
    ui->spinBox21->setValue(kernel(1, 0));
    ui->spinBox31->setValue(kernel(2, 0));
    ui->spinBox41->setValue(kernel(3, 0));
    ui->spinBox51->setValue(kernel(4, 0));
    ui->spinBox12->setValue(kernel(0, 1));
    ui->spinBox22->setValue(kernel(1, 1));
    ui->spinBox32->setValue(kernel(2, 1));
    ui->spinBox42->setValue(kernel(3, 1));
    ui->spinBox52->setValue(kernel(4, 1));
    ui->spinBox13->setValue(kernel(0, 2));
    ui->spinBox23->setValue(kernel(1, 2));
    ui->spinBox33->setValue(kernel(2, 2));
    ui->spinBox43->setValue(kernel(3, 2));
    ui->spinBox53->setValue(kernel(4, 2));
    ui->spinBox14->setValue(kernel(0, 3));
    ui->spinBox24->setValue(kernel(1, 3));
    ui->spinBox34->setValue(kernel(2, 3));
    ui->spinBox44->setValue(kernel(3, 3));
    ui->spinBox54->setValue(kernel(4, 3));
    ui->spinBox15->setValue(kernel(0, 4));
    ui->spinBox25->setValue(kernel(1, 4));
    ui->spinBox35->setValue(kernel(2, 4));
    ui->spinBox45->setValue(kernel(3, 4));
    ui->spinBox55->setValue(kernel(4, 4));

    ui->scaleSpinBox->setValue(kernel.scale());
    ui->offsetSpinBox->setValue(kernel.offset());
}

void CustomFilterDialog::clear()
{
    ui->spinBox11->clear();
    ui->spinBox21->clear();
    ui->spinBox31->clear();
    ui->spinBox41->clear();
    ui->spinBox51->clear();
    ui->spinBox12->clear();
    ui->spinBox22->clear();
    ui->spinBox32->clear();
    ui->spinBox42->clear();
    ui->spinBox52->clear();
    ui->spinBox13->clear();
    ui->spinBox23->clear();
    ui->spinBox33->clear();
    ui->spinBox43->clear();
    ui->spinBox53->clear();
    ui->spinBox14->clear();
    ui->spinBox24->clear();
    ui->spinBox34->clear();
    ui->spinBox44->clear();
    ui->spinBox54->clear();
    ui->spinBox15->clear();
    ui->spinBox25->clear();
    ui->spinBox35->clear();
    ui->spinBox45->clear();
    ui->spinBox55->clear();

    ui->scaleSpinBox->clear();
    ui->offsetSpinBox->clear();
}
