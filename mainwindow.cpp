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
    //std::string str1 = ((params.connection) ? "true" : "false");
    str_info =  "connection:\t"       + std::string(params.connection ? "true" : "false") + "\n" +
                "condition:\t"        + std::string(params.condition ? "true" : "false")      + "\n" +
                "weigth:\t\t"         + std::to_string(params.weight)         + "\n" +
                "weight_stable:\t"    + std::string(params.weight_stable ? "true" : "false")  + "\n" +
                "weight_overmax:\t"   + std::string(params.weight_overmax ? "true" : "false") + "\n" +
                "weight_net:\t"       + std::string(params.weight_net ? "true" : "false")     + "\n" +
                "weight_zero:\t"      + std::string(params.weight_zero ? "true" : "false");

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
