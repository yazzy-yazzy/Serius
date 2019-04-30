#ifndef TRACKINGCHARTEVENT_HPP
#define TRACKINGCHARTEVENT_HPP

#include <QEvent>
#include <QPointF>

class ChartReleaseEvent : public QEvent
{
public:
    explicit ChartReleaseEvent(QPointF pos);
    const QPointF &pos() const;

    static QEvent::Type staticType();
    static bool is(const QEvent * ev);

private:
    QPointF mousePos;
    static bool initialized;
    static QEvent::Type type;
};

class ChartMoveEvent : public QEvent
{
public:
    explicit ChartMoveEvent(QPointF pos);
    const QPointF &pos() const;

    static QEvent::Type staticType();
    static bool is(const QEvent * ev);

private:
    QPointF mousePos;
    static bool initialized;
    static QEvent::Type type;
};

class ChartPressEvent : public QEvent
{
public:
    explicit ChartPressEvent(QPointF pos);
    const QPointF &pos() const;

    static QEvent::Type staticType();
    static bool is(const QEvent * ev);

private:
    QPointF mousePos;
    static bool initialized;
    static QEvent::Type type;
};

#endif // TRACKINGCHARTEVENT_HPP
