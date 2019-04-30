#ifndef TONECURVEDIALOG_HPP
#define TONECURVEDIALOG_HPP

#include "memorizedialog.hpp"

QT_FORWARD_DECLARE_CLASS(QShortcut);

namespace Ui {
class ToneCurveDialog;
}

class ToneCurveDialog : public MemorizeDialog
{
    Q_OBJECT

public:
    explicit ToneCurveDialog(QWidget *parent = nullptr);
    ~ToneCurveDialog();

private:
    void createAction();

private:
    Ui::ToneCurveDialog *ui;

    QShortcut *deleteShortcut;
};

#endif // TONECURVEDIALOG_HPP
