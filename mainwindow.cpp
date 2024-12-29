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


void MainWindow::on_getWeight_released()
{
    std::cout << "Get Weight" << std::endl;
}


void MainWindow::on_setZero_released()
{
    std::cout << "Set zero" << std::endl;
}

