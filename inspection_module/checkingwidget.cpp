#include "checkingwidget.h"
#include "ui_checkingwidget.h"

CheckingWidget::CheckingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CheckingWidget)
{
    ui->setupUi(this);
}

CheckingWidget::~CheckingWidget()
{
    delete ui;
}
