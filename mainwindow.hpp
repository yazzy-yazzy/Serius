#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QMap>
#include "mdichild.hpp"

QT_FORWARD_DECLARE_CLASS(QGraphicsScene);
QT_FORWARD_DECLARE_CLASS(AdjustableGraphicsPixmapItem);

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
    void zoom(qreal factor);
    void zoomIn();
    void zoomOut();
    void zoomMag();
    void fitToWindow();
    void brightnessContrast();
    void toneCurve();
    void preference();
    void about();

    void updateAction();
    void updateStatusBar();
    void updateHistgram();
    void updateNavigator();
    void updateChannel();
//    void updateROI(const QRectF &sceneRect);
//    void updateChannel(int state);
//    void updatePreview(const QMap<Channel::Color, QList<QPointF>> &map);
//    void enter(const QPointF &scenePos);
//    void leave();
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
