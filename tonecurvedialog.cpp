#include "tonecurvedialog.hpp"
#include "ui_tonecurvedialog.h"

ToneCurveDialog::ToneCurveDialog(QWidget *parent) :
    MemorizeDialog(parent),
    ui(new Ui::ToneCurveDialog)
{
    ui->setupUi(this);

    createAction();

    connect(ui->baselineCheckBox, &QCheckBox::toggled, ui->widget, &ToneCurveWidget::setBaselineVisible);
    connect(ui->histgramCheckBox, &QCheckBox::toggled, ui->widget, &ToneCurveWidget::setHistgramVisible);
}

ToneCurveDialog::~ToneCurveDialog()
{
    delete ui;
}

void ToneCurveDialog::createAction()
{
    deleteShortcut = new QShortcut(QKeySequence::Delete, this);
    setShortcutEnabled(deleteShortcut->id(), true);

    connect(deleteShortcut, &QShortcut::activated, ui->widget, &ToneCurveWidget::removeCurrentPoint);
}
