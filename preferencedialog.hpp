#ifndef PREFERENCEDIALOG_HPP
#define PREFERENCEDIALOG_HPP

#include "memorizedialog.hpp"

namespace Ui {
class PreferenceDialog;
}

class PreferenceDialog : public MemorizeDialog
{
    Q_OBJECT

public:
    explicit PreferenceDialog(QWidget *parent = nullptr);
    ~PreferenceDialog();

private slots:
    void updateStyle(int index);

private:
    QString currentStyleKey() const;

private:
    Ui::PreferenceDialog *ui;
};

#endif // PREFERENCEDIALOG_HPP
