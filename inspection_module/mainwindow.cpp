#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QString>
#include <QDesktopWidget>

#include "driver_plain.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    connect(this, &MainWindow::showCheckingWidget, this, &MainWindow::on_showCheckingWidget);

    connect(this, &MainWindow::showMessageWidget,  this, &MainWindow::on_showMessageWidget);

    QPixmap pixmap("logo.png");
    ui->logoLabel->setPixmap(pixmap);

    setVisible(false);

    is_run = true;
    main_thread = std::make_unique<std::thread>(&MainWindow::routine, this);

}

MainWindow::~MainWindow()
{
    if(main_thread && main_thread->joinable()) {
        is_run = false;
        main_thread->join();
    }
    delete ui;
}

void MainWindow::on_getMassa_released()
{
    std::cout << "Get Massa" << std::endl;    
    GetScalesParameters();
}

void MainWindow::on_setZero_released()
{
    std::cout << "Set Zero" << std::endl;
    SetZero();
}

void MainWindow::on_setTare_released()
{
    checkingWidget = new CheckingWidget();
    checkingWidget->show();
//    std::cout << "Set Tare" << std::endl;
//    //auto tare = ui->tareBox->value();
//    //std::cout << "tare: " << tare << std::endl;
//    int32_t tare = 0;
//    SetTare(tare);
}

void MainWindow::show_info()
{
    std::string str_info;
    ScalesParameters params;
    getScalesParametersStruct(&params);
    //std::string str1 = ((params.connection) ? "true" : "false");
    str_info =  "connection:\t"       + std::string(params.connection ? "true" : "false") + "\n" +
                "condition:\t"        + std::string(params.condition ? "true" : "false")      + "\n" +
                "weigth:\t\t"         + std::to_string(params.weight)         + "\n" +
                "weight_stable:\t"    + std::string(params.weight_stable ? "true" : "false")  + "\n" +
                "weight_overmax:\t"   + std::string(params.weight_overmax ? "true" : "false") + "\n" +
                "weight_net:\t"       + std::string(params.weight_net ? "true" : "false")     + "\n" +
                "weight_zero:\t"      + std::string(params.weight_zero ? "true" : "false");

    //QString _temp(str_info.c_str());
    //ui->scaleInfo->setText(_temp);
}

void MainWindow::routine()
{
    static bool first_run = true;
    static int  counter = 0;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    while(is_run) {
        std::cout << "show_info counter: " << counter  << std::endl;

        if(first_run) {
            first_run = false;

            std::cout << "emit showMessageWidget"<< std::endl;
            emit showMessageWidget();
        }

        counter++;
        if(counter == 10) {
            //messageForm->close();
            std::cout << "emit showCheckingWidget"<< std::endl;
            emit showCheckingWidget();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

}

void MainWindow::on_closeButton_released()
{
    //close();
    std::cout << "on_closeButton_released" << std::endl;
    QCoreApplication::quit();
}

void MainWindow::on_showCheckingWidget()
{
    std::cout << "get MainWindow::on_showCheckingWidget" << std::endl;
    if(messageWidget) {
        messageWidget->hide();
    }

    setVisible(true);
    showFullScreen();
}

void MainWindow::on_showMessageWidget()
{
    std::cout << "get MainWindow::on_showMessageWidget" << std::endl;

    messageWidget = new MessageForm();

    messageWidget->adjustSize();
    messageWidget->move(QApplication::desktop()->screen()->rect().center() - messageWidget->rect().center());

    //messageWidget->show();
    messageWidget->setTextAndShow(1);

}



