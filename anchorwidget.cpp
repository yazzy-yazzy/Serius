#include "anchorwidget.hpp"
#include "ui_anchorwidget.h"

#include <QtWidgets>

AnchorWidget::AnchorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnchorWidget),
    buttonGroup(new QButtonGroup)
{
    ui->setupUi(this);

    QStyle *fusionStyle = QStyleFactory::create("Fusion");

    ui->topLeftButton->setStyle(fusionStyle);
    ui->topButton->setStyle(fusionStyle);
    ui->topRightButton->setStyle(fusionStyle);
    ui->leftButton->setStyle(fusionStyle);
    ui->centerButton->setStyle(fusionStyle);
    ui->rightButton->setStyle(fusionStyle);
    ui->bottomLeftButton->setStyle(fusionStyle);
    ui->bottomButton->setStyle(fusionStyle);
    ui->bottomRightButton->setStyle(fusionStyle);

    const QString ss = "QPushButton:checked{background-color: orange;}";
    ui->topLeftButton->setStyleSheet(ss);
    ui->topButton->setStyleSheet(ss);
    ui->topRightButton->setStyleSheet(ss);
    ui->leftButton->setStyleSheet(ss);
    ui->centerButton->setStyleSheet(ss);
    ui->rightButton->setStyleSheet(ss);
    ui->bottomLeftButton->setStyleSheet(ss);
    ui->bottomButton->setStyleSheet(ss);
    ui->bottomRightButton->setStyleSheet(ss);

    buttonGroup->addButton(ui->bottomLeftButton, 1);
    buttonGroup->addButton(ui->bottomButton, 2);
    buttonGroup->addButton(ui->bottomRightButton, 3);
    buttonGroup->addButton(ui->leftButton, 4);
    buttonGroup->addButton(ui->centerButton, 5);
    buttonGroup->addButton(ui->rightButton, 6);
    buttonGroup->addButton(ui->topLeftButton, 7);
    buttonGroup->addButton(ui->topButton, 8);
    buttonGroup->addButton(ui->topRightButton, 9);

    map.insert(1, Anchor::BottomLeft);
    map.insert(2, Anchor::Bottom);
    map.insert(3, Anchor::BottomRight);
    map.insert(4, Anchor::Left);
    map.insert(5, Anchor::Center);
    map.insert(6, Anchor::Right);
    map.insert(7, Anchor::TopLeft);
    map.insert(8, Anchor::Top);
    map.insert(9, Anchor::TopRight);

    connect(ui->topLeftButton, &QPushButton::clicked, this, &AnchorWidget::selectTopLeft);
    connect(ui->topButton, &QPushButton::clicked, this, &AnchorWidget::selectTop);
    connect(ui->topRightButton, &QPushButton::clicked, this, &AnchorWidget::selectTopRight);
    connect(ui->leftButton, &QPushButton::clicked, this, &AnchorWidget::selectLeft);
    connect(ui->centerButton, &QPushButton::clicked, this, &AnchorWidget::selectCenter);
    connect(ui->rightButton, &QPushButton::clicked, this, &AnchorWidget::selectRight);
    connect(ui->bottomLeftButton, &QPushButton::clicked, this, &AnchorWidget::selectBottomLeft);
    connect(ui->bottomButton, &QPushButton::clicked, this, &AnchorWidget::selectBottom);
    connect(ui->bottomRightButton, &QPushButton::clicked, this, &AnchorWidget::selectBottomRight);

    selectCenter();
}

AnchorWidget::~AnchorWidget()
{
    delete ui;
}

Anchor AnchorWidget::anchor() const
{
    return map.value(buttonGroup->checkedId());
}

void AnchorWidget::selectTopLeft()
{
    ui->topLeftButton->setIcon(QIcon(":/images/arrow/anchor"));
    ui->topButton->setIcon(QIcon(":/images/arrow/right"));
    ui->topRightButton->setIcon(QIcon());
    ui->leftButton->setIcon(QIcon(":/images/arrow/bottom"));
    ui->centerButton->setIcon(QIcon(":/images/arrow/bottom-right"));
    ui->rightButton->setIcon(QIcon());
    ui->bottomLeftButton->setIcon(QIcon());
    ui->bottomButton->setIcon(QIcon());
    ui->bottomRightButton->setIcon(QIcon());
}

void AnchorWidget::selectTop()
{
    ui->topLeftButton->setIcon(QIcon(":/images/arrow/left"));
    ui->topButton->setIcon(QIcon(":/images/arrow/anchor"));
    ui->topRightButton->setIcon(QIcon(":/images/arrow/right"));
    ui->leftButton->setIcon(QIcon(":/images/arrow/bottom-left"));
    ui->centerButton->setIcon(QIcon(":/images/arrow/bottom"));
    ui->rightButton->setIcon(QIcon(":/images/arrow/bottom-right"));
    ui->bottomLeftButton->setIcon(QIcon());
    ui->bottomButton->setIcon(QIcon());
    ui->bottomRightButton->setIcon(QIcon());
}

void AnchorWidget::selectTopRight()
{
    ui->topLeftButton->setIcon(QIcon());
    ui->topButton->setIcon(QIcon(":/images/arrow/left"));
    ui->topRightButton->setIcon(QIcon(":/images/arrow/anchor"));
    ui->leftButton->setIcon(QIcon());
    ui->centerButton->setIcon(QIcon(":/images/arrow/bottom-left"));
    ui->rightButton->setIcon(QIcon(":/images/arrow/bottom"));
    ui->bottomLeftButton->setIcon(QIcon());
    ui->bottomButton->setIcon(QIcon());
    ui->bottomRightButton->setIcon(QIcon());
}

void AnchorWidget::selectLeft()
{
    ui->topLeftButton->setIcon(QIcon(":/images/arrow/top"));
    ui->topButton->setIcon(QIcon(":/images/arrow/top-right"));
    ui->topRightButton->setIcon(QIcon());
    ui->leftButton->setIcon(QIcon(":/images/arrow/anchor"));
    ui->centerButton->setIcon(QIcon(":/images/arrow/right"));
    ui->rightButton->setIcon(QIcon());
    ui->bottomLeftButton->setIcon(QIcon(":/images/arrow/bottom"));
    ui->bottomButton->setIcon(QIcon(":/images/arrow/bottom-right"));
    ui->bottomRightButton->setIcon(QIcon());
}

void AnchorWidget::selectCenter()
{
    ui->topLeftButton->setIcon(QIcon(":/images/arrow/top-left"));
    ui->topButton->setIcon(QIcon(":/images/arrow/top"));
    ui->topRightButton->setIcon(QIcon(":/images/arrow/top-right"));
    ui->leftButton->setIcon(QIcon(":/images/arrow/left"));
    ui->centerButton->setIcon(QIcon(":/images/arrow/anchor"));
    ui->rightButton->setIcon(QIcon(":/images/arrow/right"));
    ui->bottomLeftButton->setIcon(QIcon(":/images/arrow/bottom-left"));
    ui->bottomButton->setIcon(QIcon(":/images/arrow/bottom"));
    ui->bottomRightButton->setIcon(QIcon(":/images/arrow/bottom-right"));
}

void AnchorWidget::selectRight()
{
    ui->topLeftButton->setIcon(QIcon());
    ui->topButton->setIcon(QIcon(":/images/arrow/top-left"));
    ui->topRightButton->setIcon(QIcon(":/images/arrow/top"));
    ui->leftButton->setIcon(QIcon());
    ui->centerButton->setIcon(QIcon(":/images/arrow/left"));
    ui->rightButton->setIcon(QIcon(":/images/arrow/anchor"));
    ui->bottomLeftButton->setIcon(QIcon());
    ui->bottomButton->setIcon(QIcon(":/images/arrow/bottom-left"));
    ui->bottomRightButton->setIcon(QIcon(":/images/arrow/bottom"));
}

void AnchorWidget::selectBottomLeft()
{
    ui->topLeftButton->setIcon(QIcon());
    ui->topButton->setIcon(QIcon());
    ui->topRightButton->setIcon(QIcon());
    ui->leftButton->setIcon(QIcon(":/images/arrow/top"));
    ui->centerButton->setIcon(QIcon(":/images/arrow/top-right"));
    ui->rightButton->setIcon(QIcon());
    ui->bottomLeftButton->setIcon(QIcon(":/images/arrow/anchor"));
    ui->bottomButton->setIcon(QIcon(":/images/arrow/right"));
    ui->bottomRightButton->setIcon(QIcon());
}

void AnchorWidget::selectBottom()
{
    ui->topLeftButton->setIcon(QIcon());
    ui->topButton->setIcon(QIcon());
    ui->topRightButton->setIcon(QIcon());
    ui->leftButton->setIcon(QIcon(":/images/arrow/top-left"));
    ui->centerButton->setIcon(QIcon(":/images/arrow/top"));
    ui->rightButton->setIcon(QIcon(":/images/arrow/top-right"));
    ui->bottomLeftButton->setIcon(QIcon(":/images/arrow/left"));
    ui->bottomButton->setIcon(QIcon(":/images/arrow/anchor"));
    ui->bottomRightButton->setIcon(QIcon(":/images/arrow/right"));
}

void AnchorWidget::selectBottomRight()
{
    ui->topLeftButton->setIcon(QIcon());
    ui->topButton->setIcon(QIcon());
    ui->topRightButton->setIcon(QIcon());
    ui->leftButton->setIcon(QIcon());
    ui->centerButton->setIcon(QIcon(":/images/arrow/top-left"));
    ui->rightButton->setIcon(QIcon(":/images/arrow/top"));
    ui->bottomLeftButton->setIcon(QIcon());
    ui->bottomButton->setIcon(QIcon(":/images/arrow/left"));
    ui->bottomRightButton->setIcon(QIcon(":/images/arrow/anchor"));
}


