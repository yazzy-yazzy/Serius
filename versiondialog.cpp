#include "versiondialog.hpp"
#include "ui_versiondialog.h"

VersionDialog::VersionDialog(QWidget *parent) :
    MemorizeDialog(parent),
    ui(new Ui::VersionDialog)
{
    ui->setupUi(this);

    ui->titleLabel->setText(QCoreApplication::applicationName());
    ui->versionLabel->setText(tr("Version: ") + QCoreApplication::applicationVersion());
    ui->copyrightLabel->setText(tr("(C) 2019 ") + QCoreApplication::organizationName());

    setWindowTitle(tr("About"));
}

VersionDialog::~VersionDialog()
{
    delete ui;
}
