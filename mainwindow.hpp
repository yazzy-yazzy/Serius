#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QMap>
#include "channel.hpp"

QT_FORWARD_DECLARE_CLASS(QGraphicsScene);
QT_FORWARD_DECLARE_CLASS(AdjustableGraphicsPixmapItem);
QT_FORWARD_DECLARE_CLASS(QLabel);

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
    void zoom(qreal coeff);
    void zoomIn();
    void zoomOut();
    void zoomMag();
    void fitToWindow();
    void brightnessContrast();
    void toneCurve();
    void preference();

    void updateView();
    void updateAction();
    void updateROI(const QRectF &sceneRect);
    void updateChannel(int state);
    void updatePreview(const QMap<Channel::Color, QList<QPointF>> &map);
    void enter(const QPointF &scenePos);
    void leave();
    void updateDock();

private:
    void readSettings();
    void writeSettings();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    AdjustableGraphicsPixmapItem *pixmapItem;
    QImage image;
    QRectF recentSceneRect;
    QLabel *statusLLabel;
    QLabel *statusRLabel;
};

#endif // MAINWINDOW_HPP
