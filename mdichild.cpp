#include "mdichild.hpp"

#include <QtWidgets>

#include "brightnesscontrastdialog.hpp"
#include "tonecurvedialog.hpp"
#include "utility.hpp"

MdiChild::MdiChild(QWidget *parent) : TrackingGraphicsView(parent)
{
    _pixmapItem = new AdjustableGraphicsPixmapItem();
    _pixmapItem->setTransformationMode(Qt::SmoothTransformation);

    _scene = new QGraphicsScene(this);
    _scene->addItem(_pixmapItem);

    setScene(_scene);

    setWindowModified(false);
    setVisible(false);
}

QString MdiChild::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

const QImage &MdiChild::image() const
{
    return _image;
}

void MdiChild::setZoomF(qreal factor)
{
    resetMatrix();
    scaleEx(factor);
}

void MdiChild::zoomIn()
{
    scaleEx(1.2);
}

void MdiChild::zoomOut()
{
    scaleEx(0.8);
}

void MdiChild::zoomMag()
{
    resetMatrix();
    scaleEx(1.0);
}

void MdiChild::fitToWindow()
{
    qreal factorX = static_cast<qreal>(width()) / static_cast<qreal>(_image.width());
    qreal factorY = static_cast<qreal>(height()) / static_cast<qreal>(_image.height());
    qreal factor = qMin(factorX, factorY);

    resetMatrix();
    scaleEx(factor);
}

void MdiChild::brightnessContrast()
{
    BrightnessContrastDialog dialog;
    dialog.setBrightness(_pixmapItem->brightness());
    dialog.setContrast(_pixmapItem->contrast());

    if (QDialog::Accepted == dialog.exec()) {
        _pixmapItem->setBrightness(dialog.brightness());
        _pixmapItem->setContrast(dialog.contrast());
        _pixmapItem->redraw();

//        ui->histgramWidget->draw(pixmapItem->pixmap().toImage());
    }
}

void MdiChild::toneCurve()
{
    ToneCurveDialog dialog;
    dialog.setToneCurves(_pixmapItem->toneCurves());
    dialog.setHistgrams(_pixmapItem->statistics());

//    connect(&dialog, &ToneCurveDialog::curveChanged, this, &MdiChild::updatePreview, Qt::QueuedConnection);

    if (QDialog::Accepted == dialog.exec()) {
        _pixmapItem->setToneCurves(dialog.points());
        _pixmapItem->redraw();
    }
}

//void MdiChild::updatePreview(const QMap<Channel::Color, QList<QPointF>> &map)
//{
//    _pixmapItem->setToneCurves(map);
//    _pixmapItem->redraw();
//}

bool MdiChild::loadFile(const QString &filename)
{
    QImageReader reader(filename);
    reader.setAutoTransform(false);

    _image = reader.read();
    if (_image.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2")
                                 .arg(QDir::toNativeSeparators(filename), reader.errorString()));
        return false;
    }

    _pixmapItem->clear();
    _pixmapItem->setImage(&_image);
    _pixmapItem->redraw();
    _pixmapItem->setVisible(true);

    _scene->setSceneRect(0, 0, _image.width(), _image.height());

    setWindowFilePath(filename);
    setWindowTitle(strippedName(filename));

    fitToWindow();

    return true;
}

bool MdiChild::saveFile(const QString &filename)
{
    QImageWriter writer(filename);

    QImage image = pixmapItem()->pixmap().toImage();
    if (!writer.write(image)) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot write %1: %2")
                                 .arg(QDir::toNativeSeparators(filename)), writer.errorString());
        return false;
    }

    return true;
}

void MdiChild::ensureVisible(const QRectF &rect)
{
//    QGraphicsView::ensureVisible(rect);

    QPointF delta = roi().topLeft() - rect.topLeft();
    if (delta.manhattanLength() > 10.0)
        QGraphicsView::ensureVisible(rect, 0, 0);
}

const AdjustableGraphicsPixmapItem *MdiChild::pixmapItem() const
{
    return _pixmapItem;
}

qreal MdiChild::zoomF() const
{
    return matrix().m11();
}
