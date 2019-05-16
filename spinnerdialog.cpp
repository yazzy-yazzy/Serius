#include "spinnerdialog.hpp"

#include <QtWidgets>

SpinnerDialog::SpinnerDialog(QWidget *parent) : QDialog(parent)
{
    QLabel *lbl = new QLabel;

    QMovie *movie = new QMovie(":/animation/indicator");
    lbl->setMovie(movie);
    lbl->show();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(lbl);

    setLayout(layout);

    setFixedSize(movie->frameRect().width(), movie->frameRect().height());
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setWindowFlag(Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);

    const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
    resize(availableGeometry.width() / 2, availableGeometry.height() / 2);
    move((availableGeometry.width() - width()) / 2, (availableGeometry.height() - height()) / 2);

    movie->start();
}
