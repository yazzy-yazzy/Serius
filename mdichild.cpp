#include "mdichild.hpp"

#include <QtWidgets>
#include <QtConcurrent>

#include "brightnesscontrastdialog.hpp"
#include "customfilterdialog.hpp"
#include "canvassizedialog.hpp"
#include "tonecurvedialog.hpp"
#include "spinnerdialog.hpp"
#include "utility.hpp"

class ZoomInCommand : public QUndoCommand
{
public:
    ZoomInCommand(MdiChild *child, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    MdiChild *_child;
};

ZoomInCommand::ZoomInCommand(MdiChild *child, QUndoCommand *parent) :
    QUndoCommand(parent),
    _child(child)
{
    setText("Zoom-In");
}

void ZoomInCommand::undo()
{
    _child->scaleEx(0.8);
}

void ZoomInCommand::redo()
{
    _child->scaleEx(1.2);
}

class ZoomOutCommand : public QUndoCommand
{
public:
    ZoomOutCommand(MdiChild *child, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    MdiChild *_child;
};

ZoomOutCommand::ZoomOutCommand(MdiChild *child, QUndoCommand *parent) :
    QUndoCommand(parent),
    _child(child)
{
    setText("Zoom-Out");
}

void ZoomOutCommand::undo()
{
    _child->scaleEx(1.2);
}

void ZoomOutCommand::redo()
{
    _child->scaleEx(0.8);
}

class ZoomMagCommand : public QUndoCommand
{
public:
    ZoomMagCommand(MdiChild *child, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    MdiChild *_child;
    qreal _factor;
};

ZoomMagCommand::ZoomMagCommand(MdiChild *child, QUndoCommand *parent) :
    QUndoCommand(parent),
    _child(child),
    _factor(1.0)
{
    setText("Zoom-100%");
}

void ZoomMagCommand::undo()
{
    _child->resetMatrix();
    _child->scaleEx(_factor);
}

void ZoomMagCommand::redo()
{
    _factor = _child->matrix().m11();
    _child->resetMatrix();
    _child->scaleEx(1.0);
}

class FitToWindowCommand : public QUndoCommand
{
public:
    FitToWindowCommand(MdiChild *child, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    MdiChild *_child;
    qreal _factor;
};

FitToWindowCommand::FitToWindowCommand(MdiChild *child, QUndoCommand *parent) :
    QUndoCommand(parent),
    _child(child),
    _factor(1.0)
{
    setText("Fit to Window");
}

void FitToWindowCommand::undo()
{
    _child->resetMatrix();
    _child->scaleEx(_factor);
}

void FitToWindowCommand::redo()
{
    _factor = _child->matrix().m11();

    qreal factorX = static_cast<qreal>(_child->width()) / static_cast<qreal>(_child->image()->width());
    qreal factorY = static_cast<qreal>(_child->height()) / static_cast<qreal>(_child->image()->height());
    qreal factor = qMin(factorX, factorY);

    _child->resetMatrix();
    _child->scaleEx(factor);
}

class FlipHorizontalCommand : public QUndoCommand
{
public:
    FlipHorizontalCommand(MdiChild *child, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    MdiChild *_child;
};

FlipHorizontalCommand::FlipHorizontalCommand(MdiChild *child, QUndoCommand *parent) :
    QUndoCommand(parent),
    _child(child)
{
    setText("Flip-Horizontal");
}

void FlipHorizontalCommand::undo()
{
    _child->setImage(_child->image()->mirrored(true, false));
}

void FlipHorizontalCommand::redo()
{
    _child->setImage(_child->image()->mirrored(true, false));
}

class FlipVerticalCommand : public QUndoCommand
{
public:
    FlipVerticalCommand(MdiChild *child, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    MdiChild *_child;
};

FlipVerticalCommand::FlipVerticalCommand(MdiChild *child, QUndoCommand *parent) :
    QUndoCommand(parent),
    _child(child)
{
    setText("Flip-Vertical");
}

void FlipVerticalCommand::undo()
{
    _child->setImage(_child->image()->mirrored(false, true));
}

void FlipVerticalCommand::redo()
{
    _child->setImage(_child->image()->mirrored(false, true));
}

class RotateCW90Command : public QUndoCommand
{
public:
    RotateCW90Command(MdiChild *child, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    MdiChild *_child;
};

RotateCW90Command::RotateCW90Command(MdiChild *child, QUndoCommand *parent) :
    QUndoCommand(parent),
    _child(child)
{
    setText("Rotate90 (CW)");
}

void RotateCW90Command::undo()
{
    QTransform matrix;
    matrix.rotate(-90);

    _child->setImage(_child->image()->transformed(matrix));
    _child->parentWidget()->resize(_child->parentWidget()->size().transposed());
}

void RotateCW90Command::redo()
{
    QTransform matrix;
    matrix.rotate(90);

    _child->setImage(_child->image()->transformed(matrix));
    _child->parentWidget()->resize(_child->parentWidget()->size().transposed());
}

class RotateCCW90Command : public QUndoCommand
{
public:
    RotateCCW90Command(MdiChild *child, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    MdiChild *_child;
};

RotateCCW90Command::RotateCCW90Command(MdiChild *child, QUndoCommand *parent) :
    QUndoCommand(parent),
    _child(child)
{
    setText("Rotate90 (CCW)");
}

void RotateCCW90Command::undo()
{
    QTransform matrix;
    matrix.rotate(90);

    _child->setImage(_child->image()->transformed(matrix));
    _child->parentWidget()->resize(_child->parentWidget()->size().transposed());
}

void RotateCCW90Command::redo()
{
    QTransform matrix;
    matrix.rotate(-90);

    _child->setImage(_child->image()->transformed(matrix));
    _child->parentWidget()->resize(_child->parentWidget()->size().transposed());
}

class BrightnessContrastCommand : public QUndoCommand
{
public:
    BrightnessContrastCommand(MdiChild *child, int brightness, int contrast, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    MdiChild *_child;

    int _brightness;
    int _contrast;
    int _backupBrightness;
    int _backupContrast;
};

BrightnessContrastCommand::BrightnessContrastCommand(MdiChild *child, int brightness, int contrast, QUndoCommand *parent) :
    QUndoCommand(parent),
    _child(child),
    _brightness(brightness),
    _contrast(contrast),
    _backupBrightness(0),
    _backupContrast(0)
{
    setText("Brightness/Contrast");
}

void BrightnessContrastCommand::undo()
{
    _child->pixmapItem()->setBrightness(_backupBrightness);
    _child->pixmapItem()->setContrast(_backupContrast);
    _child->pixmapItem()->redraw();
}

void BrightnessContrastCommand::redo()
{
    _backupBrightness = _child->pixmapItem()->brightness();
    _backupContrast = _child->pixmapItem()->contrast();

    _child->pixmapItem()->setBrightness(_brightness);
    _child->pixmapItem()->setContrast(_contrast);
    _child->pixmapItem()->redraw();
}

class ToneCurveCommand : public QUndoCommand
{
public:
    ToneCurveCommand(MdiChild *child, const QMap<Channel::Color, QList<QPointF>> &points, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    MdiChild *_child;

    QMap<Channel::Color, QList<QPointF>> _points;
    QMap<Channel::Color, QList<QPointF>> _backupPoints;
};

ToneCurveCommand::ToneCurveCommand(MdiChild *child, const QMap<Channel::Color, QList<QPointF>> &points, QUndoCommand *parent) :
    QUndoCommand(parent),
    _child(child),
    _points(points)
{
    setText("ToneCurve");
}

void ToneCurveCommand::undo()
{
    _child->pixmapItem()->setToneCurves(_backupPoints);
    _child->pixmapItem()->redraw();
}

void ToneCurveCommand::redo()
{
    _backupPoints = _child->pixmapItem()->toneCurves();

    _child->pixmapItem()->setToneCurves(_points);
    _child->pixmapItem()->redraw();
}

class CanvasSizeCommand : public QUndoCommand
{
public:
    CanvasSizeCommand(MdiChild *child, const QSize &size, Anchor anchor, bool relative, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    MdiChild *_child;

    QSize _size;
    Anchor _anchor;
    bool _relative;
    QImage _backupImage;
};

CanvasSizeCommand::CanvasSizeCommand(MdiChild *child, const QSize &size, Anchor anchor, bool relative, QUndoCommand *parent) :
    QUndoCommand(parent),
    _child(child),
    _size(size),
    _anchor(anchor),
    _relative(relative),
    _backupImage(QImage())
{
    setText("CanvasSize");
}

void CanvasSizeCommand::undo()
{
    if (_backupImage.isNull())
        return;

    _child->setImage(_backupImage);
}

void CanvasSizeCommand::redo()
{
    if (!_child->pixmapItem())
        return;

    const QImage *original = _child->image();
    QRect srcRect(original->rect());
    QRect dstRect(0, 0, _size.width(), _size.height());

    switch (_anchor) {
    case Anchor::TopLeft:
        dstRect.moveTo(0, 0);
        break;
    case Anchor::Top:
        dstRect.moveTo(-(dstRect.width() / 2 - original->width() / 2), 0);
        break;
    case Anchor::TopRight:
        dstRect.moveTo(-(dstRect.width() - original->width()), 0);
        break;
    case Anchor::Left:
        dstRect.moveTo(0, -(dstRect.height() / 2 - original->height() / 2));
        break;
    case Anchor::Center:
        dstRect.moveTo(-(dstRect.width() / 2 - original->width() / 2), -(dstRect.height() / 2 - original->height() / 2));
        break;
    case Anchor::Right:
        dstRect.moveTo(-(dstRect.width() - original->width()), -(dstRect.height() / 2 - original->height() / 2));
        break;
    case Anchor::BottomLeft:
        dstRect.moveTo(0, -(dstRect.height() - original->height()));
        break;
    case Anchor::Bottom:
        dstRect.moveTo(-(dstRect.width() / 2 - original->height() / 2), -(dstRect.height() - original->height()));
        break;
    case Anchor::BottomRight:
        dstRect.moveTo(-(dstRect.width() - original->width()), -(dstRect.height() - original->height()));
        break;
    }

    srcRect = srcRect.intersected(dstRect).translated(-srcRect.x(), -srcRect.y());
    dstRect = dstRect.intersected(srcRect).translated(-dstRect.x(), -dstRect.y());

    _backupImage = original->copy();

    QImage image(_size, original->format());

    QPainter painter(&image);
    painter.fillRect(QRect(0, 0, _size.width(), _size.height()), Qt::black);    //TBD
    painter.drawImage(dstRect, *original, srcRect);

    _child->setImage(image);
}

class FilterCustomCommand : public QUndoCommand
{
public:
    FilterCustomCommand(MdiChild *child, const Kernel &kernel, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    QImage redoTask();
    QImage undoTask();

private:
    MdiChild *_child;

    Kernel _kernel;
    Kernel _backupKernel;
};

FilterCustomCommand::FilterCustomCommand(MdiChild *child, const Kernel &kernel, QUndoCommand *parent) :
    QUndoCommand(parent),
    _child(child),
    _kernel(kernel)
{
    setText("Filter(Custom)");
}

void FilterCustomCommand::undo()
{
    SpinnerDialog spinner;
    spinner.show();

    QFuture<QImage> f = QtConcurrent::run(this, &FilterCustomCommand::undoTask);
    while (true) {
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

        QEventLoop loop;
        QTimer::singleShot(10, &loop, SLOT(quit()));
        loop.exec();

        if (f.isCanceled() || f.isFinished())
            break;
    }

    f.waitForFinished();

    _child->pixmapItem()->drawPixmap(f);
}

QImage FilterCustomCommand::undoTask()
{
    _child->pixmapItem()->setKernel(_backupKernel);
    return _child->pixmapItem()->convert();
}

void FilterCustomCommand::redo()
{
    SpinnerDialog spinner;
    spinner.show();

    QFuture<QImage> f = QtConcurrent::run(this, &FilterCustomCommand::redoTask);
    while (true) {
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

        QEventLoop loop;
        QTimer::singleShot(10, &loop, SLOT(quit()));
        loop.exec();

        if (f.isCanceled() || f.isFinished())
            break;
    }

    f.waitForFinished();

    _child->pixmapItem()->drawPixmap(f);
}

QImage FilterCustomCommand::redoTask()
{
    _backupKernel = _child->pixmapItem()->kernel();

    _child->pixmapItem()->setKernel(_kernel);
    return _child->pixmapItem()->convert();
}


MdiChild::MdiChild(QWidget *parent) :
    TrackingGraphicsView(parent),
    _undoStack(new QUndoStack)
{
    _pixmapItem = new AdjustableGraphicsPixmapItem();
    _pixmapItem->setTransformationMode(Qt::SmoothTransformation);

    _scene = new QGraphicsScene(this);
    _scene->addItem(_pixmapItem);

    connect(_undoStack, SIGNAL(canUndoChanged(bool)), this, SIGNAL(canUndoChanged(bool)));
    connect(_undoStack, SIGNAL(canRedoChanged(bool)), this, SIGNAL(canRedoChanged(bool)));

    setScene(_scene);

    setWindowModified(false);
    setVisible(false);
}

QString MdiChild::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MdiChild::setZoomF(qreal factor)
{
    resetMatrix();
    scaleEx(factor);
}

void MdiChild::zoomIn()
{
    undoStack()->push(new ZoomInCommand(this));
}

void MdiChild::zoomOut()
{
    undoStack()->push(new ZoomOutCommand(this));
}

void MdiChild::zoomMag()
{
    undoStack()->push(new ZoomMagCommand(this));
}

void MdiChild::fitToWindow()
{
    undoStack()->push(new FitToWindowCommand(this));
}

void MdiChild::brightnessContrast()
{
    BrightnessContrastDialog dialog;
    dialog.setBrightness(_pixmapItem->brightness());
    dialog.setContrast(_pixmapItem->contrast());

    if (QDialog::Accepted == dialog.exec())
        undoStack()->push(new BrightnessContrastCommand(this, dialog.brightness(), dialog.contrast()));
}

void MdiChild::toneCurve()
{
    ToneCurveDialog dialog;
    dialog.setToneCurves(_pixmapItem->toneCurves());
    dialog.setHistgrams(_pixmapItem->statistics());

    const QMap<Channel::Color, QList<QPointF>> backupToneTurves = _pixmapItem->toneCurves();

    connect(&dialog, &ToneCurveDialog::curveChanged, [=](const QMap<Channel::Color, QList<QPointF>> &points) {
        _pixmapItem->setToneCurves(points);
        _pixmapItem->redraw();
    });

    if (QDialog::Accepted == dialog.exec()) {
        _pixmapItem->setToneCurves(backupToneTurves);
        undoStack()->push(new ToneCurveCommand(this, dialog.points()));
    } else {
        _pixmapItem->setToneCurves(backupToneTurves);
        _pixmapItem->redraw();
    }
}

bool MdiChild::loadFile(const QString &filename)
{
    QImageReader reader(filename);
    reader.setAutoTransform(false);

    QImage image = reader.read();
    if (image.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2")
                                 .arg(QDir::toNativeSeparators(filename), reader.errorString()));
        return false;
    }

    setImage(image);

    setWindowFilePath(filename);
    setWindowTitle(strippedName(filename));

    fitToWindow();

    _undoStack->clear();
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

const QImage *MdiChild::image() const
{
    return (pixmapItem() ? pixmapItem()->image() : nullptr);
}

void MdiChild::setImage(const QImage &image)
{
    _pixmapItem->clear();
    _pixmapItem->setImage(image);
    _pixmapItem->redraw();
    _pixmapItem->setVisible(true);

    _scene->setSceneRect(0, 0, image.width(), image.height());
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

AdjustableGraphicsPixmapItem *MdiChild::pixmapItem()
{
    return _pixmapItem;
}

qreal MdiChild::zoomF() const
{
    return matrix().m11();
}

void MdiChild::undo()
{
    _undoStack->undo();
}

void MdiChild::redo()
{
    _undoStack->redo();
}

QUndoStack *MdiChild::undoStack() const
{
    return _undoStack;
}

void MdiChild::flipHorizontal()
{
    undoStack()->push(new FlipHorizontalCommand(this));
}

void MdiChild::flipVertical()
{
    undoStack()->push(new FlipVerticalCommand(this));
}

void MdiChild::rotateCW90()
{
    undoStack()->push(new RotateCW90Command(this));
}

void MdiChild::rotateCCW90()
{
    undoStack()->push(new RotateCCW90Command(this));
}

void MdiChild::canvasSize()
{
    CanvasSizeDialog dialog;
    dialog.setImage(image());

    if (QDialog::Accepted == dialog.exec())
        undoStack()->push(new CanvasSizeCommand(this, dialog.newSize(), dialog.anchor(), dialog.relative()));
}

void MdiChild::filterCustom()
{
    CustomFilterDialog dialog;
    dialog.setKernel(pixmapItem()->kernel());

//    Kernel kernel(new int[25] { 0,0,0,0,0, 0,0,-2,0,0, 0,-2,8,-2,0, 0,0,-2,0,0, 0,0,0,0,0 });
//    dialog.setKernel(kernel);

    if (QDialog::Accepted == dialog.exec())
        undoStack()->push(new FilterCustomCommand(this, dialog.kernel()));
}
