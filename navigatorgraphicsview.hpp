#ifndef NAVIGATORGRAPHICSVIEW_HPP
#define NAVIGATORGRAPHICSVIEW_HPP

#include <QGraphicsView>

QT_FORWARD_DECLARE_CLASS(QRubberBand);

class NavigatorGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit NavigatorGraphicsView(QWidget *parent = nullptr);

    QRubberBand *rubberBand() const;

public slots:
    void drawROI(const QRectF &roi);
    void fitToWindow(const QImage &image);

signals:
    void roiChanged(const QRectF &roi);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QRubberBand *_rubberBand;
    bool _nowDrag;
    QPoint _offset;
};

#endif // NAVIGATORGRAPHICSVIEW_HPP
