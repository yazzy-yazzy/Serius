#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include "mdichild.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void open();
    void saveAs();
    void preference();
    void brightnessContrast();
    void toneCurve();
    void duplicate();
    void flipHorizontal();
    void flipVertical();
    void rotateCW90();
    void rotateCCW90();
    void zoom(qreal factor);
    void zoomIn();
    void zoomOut();
    void zoomMag();
    void fitToWindow();
    void about();

    void updateAction();
    void updateStatusBar();
    void updateHistgram();
    void updateNavigator();
    void updateChannel();
    void updateDock();

private:
    void readSettings();
    void writeSettings();

    MdiChild *createMdiChild();
    MdiChild *activeMdiChild() const;
    QList<MdiChild *> deactiveMdiChilds() const;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_HPP
