#include "mdichild.hpp"

#include <QtWidgets>

#include "brightnesscontrastdialog.hpp"
#include "canvassizedialog.hpp"
#include "tonecurvedialog.hpp"
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

    qreal factorX = static_cast<qreal>(_child->width()) / static_cast<qreal>(_child->image().width());
    qreal factorY = static_cast<qreal>(_child->height()) / static_cast<qreal>(_child->image().height());
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
    _child->setImage(_child->image().mirrored(true, false));
}

void FlipHorizontalCommand::redo()
{
    _child->setImage(_child->image().mirrored(true, false));
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
    _child->setImage(_child->image().mirrored(false, true));
}

void FlipVerticalCommand::redo()
{
    _child->setImage(_child->image().mirrored(false, true));
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

    _child->setImage(_child->image().transformed(matrix));
    _child->parentWidget()->resize(_child->parentWidget()->size().transposed());
}

void RotateCW90Command::redo()
{
    QTransform matrix;
    matrix.rotate(90);

    _child->setImage(_child->image().transformed(matrix));
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

    _child->setImage(_child->image().transformed(matrix));
    _child->parentWidget()->resize(_child->parentWidget()->size().transposed());
}

void RotateCCW90Command::redo()
{
    QTransform matrix;
    matrix.rotate(-90);

    _child->setImage(_child->image().transformed(matrix));
    _child->parentWidget()->resize(_child->parentWidget()->size().transposed());
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

    if (QDialog::Accepted == dialog.exec()) {
        _pixmapItem->setBrightness(dialog.brightness());
        _pixmapItem->setContrast(dialog.contrast());
        _pixmapItem->redraw();
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

    QImage image = reader.read();
    if (image.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2")
                                 .arg(QDir::toNativeSeparators(filename), reader.errorString()));
        return false;
    }

    _pixmapItem->clear();

    setImage(image);

    _pixmapItem->setVisible(true);

    _scene->setSceneRect(0, 0, _image.width(), _image.height());

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

void MdiChild::setImage(const QImage &image)
{
    _image = image;
    _pixmapItem->setImage(&_image);
    _pixmapItem->redraw();

    _scene->setSceneRect(0, 0, _image.width(), _image.height());
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

void MdiChild::crop()
{
    CanvasSizeDialog dialog;
    dialog.setImage(&_image);

    if (QDialog::Accepted == dialog.exec()) {
        //TODO
    }
}
