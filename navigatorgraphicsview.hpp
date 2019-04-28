#ifndef NAVIGATORGRAPHICSVIEW_HPP
#define NAVIGATORGRAPHICSVIEW_HPP

#include <QGraphicsView>

QT_FORWARD_DECLARE_CLASS(QRubberBand);

class NavigatorGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit NavigatorGraphicsView(QWidget *parent = nullptr);

public slots:
    void drawViewport(const QRectF &sceneRect);

signals:
    void viewportChanged(const QRectF &sceneRect);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QRubberBand *rubberBand;
    bool nowDrag;
    QPoint offset;
};

#endif // NAVIGATORGRAPHICSVIEW_HPP
