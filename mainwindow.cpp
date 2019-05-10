#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QtWidgets>

#include "preferencedialog.hpp"
#include "tonecurvedialog.hpp"
#include "brightnesscontrastdialog.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
    connect(ui->actionDisplayUndoStack, &QAction::triggered, this, &MainWindow::updateDock);
    connect(ui->actionPreference, &QAction::triggered, this, &MainWindow::preference);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::about);
    connect(ui->actionAboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);

    connect(ui->mdiArea, &QMdiArea::subWindowActivated, this, &MainWindow::updateAction);
    connect(ui->mdiArea, &QMdiArea::subWindowActivated, this, &MainWindow::updateStatusBar);
    connect(ui->mdiArea, &QMdiArea::subWindowActivated, this, &MainWindow::updateNavigator);
    connect(ui->mdiArea, &QMdiArea::subWindowActivated, this, &MainWindow::updateHistgram);
    connect(ui->mdiArea, &QMdiArea::subWindowActivated, this, &MainWindow::updateChannel);

    readSettings();

    setUnifiedTitleAndToolBarOnMac(true);
    setWindowTitle(QCoreApplication::applicationName());

    show();

    ui->actionDisplayDock1->setChecked(ui->dockWidget1->isVisible());
    ui->actionDisplayDock2->setChecked(ui->dockWidget2->isVisible());
    ui->actionDisplayDock3->setChecked(ui->dockWidget3->isVisible());
    ui->actionDisplayUndoStack->setChecked(ui->dockWidget4->isVisible());
}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::enter(const QPointF &scenePos)
//{
//    QRect r = scene->sceneRect().toRect();
//    int x = scenePos.toPoint().x();
//    int y = scenePos.toPoint().y();

//    if (!r.contains(x, y)) {
//        leave();
//        return;
//    }

//    QColor c = image.pixelColor(x, y);
//    QString message(tr("(%1,%2) R:%3 G:%4 B:%5").arg(x).arg(y).arg(c.red()).arg(c.green()).arg(c.blue()));
//    statusRLabel->setText(message);
//}

//void MainWindow::leave()
//{
//    statusRLabel->clear();
//}

void MainWindow::closeEvent(QCloseEvent *event)
{
    ui->mdiArea->closeAllSubWindows();
    if (ui->mdiArea->currentSubWindow()) {
        event->ignore();
    } else {
        writeSettings();
        event->accept();
    }
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

void MainWindow::updateHistgram()
{
    ui->histgramWidget->setEnabled(activeMdiChild());

    if (activeMdiChild()) {
        connect(activeMdiChild()->pixmapItem(), &AdjustableGraphicsPixmapItem::statisticsChanged, ui->histgramWidget, &HistgramWidget::draw);
    }

    foreach (MdiChild *child, deactiveMdiChilds()) {
        disconnect(child->pixmapItem(), &AdjustableGraphicsPixmapItem::statisticsChanged, ui->histgramWidget, &HistgramWidget::draw);
    }

    if (activeMdiChild())
        ui->histgramWidget->draw(activeMdiChild()->pixmapItem()->statistics());
    else
        ui->histgramWidget->clear();
}

void MainWindow::updateNavigator()
{
    ui->navigatorWidget->setEnabled(activeMdiChild());
    ui->navigatorWidget->view()->setScene(activeMdiChild() ? activeMdiChild()->scene() : nullptr);
    ui->navigatorWidget->view()->rubberBand()->setVisible(activeMdiChild());    //↑と連動させたい。。。

    if (activeMdiChild()) {
        connect(activeMdiChild(), &TrackingGraphicsView::roiChanged, ui->navigatorWidget->view(), &NavigatorGraphicsView::drawROI);
        connect(activeMdiChild(), &TrackingGraphicsView::scaleChanged, ui->navigatorWidget, &NavigatorWidget::setZoomF);

        connect(ui->navigatorWidget, &NavigatorWidget::zoomChangedF, activeMdiChild(), &MdiChild::setZoomF);
        connect(ui->navigatorWidget->view(), &NavigatorGraphicsView::roiChanged, activeMdiChild(), &MdiChild::ensureVisible);
    }

    foreach (MdiChild *child, deactiveMdiChilds()) {
        disconnect(child, &TrackingGraphicsView::roiChanged, ui->navigatorWidget->view(), &NavigatorGraphicsView::drawROI);
        disconnect(child, &TrackingGraphicsView::scaleChanged, ui->navigatorWidget, &NavigatorWidget::setZoomF);

        disconnect(ui->navigatorWidget, &NavigatorWidget::zoomChangedF, child, &MdiChild::setZoomF);
        disconnect(ui->navigatorWidget->view(), &NavigatorGraphicsView::roiChanged, child, &MdiChild::ensureVisible);
    }

    if (activeMdiChild()) {
        ui->navigatorWidget->setZoomF(activeMdiChild()->zoomF());
        ui->navigatorWidget->view()->fitToWindow(activeMdiChild()->image());
        ui->navigatorWidget->view()->drawROI(activeMdiChild()->roi());
    } else {
        ui->navigatorWidget->clear();
    }
}

void MainWindow::updateChannel()
{
    ui->channelwidget->setEnabled(activeMdiChild());

    if (activeMdiChild()) {
        connect(ui->channelwidget, &ChannelWidget::stateChanged, activeMdiChild()->pixmapItem(), &AdjustableGraphicsPixmapItem::setChannelVisible);
        connect(ui->channelwidget, &ChannelWidget::stateChanged, activeMdiChild()->pixmapItem(), &AdjustableGraphicsPixmapItem::redraw);
    }

    foreach (MdiChild *child, deactiveMdiChilds()) {
        disconnect(ui->channelwidget, &ChannelWidget::stateChanged, child->pixmapItem(), &AdjustableGraphicsPixmapItem::setChannelVisible);
        disconnect(ui->channelwidget, &ChannelWidget::stateChanged, child->pixmapItem(), &AdjustableGraphicsPixmapItem::redraw);
    }

    if (activeMdiChild())
        ui->channelwidget->select(activeMdiChild()->pixmapItem()->channelVisibles());
    else
        ui->channelwidget->selectAll();
}

void MainWindow::updateAction()
{
    ui->actionSaveAs->setEnabled(activeMdiChild());
    ui->actionZoomIn->setEnabled(activeMdiChild());
    ui->actionZoomOut->setEnabled(activeMdiChild());
    ui->actionMagnification->setEnabled(activeMdiChild());
    ui->actionFitToWindow->setEnabled(activeMdiChild());
    ui->actionBrightnessContrast->setEnabled(activeMdiChild());
    ui->actionToneCurve->setEnabled(activeMdiChild());

    if (activeMdiChild()) {
        connect(ui->actionUndo, &QAction::triggered, activeMdiChild(), &MdiChild::undo);
        connect(ui->actionRedo, &QAction::triggered, activeMdiChild(), &MdiChild::redo);

        connect(activeMdiChild(), &MdiChild::canUndoChanged, this, [=] (bool canUndo) {
            ui->actionUndo->setEnabled(canUndo);
        });
        connect(activeMdiChild(), &MdiChild::canRedoChanged, this, [=] (bool canRedo) {
            ui->actionRedo->setEnabled(canRedo);
        });
    }

    foreach (MdiChild *child, deactiveMdiChilds()) {
        disconnect(ui->actionUndo, &QAction::triggered, child, &MdiChild::undo);
        disconnect(ui->actionRedo, &QAction::triggered, child, &MdiChild::redo);

        connect(child, &MdiChild::canUndoChanged, this, [=] (bool) {});
        connect(child, &MdiChild::canRedoChanged, this, [=] (bool) {});
    }

    if (activeMdiChild()) {
        ui->actionUndo->setEnabled(activeMdiChild()->undoStack()->canUndo());
        ui->actionRedo->setEnabled(activeMdiChild()->undoStack()->canRedo());
    } else {
        ui->actionUndo->setEnabled(false);
        ui->actionRedo->setEnabled(false);
    }
    ui->undoView->setStack(activeMdiChild() ? activeMdiChild()->undoStack() : nullptr);
}

void MainWindow::updateDock()
{
    ui->dockWidget1->setVisible(ui->actionDisplayDock1->isChecked());
    ui->dockWidget2->setVisible(ui->actionDisplayDock2->isChecked());
    ui->dockWidget3->setVisible(ui->actionDisplayDock3->isChecked());
    ui->dockWidget4->setVisible(ui->actionDisplayUndoStack->isChecked());
}

void MainWindow::updateStatusBar()
{
    if (!activeMdiChild()) {
        ui->statusBar->clearMessage();
        return;
    }

    const QImage &image = activeMdiChild()->image();
    const QString &fileName = activeMdiChild()->windowFilePath();

    const QString message = tr("\"%1\", %2x%3, Depth: %4")
        .arg(QDir::toNativeSeparators(fileName)).arg(image.width()).arg(image.height()).arg(image.depth());
    ui->statusBar->showMessage(message);
}

MdiChild *MainWindow::createMdiChild()
{
    MdiChild *child = new MdiChild;
    return child;
}

MdiChild *MainWindow::activeMdiChild() const
{
    if (QMdiSubWindow *activeSubWindow = ui->mdiArea->activeSubWindow())
        return qobject_cast<MdiChild *>(activeSubWindow->widget());
    return nullptr;
}

QList<MdiChild *> MainWindow::deactiveMdiChilds() const
{
    QList<MdiChild *> list;
    foreach (QMdiSubWindow *subWindow, ui->mdiArea->subWindowList()) {
        MdiChild *child = qobject_cast<MdiChild *>(subWindow->widget());
        if (subWindow != ui->mdiArea->activeSubWindow())
            list.append(child);
    }
    return list;
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
        QString filename = dialog.selectedFiles().first();

        MdiChild *child = createMdiChild();
        if (!child->loadFile(filename)) {
            child->close();
            return;
        }

        QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
        settings.setValue("FileDialog/RecentOpenDir", QFileInfo(filename).dir().path());

        ui->mdiArea->addSubWindow(child);
        child->show();
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
        QString filename = dialog.selectedFiles().first();

        if (!activeMdiChild()->saveFile(filename)) {
            return;
        }

        QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
        settings.setValue("FileDialog/RecentSaveDir", QFileInfo(filename).dir().path());
    }
}

void MainWindow::zoom(qreal factor)
{
    if (activeMdiChild())
        activeMdiChild()->setZoomF(factor);
}

void MainWindow::zoomIn()
{
    if (activeMdiChild())
        activeMdiChild()->zoomIn();
}

void MainWindow::zoomOut()
{
    if (activeMdiChild())
        activeMdiChild()->zoomOut();
}

void MainWindow::zoomMag()
{
    if (activeMdiChild())
        activeMdiChild()->zoomMag();
}

void MainWindow::fitToWindow()
{
    if (activeMdiChild())
        activeMdiChild()->fitToWindow();
}

void MainWindow::brightnessContrast()
{
    if (activeMdiChild())
        activeMdiChild()->brightnessContrast();
}

void MainWindow::toneCurve()
{
    if (activeMdiChild())
        activeMdiChild()->toneCurve();
}

void MainWindow::preference()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QString language = settings.value("Application/translator", "ja").toString();
    const QString style = settings.value("Application/style").toString();

    PreferenceDialog dialog;
    dialog.setStyle(style);
    dialog.setLanguage(language);

    if (QDialog::Accepted == dialog.exec()) {
        settings.setValue("Application/translator", dialog.language());
        settings.setValue("Application/style", dialog.style());
    }
}

void MainWindow::about()
{
    //TODO
}
