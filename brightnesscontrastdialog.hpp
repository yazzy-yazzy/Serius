#ifndef BRIGHTNESSCONTRASTDIALOG_HPP
#define BRIGHTNESSCONTRASTDIALOG_HPP

#include "memorizedialog.hpp"

namespace Ui {
class BrightnessContrastDialog;
}

class BrightnessContrastDialog : public MemorizeDialog
{
    Q_OBJECT

public:
    explicit BrightnessContrastDialog(QWidget *parent = nullptr);
    ~BrightnessContrastDialog();

    int brightness() const;
    int contrast() const;

public slots:
    void clear();
    void setBrightness(int value);
    void setContrast(int value);

private:
    Ui::BrightnessContrastDialog *ui;
};

#endif // BRIGHTNESSCONTRASTDIALOG_HPP
