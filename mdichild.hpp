#ifndef MDICHILD_HPP
#define MDICHILD_HPP

#include <QMap>

#include "adjustablegraphicspixmapitem.hpp"
#include "channel.hpp"
#include "trackinggraphicsview.hpp"
#include "statistics.hpp"

QT_FORWARD_DECLARE_CLASS(QGraphicsScene);

class MdiChild : public TrackingGraphicsView
{
    Q_OBJECT

public:
    explicit MdiChild(QWidget *parent = nullptr);
    bool loadFile(const QString &filename);
    bool saveFile(const QString &filename);

    const QImage &image() const;
    const AdjustableGraphicsPixmapItem *pixmapItem() const;
    qreal zoomF() const;

public slots:
    void setZoomF(qreal factor);
    void zoomIn();
    void zoomOut();
    void zoomMag();
    void fitToWindow();
    void brightnessContrast();
    void toneCurve();

    void ensureVisible(const QRectF &roi);

signals:

private:
    QString strippedName(const QString &fullFileName);

private:
    QImage _image;
    QGraphicsScene *_scene;
    AdjustableGraphicsPixmapItem *_pixmapItem;
};

#endif // MDICHILD_HPP
