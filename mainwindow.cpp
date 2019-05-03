#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QtWidgets>

#include "preferencedialog.hpp"
#include "tonecurvedialog.hpp"
#include "brightnesscontrastdialog.hpp"
#include "utility.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    pixmapItem = new QGraphicsPixmapItem();
    pixmapItem->setTransformationMode(Qt::SmoothTransformation);
    pixmapItem->setPixmap(QPixmap::fromImage(image));

    scene = new QGraphicsScene(this);
    scene->addItem(pixmapItem);

    statusLLabel = new QLabel();
    statusLLabel->setAlignment(Qt::AlignLeft);
    statusRLabel = new QLabel();
    statusRLabel->setAlignment(Qt::AlignRight);

    ui->statusBar->addWidget(statusLLabel, 1);
    ui->statusBar->addWidget(statusRLabel);

    ui->graphicsView->setScene(scene);
    ui->navigatorWidget->view()->setScene(scene);

    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::open);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::saveAs);
    connect(ui->actionZoomIn, &QAction::triggered, this, &MainWindow::zoomIn);
    connect(ui->actionZoomOut, &QAction::triggered, this, &MainWindow::zoomOut);
    connect(ui->actionMagnification, &QAction::triggered, this, &MainWindow::zoomMag);
    connect(ui->actionFitToWindow, &QAction::triggered, this, &MainWindow::fitToWindow);
    connect(ui->actionBrightnessContrast, &QAction::triggered, this, &MainWindow::brightnessContrast);
    connect(ui->actionToneCurve, &QAction::triggered, this, &MainWindow::toneCurve);
    connect(ui->actionDisplayDock1, &QAction::triggered, this, &MainWindow::updateDock);
    connect(ui->actionDisplayDock2, &QAction::triggered, this, &MainWindow::updateDock);
    connect(ui->actionDisplayDock3, &QAction::triggered, this, &MainWindow::updateDock);
    connect(ui->actionPreference, &QAction::triggered, this, &MainWindow::preference);

    connect(ui->graphicsView, &TrackingGraphicsView::viewportChanged, ui->navigatorWidget->view(), &NavigatorGraphicsView::drawROI);
    connect(ui->graphicsView, &TrackingGraphicsView::scaleChanged, ui->navigatorWidget, &NavigatorWidget::setZoomF);
    connect(ui->graphicsView, &TrackingGraphicsView::entered, this, &MainWindow::enter);
    connect(ui->graphicsView, &TrackingGraphicsView::leaved, this, &MainWindow::leave);
    connect(ui->navigatorWidget, &NavigatorWidget::zoomChangedF, this, &MainWindow::zoom);
    connect(ui->navigatorWidget->view(), &NavigatorGraphicsView::roiChanged, this, &MainWindow::updateROI);
    connect(ui->channelwidget, &ChannelWidget::stateChanged, this, &MainWindow::updateChannel);

    readSettings();

    setUnifiedTitleAndToolBarOnMac(true);
    setWindowTitle(QCoreApplication::applicationName());

    show();

    ui->actionDisplayDock1->setChecked(ui->dockWidget1->isVisible());
    ui->actionDisplayDock2->setChecked(ui->dockWidget2->isVisible());
    ui->actionDisplayDock3->setChecked(ui->dockWidget3->isVisible());
}

void MainWindow::updateDock()
{
    ui->dockWidget1->setVisible(ui->actionDisplayDock1->isChecked());
    ui->dockWidget2->setVisible(ui->actionDisplayDock2->isChecked());
    ui->dockWidget3->setVisible(ui->actionDisplayDock3->isChecked());
}

void MainWindow::updateROI(const QRectF &sceneRect)
{
    QPointF delta = recentSceneRect.topLeft() - sceneRect.topLeft();
    if (delta.manhattanLength() > 10.0) {
        ui->graphicsView->ensureVisible(sceneRect,0,0);
        recentSceneRect = sceneRect;
    }
}

void MainWindow::updateChannel(int state)
{
    QList<Channel::Color> eraseChannels;
    if (!ui->channelwidget->contains(state, Channel::red))
        eraseChannels.append(Channel::red);
    if (!ui->channelwidget->contains(state, Channel::green))
        eraseChannels.append(Channel::green);
    if (!ui->channelwidget->contains(state, Channel::blue))
        eraseChannels.append(Channel::blue);

    QImage tmp = image.copy();

    pixmapItem->setPixmap(QPixmap::fromImage(Utility::erase(tmp, eraseChannels)));
}

void MainWindow::enter(const QPointF &scenePos)
{
    QRect r = scene->sceneRect().toRect();
    int x = scenePos.toPoint().x();
    int y = scenePos.toPoint().y();

    if (!r.contains(x, y)) {
        leave();
        return;
    }

    QColor c = image.pixelColor(x, y);
    QString message(tr("(%1,%2) R:%3 G:%4 B:%5").arg(x).arg(y).arg(c.red()).arg(c.green()).arg(c.blue()));
    statusRLabel->setText(message);
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

    const QByteArray windowState = settings.value("MainWindow/windowState", QByteArray()).toByteArray();
    if (!windowState.isEmpty())
        restoreState(windowState);
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
    ui->histgramWidget->setEnabled(scene->isActive());
    ui->channelwidget->setEnabled(scene->isActive());
}

void MainWindow::updateAction()
{
    ui->actionSaveAs->setEnabled(scene->isActive());
    ui->actionZoomIn->setEnabled(scene->isActive());
    ui->actionZoomOut->setEnabled(scene->isActive());
    ui->actionMagnification->setEnabled(scene->isActive());
    ui->actionFitToWindow->setEnabled(scene->isActive());
    ui->actionBrightnessContrast->setEnabled(scene->isActive());
    ui->actionToneCurve->setEnabled(scene->isActive());
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
    QString recentOpenDir = settings.value("FileDialog/RecentOpenDir").toString();
    if (!recentOpenDir.isEmpty() && QFileInfo::exists(recentOpenDir))
        dialog.setDirectory(recentOpenDir);

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
        settings.setValue("FileDialog/RecentOpenDir", QFileInfo(selectedFile).dir().path());

        pixmapItem->setPixmap(QPixmap::fromImage(image));
        scene->setSceneRect(0, 0, image.width(), image.height());

        qreal factorX = static_cast<qreal>(ui->navigatorWidget->view()->width() - 10) / static_cast<qreal>(image.width());
        qreal factorY = static_cast<qreal>(ui->navigatorWidget->view()->height() - 10) / static_cast<qreal>(image.height());
        qreal factor = qMin(factorX, factorY);
        ui->navigatorWidget->view()->resetMatrix();
        ui->navigatorWidget->view()->scale(factor, factor);

        toneCurves.clear();

        setWindowFilePath(selectedFile);
        setWindowTitle(QFileInfo(selectedFile).fileName());

        const QString message = tr("\"%1\", %2x%3, Depth: %4")
            .arg(QDir::toNativeSeparators(selectedFile)).arg(image.width()).arg(image.height()).arg(image.depth());
        statusLLabel->setText(message);

        ui->histgramWidget->draw(image);
        updateChannel(ui->channelwidget->state());

        updateView();
        updateAction();

        fitToWindow();
    }
}

void MainWindow::saveAs()
{
    QStringList mimeTypeFilters;
    foreach (const QByteArray &mimeTypeName, QImageWriter::supportedMimeTypes())
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();

    QFileDialog dialog(this, tr("Save File As"));
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.selectMimeTypeFilter("image/jpeg");
    dialog.setDefaultSuffix("jpg");

    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    QString recentSaveDir = settings.value("FileDialog/RecentSaveDir").toString();
    if (!recentSaveDir.isEmpty() && QFileInfo::exists(recentSaveDir))
        dialog.setDirectory(recentSaveDir);

    if (QDialog::Accepted == dialog.exec()) {
        QString selectedFile = dialog.selectedFiles().first();
        QImageWriter writer(selectedFile);

        QImage image = pixmapItem->pixmap().toImage();
        if (!writer.write(image)) {
            QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                     tr("Cannot write %1: %2")
                                     .arg(QDir::toNativeSeparators(selectedFile)), writer.errorString());
            return;
        }

        QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
        settings.setValue("FileDialog/RecentSaveDir", QFileInfo(selectedFile).dir().path());
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

void MainWindow::brightnessContrast()
{
    BrightnessContrastDialog dialog;

    if (QDialog::Accepted == dialog.exec()) {
        QImage tmp = image.copy();
        Utility::brightness(tmp, dialog.brightness());
        Utility::contrast(tmp, dialog.contrast());
        pixmapItem->setPixmap(QPixmap::fromImage(tmp));
        ui->histgramWidget->draw(tmp);
    }
}

void MainWindow::toneCurve()
{
    ToneCurveDialog dialog;
    dialog.setPreview(&image, pixmapItem);
    dialog.setToneCurve(toneCurves);
    dialog.setHistgram(ui->histgramWidget->statistics());

    if (QDialog::Accepted == dialog.exec())
        toneCurves = dialog.points();

    QMap<int, int> lutL = Utility::createLUT(toneCurves.value(Channel::luminance));
    QMap<int, int> lutR = Utility::createLUT(toneCurves.value(Channel::red));
    QMap<int, int> lutG = Utility::createLUT(toneCurves.value(Channel::green));
    QMap<int, int> lutB = Utility::createLUT(toneCurves.value(Channel::blue));

    if (lutR.isEmpty() && lutG.isEmpty() && lutB.isEmpty() && lutL.isEmpty()) {
        pixmapItem->setPixmap(QPixmap::fromImage(image));
        ui->histgramWidget->draw(image);
        return;
    }

    QImage tmp = image.copy();
    if (lutR.isEmpty() && lutG.isEmpty() && lutB.isEmpty())
        Utility::convert(tmp, lutL);
    else
        Utility::convert(tmp, lutR, lutG, lutB);

    pixmapItem->setPixmap(QPixmap::fromImage(tmp));
    ui->histgramWidget->draw(tmp);
}

void MainWindow::preference()
{
    PreferenceDialog dialog;

    if (QDialog::Accepted == dialog.exec()) {
        //TODO
    }
}

