#include "messageform.h"
#include "ui_messageform.h"

#include <iostream>

MessageForm::MessageForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MessageForm)
{
    ui->setupUi(this);

    // QPixmap pixmap("quit.png");
    // QIcon ButtonIcon(pixmap);
    // ui->closeButton->setIcon(ButtonIcon);
    // ui->closeButton->setIconSize(pixmap.rect().size());

    // auto rrr = pixmap.rect().size();
    // rrr.setHeight(rrr.height() + 10);
    // rrr.setWidth(rrr.width() + 10);
    // ui->closeButton->setFixedSize(rrr);
    // ui->closeButton->setFixedSize(rrr);    

    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint );

    //setWindowModality(Qt::WindowModal);
    setWindowModality(Qt::ApplicationModal);
}

MessageForm::~MessageForm()
{
    delete ui;
}

void MessageForm::setTextAndShow(int deviceStatus)
{
    if(deviceStatus == 1) {
        //Text_25
        ui->messageLabel->setText("  Поиск весов  ");
    } else if(deviceStatus == 2) {
        //Text_26
        ui->messageLabel->setText("Весовое устройство неисправно");
    } else {
        ui->messageLabel->setText("Unknown message");
    }

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    //show();
}

void MessageForm::on_closeButton_clicked()
{
    //std::cout << "on_closeButton_clicked" << std::endl;
    QCoreApplication::exit(0);//quit();
}


void MessageForm::on_closeButton_released()
{
    //std::cout << "on_closeButton_released" << std::endl;
    QCoreApplication::exit(0);//quit();
}

