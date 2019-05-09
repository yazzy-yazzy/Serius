#ifndef RECTANGLEGRAPHICSVIEW_HPP
#define RECTANGLEGRAPHICSVIEW_HPP

#include <QGraphicsView>

class RectangleGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit RectangleGraphicsView(QWidget *parent = nullptr);

    QPen pen() const;

public slots:
    void drawRectangle(const QRect &roi);
    void setPen(QPen pen);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QRect recentSceneRect;
    QRect recentDrawRect;
    QPen recentPen;
};

#endif // RECTANGLEGRAPHICSVIEW_HPP
