#include "alignwidget.h"
#include "ui_alignwidget.h"

#include <iostream>

#include "mainwindow.h"

AlignWidget::AlignWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AlignWidget)
{
    ui->setupUi(this);
    //setWindowState(Qt::WindowFullScreen);
    //showFullScreen();

    QPixmap pixmap("quit.png");
    QIcon ButtonIcon(pixmap);
    ui->closeButton->setIcon(ButtonIcon);
    ui->closeButton->setIconSize(pixmap.rect().size());

    auto rrr = pixmap.rect().size();
    rrr.setHeight(rrr.height() + 10);
    rrr.setWidth(rrr.width() + 10);
    ui->closeButton->setFixedSize(rrr);
}

AlignWidget::~AlignWidget()
{
    delete ui;
}

void AlignWidget::connectMainWindow(QWidget *parent)
{
    connect(this, &AlignWidget::finishAlignWidget, (MainWindow*)parent, &MainWindow::on_finishAlignWidget);
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

void AlignWidget::on_finishButton_released()
{
    std::cout << "on_finishButton_released" << std::endl;
    emit finishAlignWidget();
}

