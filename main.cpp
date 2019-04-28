#include "mainwindow.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Yazzy.2");
    QCoreApplication::setApplicationName("Serius");
    QCoreApplication::setApplicationVersion("0.0.0");

    MainWindow w;
    w.show();

    return a.exec();
}
