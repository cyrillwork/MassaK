#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_getMassa_released()
{
    std::cout << "Get Massa" << std::endl;    

    controller.getMassa();

    show_info();
}

void MainWindow::on_setZero_released()
{
    std::cout << "Set Zero" << std::endl;
    controller.setZero();
}

void MainWindow::on_setTare_released()
{
    std::cout << "Set Tare" << std::endl;
    controller.setTare();
}

//void MainWindow::on_startButton_released()
//{
//    std::cout << "Start" << std::endl;
//    controller.start();
//}

//void MainWindow::on_stopButton_released()
//{
//    std::cout << "Stop" << std::endl;
//    controller.stop();
//}

void MainWindow::show_info()
{
    std::string str_info;
    auto params = controller.getScalesParameters();
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
