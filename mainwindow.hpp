#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

QT_FORWARD_DECLARE_CLASS(QGraphicsScene);
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
    void toneCurve();
    void preference();

    void updateView();
    void updateAction();
    void updateROI(const QRectF &sceneRect);
    void updateChannel(int state);
    void enter(const QPointF &scenePos);
    void leave();
    void updateDock();

private:
    void readSettings();
    void writeSettings();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QImage image;
    QRectF recentSceneRect;
    QLabel *statusLLabel;
    QLabel *statusRLabel;
};

#endif // MAINWINDOW_HPP