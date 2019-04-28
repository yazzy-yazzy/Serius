#ifndef NAVIGATORRUBBERBAND_HPP
#define NAVIGATORRUBBERBAND_HPP

#include <QRubberBand>

class NavigatorRubberBand : public QRubberBand
{
    Q_OBJECT
public:
    explicit NavigatorRubberBand(Shape s, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // NAVIGATORRUBBERBAND_HPP
