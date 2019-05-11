#ifndef VERSIONDIALOG_HPP
#define VERSIONDIALOG_HPP

#include "memorizedialog.hpp"

namespace Ui {
class VersionDialog;
}

class VersionDialog : public MemorizeDialog
{
    Q_OBJECT

public:
    explicit VersionDialog(QWidget *parent = nullptr);
    ~VersionDialog();

private:
    Ui::VersionDialog *ui;
};

#endif // VERSIONDIALOG_HPP
