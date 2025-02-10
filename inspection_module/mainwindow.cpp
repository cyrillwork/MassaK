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

    //QPixmap pixmap("logo.png");
    //ui->logoLabel->setPixmap(pixmap);
    QPixmap pixmap1("logo.png");
    QIcon ButtonLogoIcon(pixmap1);
    ui->logoButton->setIcon(ButtonLogoIcon);
    ui->logoButton->setIconSize(pixmap1.rect().size());
    auto rrr1 = pixmap1.rect().size();
    rrr1.setHeight(rrr1.height() + 10);
    rrr1.setWidth(rrr1.width() + 10);
    ui->logoButton->setFixedSize(rrr1);

    QPixmap pixmap2("quit.png");
    QIcon ButtonIcon(pixmap2);
    ui->closeButton->setIcon(ButtonIcon);

    ui->closeButton->setIconSize(pixmap2.rect().size());
    auto rrr2 = pixmap2.rect().size();
    rrr2.setHeight(rrr2.height() + 10);
    rrr2.setWidth(rrr2.width() + 10);
    ui->closeButton->setFixedSize(rrr2);

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

void MainWindow::on_finishAlignWidget()
{
    std::cout << "on_finishAlignWidget" << std::endl;

    auto res = Driver::instance().SetCal(calCode);

    std::cout << "Driver::instance().SetCal(calCode) res: " << res << std::endl;

//    if(alignWidget) {
//        alignWidget->hide();
//    }

//    setVisible(true);
//    showFullScreen();

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
    //std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    while(is_run) {
        std::cout << "info deviceStatus: " << (int)deviceStatus << " calCode: " << calCode << std::endl;

        if(DeviceStatusType::NoPortAnswer == deviceStatus || deviceStatus == DeviceStatusType::AnswerWithError)
        {
            std::cout << "Driver::instance().GetScaleParCheck()"<< std::endl;
            deviceStatus = Driver::instance().GetScaleParCheck(ackScaleParameters);

            if(DeviceStatusType::NoPortAnswer == deviceStatus || DeviceStatusType::AnswerWithError == deviceStatus) {
                std::cout << "emit showMessageWidget"<< std::endl;
                emit showMessageWidget();
            }
        }

        if(deviceStatus == DeviceStatusType::GetGoodAnswer) {
            if(Mode == 0) {
                std::cout << "try get Massa "<< std::endl;
                if(Driver::instance().GetScalesParameters()) {
                    Driver::instance().GetScalesParametersStruct(scalesParameters);
                    std::cout << "emit showCheckingWidget"<< std::endl;
                    emit showCheckingWidget();
                } else {
                    deviceStatus = DeviceStatusType::NoPortAnswer;
                }
            }
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
    if(Mode != 0) {
        return;
    }

    if(messageWidget) {
        messageWidget->hide();
    }

    setVisible(true);
    showFullScreen();

    if(!ackScaleParameters.Calcode.empty()) {
        std::string _temp_str;
        const char *ptr_calc = nullptr;
        auto _pos = ackScaleParameters.Calcode.find('=');

        if(_pos != std::string::npos && ((_pos + 1) < ackScaleParameters.Calcode.size()) ) {
            _temp_str = ackScaleParameters.Calcode.substr(_pos + 1).c_str();
            ptr_calc = _temp_str.c_str();
            ui->calcodeLabel->setText(ptr_calc);
        } else {
            ptr_calc = ackScaleParameters.Calcode.c_str();
            ui->calcodeLabel->setText( ptr_calc );
        }

        if(ptr_calc) {
            calCode = ::atoi(ptr_calc);
        }

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

    { //set Massa
        char _buff[32] = {};
        double weight = scalesParameters.weight * 0.001;

        ::sprintf(_buff, "%.3f kg", weight);

        QString weight_temp(_buff);
        ui->weightLabel->setText(weight_temp);
    }

    { //set info
        std::string str1;
        str1 += ackScaleParameters.P_Max + " " + ackScaleParameters.P_Min + " " + ackScaleParameters.P_e +
                " " + ackScaleParameters.P_T;
        QString info_temp(str1.c_str());
        ui->infoLabel->setText(info_temp);
    }
}

void MainWindow::on_showMessageWidget()
{
    std::cout << "get MainWindow::on_showMessageWidget" << std::endl;
    if(!messageWidget) {
        messageWidget = std::make_unique<MessageForm>();
    }

    messageWidget->setTextAndShow(deviceStatus);
    messageWidget->adjustSize();

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    messageWidget->move(QApplication::desktop()->screen()->rect().center() - messageWidget->rect().center());
#else
    messageWidget->move(QGuiApplication::screens().at(0)->geometry().center() - messageWidget->rect().center());
#endif
    //messageWidget->show();
}

void MainWindow::on_logoButton_released()
{
    std::cout << "on_logoButton_released" << std::endl;
    Mode = 1;
    setVisible(false);

    if(!alignWidget) {
        alignWidget = std::make_unique<AlignWidget>();
        alignWidget->connectMainWindow(this);
    }

    alignWidget->show();
    alignWidget->showFullScreen();
}
