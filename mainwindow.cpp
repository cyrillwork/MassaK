#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , is_run{true}
    , main_thread{&MainWindow::routine, this}
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    if(main_thread.joinable()) {
        is_run = false;
        main_thread.join();
    }
    delete ui;
}

void MainWindow::on_getMassa_released()
{
    std::cout << "Get Massa" << std::endl;    

    controller.getMassa();

    //show_info();
}

void MainWindow::on_setZero_released()
{
    std::cout << "Set Zero" << std::endl;
    controller.setZero();
}

void MainWindow::on_setTare_released()
{
    std::cout << "Set Tare" << std::endl;

    auto tare = ui->tareBox->value();
    std::cout << "tare: " << tare << std::endl;

    controller.setTare(tare);
}

void MainWindow::show_info()
{
    std::string str_info;
    ScalesParameters params;
    controller.getScalesParameters(params);
    str_info =  "connection: "      + std::to_string(params.connection)     + "\n" +
                "condition: "       + std::to_string(params.condition)      + "\n" +
                "weigth: "          + std::to_string(params.weight)         + "\n" +
                "weight_stable:"    + std::to_string(params.weight_stable)  + "\n" +
                "weight_overmax:"   + std::to_string(params.weight_overmax) + "\n" +
                "weight_net:"       + std::to_string(params.weight_net)     + "\n" +
                "weight_zero:"      + std::to_string(params.weight_zero);

    QString _temp(str_info.c_str());
    ui->scaleInfo->setText(_temp);
}

void MainWindow::routine()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    while(is_run) {
        //std::cout << "show_info" << std::endl;
        show_info();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
