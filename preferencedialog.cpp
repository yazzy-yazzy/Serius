#include "preferencedialog.hpp"
#include "ui_preferencedialog.h"

#include <QtWidgets>

PreferenceDialog::PreferenceDialog(QWidget *parent) :
    MemorizeDialog(parent),
    ui(new Ui::PreferenceDialog)
{
    ui->setupUi(this);

    ui->styleComboBox->addItems(QStyleFactory::keys());
    ui->styleComboBox->setCurrentText(currentStyleKey());

    connect(ui->styleComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateStyle(int)));
}

PreferenceDialog::~PreferenceDialog()
{
    delete ui;
}

void PreferenceDialog::updateStyle(int index)
{
    QStyle *style = QStyleFactory::create(ui->styleComboBox->itemText(index));

    qApp->setPalette(style->standardPalette());
    qApp->setStyleSheet("");
    qApp->setStyle(style);
}

QString PreferenceDialog::currentStyleKey() const
{
    foreach (QString key, QStyleFactory::keys()) {
        QStyle *style = QStyleFactory::create(key);
        if (style->objectName() == QApplication::style()->objectName())
            return key;
    }
    return QString("");
}
