#include "messageform.h"
#include "ui_messageform.h"

#include <iostream>

MessageForm::MessageForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MessageForm)
{
    ui->setupUi(this);
}

MessageForm::~MessageForm()
{
    delete ui;
}

void MessageForm::setTextAndShow(int deviceStatus)
{
    if(deviceStatus == 1) {
        //Text_25
        ui->messageLabel->setText("Весовое устройство не подключено");
    } else if(deviceStatus == 2) {
        //Text_26
        ui->messageLabel->setText("Весовое устройство неисправно");
    } else {
        ui->messageLabel->setText("Unknown message");
    }

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    show();
}

void MessageForm::on_pushButton_released()
{
    std::cout << "on_pushButton_released" << std::endl;
    QCoreApplication::quit();
}


void MessageForm::on_pushButton_clicked()
{
    std::cout << "on_pushButton_clicked" << std::endl;
    QCoreApplication::quit();

}

