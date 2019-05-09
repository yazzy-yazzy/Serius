#ifndef TRACKINGGRAPHICSVIEW_HPP
#define TRACKINGGRAPHICSVIEW_HPP

#include <QGraphicsView>

class TrackingGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit TrackingGraphicsView(QWidget *parent = nullptr);

    void scaleEx(qreal s);
    QRectF roi() const;

signals:
    void roiChanged(const QRectF &roi);
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
    QRectF _roi;
};

#endif // TRACKINGGRAPHICSVIEW_HPP
