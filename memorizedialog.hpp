#ifndef MEMORIZEDIALOG_HPP
#define MEMORIZEDIALOG_HPP

#include <QDialog>

class MemorizeDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MemorizeDialog(QWidget *parent = nullptr);
    ~MemorizeDialog() override;

protected:
    void showEvent(QShowEvent * event) override;
    void done(int r) override;

protected:
    virtual void readSettings();
    virtual void writeSettings();
};

#endif // MEMORIZEDIALOG_HPP
