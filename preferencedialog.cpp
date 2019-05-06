#include "preferencedialog.hpp"
#include "ui_preferencedialog.h"

#include <QtWidgets>

PreferenceDialog::PreferenceDialog(QWidget *parent) :
    MemorizeDialog(parent),
    ui(new Ui::PreferenceDialog)
{
    ui->setupUi(this);

    ui->styleComboBox->addItems(QStyleFactory::keys());

    ui->languageComboBox->addItem(tr("en"), "en");
    ui->languageComboBox->addItem(tr("ja"), "ja");

    connect(ui->styleComboBox, QOverload<int>::of(&QComboBox::activated), [=](int index) {
        Q_UNUSED(index);
        QMessageBox::information(this, tr("Restart Required"), tr("The style change will take effect after restart."));
    });
    connect(ui->languageComboBox, QOverload<int>::of(&QComboBox::activated), [=](int index) {
        Q_UNUSED(index);
        QMessageBox::information(this, tr("Restart Required"), tr("The language change will take effect after restart."));
    });
}

PreferenceDialog::~PreferenceDialog()
{
    delete ui;
}

QString PreferenceDialog::style() const
{
    return ui->styleComboBox->currentText();
}

void PreferenceDialog::setStyle(const QString &key)
{
    ui->styleComboBox->setCurrentText(key);
}

QString PreferenceDialog::language() const
{
    return ui->languageComboBox->currentData().toString();
}

void PreferenceDialog::setLanguage(const QString &lang)
{
    ui->languageComboBox->setCurrentIndex(ui->languageComboBox->findData(lang));
}
