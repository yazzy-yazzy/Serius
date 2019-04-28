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
    void setZoom(int percent);
    void setZoomF(qreal coeff);
    void update();

signals:
    void zoomChanged(int zoom);
    void zoomChangedF(qreal coeff);

private:
    Ui::NavigatorWidget *ui;
};

#endif // NAVIGATORWIDGET_HPP
