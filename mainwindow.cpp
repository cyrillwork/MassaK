#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

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
}

void MainWindow::on_setZero_released()
{
    std::cout << "Set Zero" << std::endl;
}

void MainWindow::on_setTare_released()
{
    std::cout << "Set Tare" << std::endl;
}

void MainWindow::on_startButton_released()
{
    std::cout << "Start" << std::endl;
    controller.start();
}

void MainWindow::on_stopButton_released()
{
    std::cout << "Stop" << std::endl;
    controller.stop();
}
