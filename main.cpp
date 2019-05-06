#include "mainwindow.hpp"
#include <QApplication>
#include <QTranslator>
#include <QSettings>
#include <QDebug>
#include <QStyle>
#include <QStyleFactory>

QString currentStyleKey()
{
    foreach (QString key, QStyleFactory::keys()) {
        QStyle *style = QStyleFactory::create(key);
        if (style->objectName() == QApplication::style()->objectName())
            return key;
    }
    return QString("");
}

bool useStandardPalette(const QStyle *style)
{
    QStringList donotUseStandardPaletteStyles({"macintosh", "windowsxp"});
    return !donotUseStandardPaletteStyles.contains(style->objectName());
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Yazzy.2");
    QCoreApplication::setApplicationName("Serius");
    QCoreApplication::setApplicationVersion("0.0.0");

    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    QString language = settings.value("Application/translator", "ja").toString();
    QString styleKey = settings.value("Application/style", currentStyleKey()).toString();

    QTranslator translator;
    translator.load("language_" + language + ".qm", ":/assets/translate");
    a.installTranslator(&translator);

    QStyle *style = QStyleFactory::create(styleKey);
    if (style) {
        if (useStandardPalette(style))
            qApp->setPalette(style->standardPalette());
        qApp->setStyle(QStyleFactory::create(styleKey));
    }

    MainWindow w;
    w.show();

    return a.exec();
}
