#include "memorizedialog.hpp"

#include <QtWidgets>

MemorizeDialog::MemorizeDialog(QWidget *parent) : QDialog(parent)
{
}

MemorizeDialog::~MemorizeDialog()
{
}

void MemorizeDialog::showEvent(QShowEvent * event)
{
    readSettings();
    event->accept();
}

void MemorizeDialog::done(int r)
{
    writeSettings();
    QDialog::done(r);
}

void MemorizeDialog::readSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value(tr("%1/geometry").arg(objectName()), QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        move((availableGeometry.width() - width()) / 2, (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }
}

void MemorizeDialog::writeSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue(tr("%1/geometry").arg(objectName()), saveGeometry());
}
