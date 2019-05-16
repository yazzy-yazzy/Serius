#-------------------------------------------------
#
# Project created by QtCreator 2019-04-28T16:30:49
#
#-------------------------------------------------

QT       += core gui charts concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Sirius
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    navigatorgraphicsview.cpp \
    navigatorrubberband.cpp \
    navigatorwidget.cpp \
    rectanglegraphicsview.cpp \
    trackinggraphicsview.cpp \
    statistics.cpp \
    histgramwidget.cpp \
    channelwidget.cpp \
    tonecurvedialog.cpp \
    memorizedialog.cpp \
    tonecurvewidget.cpp \
    trackingsplineseries.cpp \
    trackingchartview.cpp \
    trackingchartevent.cpp \
    preferencedialog.cpp \
    utility.cpp \
    brightnesscontrastdialog.cpp \
    adjustablegraphicspixmapitem.cpp \
    mdichild.cpp \
    versiondialog.cpp \
    canvassizedialog.cpp \
    anchorwidget.cpp \
    customfilterdialog.cpp \
    spinnerdialog.cpp

HEADERS += \
    mainwindow.hpp \
    navigatorgraphicsview.hpp \
    navigatorrubberband.hpp \
    navigatorwidget.hpp \
    rectanglegraphicsview.hpp \
    trackinggraphicsview.hpp \
    statistics.hpp \
    utility.hpp \
    histgramwidget.hpp \
    channelwidget.hpp \
    tonecurvedialog.hpp \
    memorizedialog.hpp \
    tonecurvewidget.hpp \
    trackingsplineseries.hpp \
    trackingchartview.hpp \
    trackingchartevent.hpp \
    preferencedialog.hpp \
    singleton.hpp \
    channel.hpp \
    brightnesscontrastdialog.hpp \
    adjustablegraphicspixmapitem.hpp \
    mdichild.hpp \
    versiondialog.hpp \
    canvassizedialog.hpp \
    anchorwidget.hpp \
    customfilterdialog.hpp \
    kernel.hpp \
    spinnerdialog.hpp

FORMS += \
    mainwindow.ui \
    navigatorwidget.ui \
    histgramwidget.ui \
    channelwidget.ui \
    tonecurvedialog.ui \
    tonecurvewidget.ui \
    preferencedialog.ui \
    brightnesscontrastdialog.ui \
    versiondialog.ui \
    canvassizedialog.ui \
    anchorwidget.ui \
    customfilterdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

TRANSLATIONS = \
    assets/translate/language_ja.ts \
    assets/translate/language_en.ts
