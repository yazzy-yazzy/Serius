#ifndef NAVIGATORWIDGET_HPP
#define NAVIGATORWIDGET_HPP

#include <QWidget>
#include "navigatorgraphicsview.hpp"

QT_FORWARD_DECLARE_CLASS(QGraphicsView);

namespace Ui {
class NavigatorWidget;
}

class NavigatorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NavigatorWidget(QWidget *parent = nullptr);
    ~NavigatorWidget();

    NavigatorGraphicsView *view() const;
    int zoom() const;
    qreal zoomF() const;

public slots:
    void setZoom(int factor);
    void setZoomF(qreal factor);
    void update();
    void clear();

signals:
    void zoomChanged(int factor);
    void zoomChangedF(qreal factor);

private:
    Ui::NavigatorWidget *ui;
};

#endif // NAVIGATORWIDGET_HPP
