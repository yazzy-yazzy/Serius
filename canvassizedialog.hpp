#ifndef CANVASSIZEDIALOG_HPP
#define CANVASSIZEDIALOG_HPP

#include "memorizedialog.hpp"

namespace Ui {
class CanvasSizeDialog;
}

class CanvasSizeDialog : public MemorizeDialog
{
    Q_OBJECT

public:
    explicit CanvasSizeDialog(QWidget *parent = nullptr);
    ~CanvasSizeDialog();

    void setImage(const QImage *image);

private slots:
    void updateNewSize();

private:
    QString humanReadableSize(qint64 size) const;

private:
    Ui::CanvasSizeDialog *ui;

    const QImage *_image;
};

#endif // CANVASSIZEDIALOG_HPP
