#include "alignwidget.h"
#include "ui_alignwidget.h"

#include <iostream>

AlignWidget::AlignWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CheckingWidget)
{
    ui->setupUi(this);

    showFullScreen();

    QPixmap pixmap("quit.png");
    QIcon ButtonIcon(pixmap);
    ui->closeButton->setIcon(ButtonIcon);
    ui->closeButton->setIconSize(pixmap.rect().size());

    ui->closeButton->setFixedSize(pixmap.rect().size());
}

AlignWidget::~AlignWidget()
{
    delete ui;
}

void AlignWidget::on_closeButton_released()
{
    std::cout << "on_closeButton_released" << std::endl;
    QCoreApplication::quit();
}


void AlignWidget::on_closeButton_clicked()
{
    std::cout << "on_closeButton_clicked" << std::endl;
    QCoreApplication::quit();
}

