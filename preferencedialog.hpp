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

    QString style() const;
    void setStyle(const QString &key);

    QString language() const;
    void setLanguage(const QString &lang);

private:
    Ui::PreferenceDialog *ui;
};

#endif // PREFERENCEDIALOG_HPP
