#ifndef CUSTOMFILTERDIALOG_HPP
#define CUSTOMFILTERDIALOG_HPP

#include <QDialog>

#include "kernel.hpp"
#include "memorizedialog.hpp"

namespace Ui {
class CustomFilterDialog;
}

class CustomFilterDialog : public MemorizeDialog
{
    Q_OBJECT

public:
    explicit CustomFilterDialog(QWidget *parent = nullptr);
    ~CustomFilterDialog();

    Kernel kernel() const;
    void setKernel(const Kernel &kernel);

public slots:
    void clear();

private:
    Ui::CustomFilterDialog *ui;
};

#endif // CUSTOMFILTERDIALOG_HPP
