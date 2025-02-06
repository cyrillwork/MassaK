#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QString>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QDesktopWidget>
#else
#include <QScreen>
#endif

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
    Driver::instance().GetScalesParameters();
}

void MainWindow::on_setZero_released()
{
    std::cout << "Set Zero" << std::endl;
    Driver::instance().SetZero();
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
    Driver::instance().GetScalesParametersStruct(params);
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
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    while(is_run) {
        std::cout << "info deviceStatus: " << (int)deviceStatus << std::endl;

        if(DeviceStatusType::NoPortAnswer == deviceStatus || deviceStatus == DeviceStatusType::AnswerWithError)
        {
            std::cout << "Driver::instance().GetScaleParCheck()"<< std::endl;
            deviceStatus = Driver::instance().GetScaleParCheck(ackScaleParameters);

            if(DeviceStatusType::NoPortAnswer == deviceStatus || DeviceStatusType::AnswerWithError == deviceStatus) {
                std::cout << "emit showMessageWidget"<< std::endl;
                emit showMessageWidget();
            }
        }

        if(DeviceStatusType::GetGoodAnswer == deviceStatus) {
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

    if(!ackScaleParameters.Calcode.empty()) {
        QString calcode_temp(ackScaleParameters.Calcode.c_str());
        ui->calcodeLabel->setText(calcode_temp);
    } else {
        ui->calcodeLabel->setText("  ");
    }


    if(!ackScaleParameters.PO_Ver.empty()) {
        QString po_ver_temp(ackScaleParameters.PO_Ver.c_str());
        ui->PoVerLabel->setText(po_ver_temp);
    } else {
        ui->PoVerLabel->setText("  ");
    }

    if(!ackScaleParameters.PO_Summ.empty()) {
        QString po_summ_temp(ackScaleParameters.PO_Summ.c_str());
        ui->PoSummLabel->setText(po_summ_temp);
    } else {
        ui->PoSummLabel->setText("   ");
    }

    //showFullScreen();
}

void MainWindow::on_showMessageWidget()
{
    std::cout << "get MainWindow::on_showMessageWidget" << std::endl;

    messageWidget = new MessageForm();    
    messageWidget->setTextAndShow(deviceStatus);

    messageWidget->adjustSize();

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    messageWidget->move(QApplication::desktop()->screen()->rect().center() - messageWidget->rect().center());
#else
    messageWidget->move(QGuiApplication::screens().at(0)->geometry().center() - messageWidget->rect().center());
#endif

    //messageWidget->show();


}

