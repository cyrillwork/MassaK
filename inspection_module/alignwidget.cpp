#include "alignwidget.h"
#include "ui_alignwidget.h"

#include <iostream>
#include <QPalette>
#include <QColor>

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


    QPalette palette = ui->finishButton->palette();

    palette.setColor(QPalette::Button, QColor(Qt::darkBlue));
    palette.setColor(QPalette::ButtonText, QColor(Qt::white));
    //palette.setColor(QPalette::Foreground, QColor(Qt.blue));
    //palette.setColor(QPalette::Base, QColor(Qt.blue));
    //palette.setColor(QPalette::AlternateBase, QColor(Qt.blue));
    //palette.setColor(QPalette::ToolTipBase, QColor(Qt.blue));
    //palette.setColor(QPalette::ToolTipText, QColor(Qt.blue));
    //palette.setColor(QPalette::Text, QColor(Qt.blue));
    //palette.setColor(QPalette::Button, QColor(Qt.blue));
    //palette.setColor(QPalette::ButtonText, QColor(Qt.blue));
    //palette.setColor(QPalette::BrightText, QColor(Qt.blue));
    ui->finishButton->setPalette(palette);
    ui->finishButton->show();
}

AlignWidget::~AlignWidget()
{
    delete ui;
}

void AlignWidget::connectMainWindow(QWidget *parent)
{
    connect(this, &AlignWidget::finishAlignWidget, (MainWindow*)parent, &MainWindow::on_finishAlignWidget);
}

void AlignWidget::setAlignWidgetType(bool type)
{
    if(type) {
        ui->titleLabel->setText("Юстировка при нулевой нагрузке");
        ui->finishButton->setText("Продолжить");
    } else {
        ui->titleLabel->setText("Юстировка под нагрузкой");
        ui->finishButton->setText("Готово");
    }
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

bool AlignWidget::getAlignWidgetType() const
{
    return alignType;
}

