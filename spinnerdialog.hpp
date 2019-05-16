#ifndef SPINNERDIALOG_HPP
#define SPINNERDIALOG_HPP

#include <QDialog>

class SpinnerDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SpinnerDialog(QWidget *parent = nullptr);
};

#endif // SPINNERDIALOG_HPP
