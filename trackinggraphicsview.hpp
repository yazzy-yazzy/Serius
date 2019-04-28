#ifndef TRACKINGGRAPHICSVIEW_HPP
#define TRACKINGGRAPHICSVIEW_HPP

#include <QGraphicsView>

class TrackingGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit TrackingGraphicsView(QWidget *parent = nullptr);

    void scaleEx(qreal s);

signals:
    void viewportChanged(const QRectF &sceneRect);
    void scaleChanged(qreal s);
    void entered(const QPointF &scenePos);
    void leaved();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    bool event(QEvent *event) override;

private:
    QRectF recentSceneRect;
};

#endif // TRACKINGGRAPHICSVIEW_HPP
