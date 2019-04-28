#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(QCoreApplication::applicationName());

    scene = new QGraphicsScene(this);
    statusLLabel = new QLabel();
    statusLLabel->setAlignment(Qt::AlignLeft);
    statusRLabel = new QLabel();
    statusRLabel->setAlignment(Qt::AlignRight);

    ui->statusBar->addWidget(statusLLabel, 1);
    ui->statusBar->addWidget(statusRLabel);

    ui->graphicsView->setScene(scene);
    ui->navigatorWidget->view()->setScene(scene);

    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::open);
    connect(ui->actionZoomIn, &QAction::triggered, this, &MainWindow::zoomIn);
    connect(ui->actionZoomOut, &QAction::triggered, this, &MainWindow::zoomOut);
    connect(ui->actionMagnification, &QAction::triggered, this, &MainWindow::zoomMag);
    connect(ui->actionFitToWindow, &QAction::triggered, this, &MainWindow::fitToWindow);

    connect(ui->graphicsView, &TrackingGraphicsView::viewportChanged, ui->navigatorWidget->view(), &NavigatorGraphicsView::drawViewport);
    connect(ui->graphicsView, &TrackingGraphicsView::scaleChanged, ui->navigatorWidget, &NavigatorWidget::setZoomF);
    connect(ui->graphicsView, &TrackingGraphicsView::entered, this, &MainWindow::enter);
    connect(ui->graphicsView, &TrackingGraphicsView::leaved, this, &MainWindow::leave);
    connect(ui->navigatorWidget, &NavigatorWidget::zoomChangedF, this, &MainWindow::zoom);
    connect(ui->navigatorWidget->view(), &NavigatorGraphicsView::viewportChanged, this, &MainWindow::updateViewport);

    readSettings();
}

void MainWindow::updateViewport(const QRectF &sceneRect)
{
    QPointF delta = recentSceneRect.topLeft() - sceneRect.topLeft();
//    qDebug() << __PRETTY_FUNCTION__ << sceneRect << delta.manhattanLength();
    if (delta.manhattanLength() > 10.0) {
        ui->graphicsView->ensureVisible(sceneRect,0,0);
        recentSceneRect = sceneRect;
    }
}

void MainWindow::enter(const QPointF &scenePos)
{
    QRect r = scene->sceneRect().toRect();
    int x = scenePos.toPoint().x();
    int y = scenePos.toPoint().y();

    if (r.contains(x, y))
        statusRLabel->setText(tr("(%1,%2)").arg(x).arg(y));
    else
        leave();
}

void MainWindow::leave()
{
    statusRLabel->clear();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
}

void MainWindow::readSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("MainWindow/geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() / 2, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2, (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }

//    const QByteArray windowState = settings.value("MainWindow/windowState", QByteArray()).toByteArray();
//    if (!windowState.isEmpty())
//        restoreState(windowState);
}

void MainWindow::writeSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("MainWindow/geometry", saveGeometry());
    settings.setValue("MainWindow/windowState", saveState());
}

void MainWindow::updateView()
{
    ui->navigatorWidget->setEnabled(scene->isActive());
}

void MainWindow::updateAction()
{
    ui->actionZoomIn->setEnabled(scene->isActive());
    ui->actionZoomOut->setEnabled(scene->isActive());
    ui->actionMagnification->setEnabled(scene->isActive());
    ui->actionFitToWindow->setEnabled(scene->isActive());
}

void MainWindow::open()
{
    QStringList mimeTypeFilters;
    foreach (const QByteArray &mimeTypeName, QImageReader::supportedMimeTypes())
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();

    QFileDialog dialog(this, tr("Open File"));
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.selectMimeTypeFilter("image/jpeg");

    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    QString recentOpenFile = settings.value("FileDialog/RecentOpenFile").toString();
//    qDebug() << __PRETTY_FUNCTION__ << recentOpenFile;
    if (!recentOpenFile.isEmpty() && QFileInfo::exists(recentOpenFile))
        dialog.selectFile(recentOpenFile);

    if (QDialog::Accepted == dialog.exec()) {
        QString selectedFile = dialog.selectedFiles().first();

        QImageReader reader(selectedFile);
        reader.setAutoTransform(false);
        image = reader.read();
        if (image.isNull()) {
            QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                     tr("Cannot load %1: %2")
                                     .arg(QDir::toNativeSeparators(selectedFile), reader.errorString()));
            return;
        }

        QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
        settings.setValue("FileDialog/RecentOpenFile", selectedFile);

        QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem();
        pixmapItem->setTransformationMode(Qt::SmoothTransformation);
        pixmapItem->setPixmap(QPixmap::fromImage(image));

        scene->clear();
        scene->addItem(pixmapItem);
        scene->setSceneRect(0, 0, image.width(), image.height());

        qreal factorX = static_cast<qreal>(ui->navigatorWidget->view()->width() - 10) / static_cast<qreal>(image.width());
        qreal factorY = static_cast<qreal>(ui->navigatorWidget->view()->height() - 10) / static_cast<qreal>(image.height());
        qreal factor = qMin(factorX, factorY);
        ui->navigatorWidget->view()->resetMatrix();
        ui->navigatorWidget->view()->scale(factor, factor);

        setWindowFilePath(selectedFile);
        setWindowTitle(QFileInfo(selectedFile).fileName());

        const QString message = tr("\"%1\", %2x%3, Depth: %4")
            .arg(QDir::toNativeSeparators(selectedFile)).arg(image.width()).arg(image.height()).arg(image.depth());
        statusLLabel->setText(message);

        updateView();
        updateAction();

        fitToWindow();
    }
}

void MainWindow::zoom(qreal coeff)
{
    ui->graphicsView->resetMatrix();
    ui->graphicsView->scaleEx(coeff);
}

void MainWindow::zoomIn()
{
    ui->graphicsView->scaleEx(1.2);
}

void MainWindow::zoomOut()
{
    ui->graphicsView->scaleEx(0.8);
}

void MainWindow::zoomMag()
{
    ui->graphicsView->resetMatrix();
    ui->graphicsView->scaleEx(1.0);
}

void MainWindow::fitToWindow()
{
    qreal factorX = static_cast<qreal>(ui->graphicsView->width()) / static_cast<qreal>(image.width());
    qreal factorY = static_cast<qreal>(ui->graphicsView->height()) / static_cast<qreal>(image.height());
    qreal factor = qMin(factorX, factorY);

    ui->graphicsView->resetMatrix();
    ui->graphicsView->scaleEx(factor);
}
