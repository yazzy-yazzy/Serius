#include "canvassizedialog.hpp"
#include "ui_canvassizedialog.h"

#include <QtWidgets>

CanvasSizeDialog::CanvasSizeDialog(QWidget *parent) :
    MemorizeDialog(parent),
    ui(new Ui::CanvasSizeDialog),
    _image(nullptr)
{
    ui->setupUi(this);

//    ui->widthUnitComboBox->addItem("%");
    ui->widthUnitComboBox->addItem("pixels");

//    ui->heightUnitComboBox->addItem("%");
    ui->heightUnitComboBox->addItem("pixels");

    ui->colorComboBox->addItem(tr("Foreground"));
    ui->colorComboBox->addItem(tr("Background"));
    ui->colorComboBox->addItem(tr("White"));
    ui->colorComboBox->addItem(tr("Black"));
    ui->colorComboBox->addItem(tr("Gray"));
    ui->colorComboBox->addItem(tr("Other..."));

    connect(ui->widthSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &CanvasSizeDialog::updateNewSize);
    connect(ui->heightSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &CanvasSizeDialog::updateNewSize);
}

CanvasSizeDialog::~CanvasSizeDialog()
{
    delete ui;
}

void CanvasSizeDialog::setImage(const QImage *image)
{
    if (!image)
        return;

    _image = image;

    qint64 imageSize = image->bitPlaneCount() * image->width() * image->height();

//    QString valueText = locale().formattedDataSize(imageSize);
    const QString currentSizeInfo = tr("Current Size: %1").arg(humanReadableSize(imageSize));
    ui->currentSizeGroup->setTitle(currentSizeInfo);
    ui->currentWidthLabel->setText(QString::number(image->width()));
    ui->currentHeightLabel->setText(QString::number(image->height()));

    ui->widthSpinBox->setValue(image->width());
    ui->heightSpinBox->setValue(image->height());
}

Anchor CanvasSizeDialog::anchor() const
{
    return ui->anchorWidget->anchor();
}

QSize CanvasSizeDialog::newSize() const
{
    return QSize(ui->widthSpinBox->value(), ui->heightSpinBox->value());
}

void CanvasSizeDialog::setAnchor(Anchor anchor)
{
    ui->anchorWidget->setAnchor(anchor);
}

bool CanvasSizeDialog::relative() const
{
    return ui->relativeCheckBox->isChecked();
}

void CanvasSizeDialog::setRelative(bool relative)
{
    return ui->relativeCheckBox->setChecked(relative);
}

void CanvasSizeDialog::setNewSize(const QSize &size)
{
    ui->widthSpinBox->setValue(size.width());
    ui->heightSpinBox->setValue(size.height());
}


#include <QFileInfo>

QString CanvasSizeDialog::humanReadableSize(qint64 size) const
{
    //
    // {https://stackoverflow.com/questions/30958043/qt-easiest-way-to-convert-int-to-file-size-format-kb-mb-or-gb}
    //
    qreal num = size;
    QStringList list;
    list << "KB" << "MB" << "GB" << "TB";

    QStringListIterator i(list);
    QString unit("bytes");

    while(num >= 1024.0 && i.hasNext()) {
        unit = i.next();
        num /= 1024.0;
    }

    return QString().setNum(num,'f',2) + " " + unit;
}

void CanvasSizeDialog::updateNewSize()
{
    if (!_image)
        return;

    qint64 imageSize = _image->bitPlaneCount() * ui->widthSpinBox->value() * ui->heightSpinBox->value();

    const QString newSizeInfo = tr("New Size: %1").arg(humanReadableSize(imageSize));
    ui->newSizeGroup->setTitle(newSizeInfo);
}
