#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QString>
#include <iostream>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QDesktopWidget>
#else
#include <QScreen>
#endif

#ifdef DEBUG_TEST
static bool is_full_screen = false;
#else
static bool is_full_screen = true;
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(this, &MainWindow::updateMainWidget, this, &MainWindow::on_updateMainWidget);
    connect(this, &MainWindow::showMessageWidget,  this, &MainWindow::on_showMessageWidget);
    connect(this, &MainWindow::lostConnection, this, &MainWindow::on_lostConnection);
    connect(this, &MainWindow::saveToJson, this, &MainWindow::on_saveToJson);

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

    QPalette palette = ui->setZero->palette();
    palette.setColor(QPalette::Button, QColor(Qt::darkRed));
    palette.setColor(QPalette::ButtonText, QColor(Qt::white));
    ui->setZero->setPalette(palette);
    ui->setZero->show();

    QPalette palette2 = ui->setTare->palette();
    palette2.setColor(QPalette::Button, QColor(Qt::darkGreen));
    palette2.setColor(QPalette::ButtonText, QColor(Qt::white));
    ui->setTare->setPalette(palette2);
    ui->setTare->show();

    holdTimer = new QTimer(this);
    holdTimer->setInterval(timeoutUsec);
    connect(holdTimer, &QTimer::timeout, this, &MainWindow::on_holdTimerTimeout);

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

void MainWindow::setJsonFilename(const std::string& name)
{
    json_file_name = name;
}

void MainWindow::on_finishAlignWidget()
{
    std::cout << "on_finishAlignWidget" << std::endl;
    if(Mode == 1) {
        Mode = 2;
        alignWidget->setAlignWidgetType(false, display);
    } else if (Mode == 2) {
        Mode = 0;
        if(alignWidget) {
            alignWidget->hide();
        }
        setVisible(true);

        if(is_full_screen) {
            showFullScreen();
        }
    } else {
        std::cout << "on_finishAlignWidget Error Mode: " << (int)Mode << std::endl;
    }

}

void MainWindow::on_setZero_released()
{
    auto res = Driver::instance().SetZero();
    std::cout << "Set Zero res:" << res << std::endl;
    if(!res) {
        deviceStatus = DeviceStatusType::NoPortAnswer;
        emit lostConnection();
    }
}

void MainWindow::on_setTare_released()
{
    ScalesParameters params;
    Driver::instance().GetScalesParametersStruct(params);

    int32_t tare = params.weight;
    std::cout << "tare: " << tare << std::endl;

    auto res = Driver::instance().SetTare(tare);
    std::cout << "Set Tare res:" << res << std::endl;

    if(!res) {
        deviceStatus = DeviceStatusType::NoPortAnswer;
        emit lostConnection();
    }
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
        std::cout << "info deviceStatus: " << (int)deviceStatus << " calCode: " << display.codeAD << std::endl;

        if( (DeviceStatusType::NoPortAnswer == deviceStatus || deviceStatus == DeviceStatusType::AnswerWithError)
            || (display.codeAD == "") || (display.codeAD == "0") )
        {
            std::cout << "Driver::instance().GetScaleParCheck()"<< std::endl;
            deviceStatus = Driver::instance().GetScaleParCheck(ackScaleParameters);

            if(!json_file_name.empty() && deviceStatus == DeviceStatusType::GetGoodAnswer) {
                emit saveToJson();
            }

            if(DeviceStatusType::NoPortAnswer == deviceStatus || DeviceStatusType::AnswerWithError == deviceStatus) {
                std::cout << "emit showMessageWidget"<< std::endl;
                emit showMessageWidget();
            }
        }

        if( (deviceStatus == DeviceStatusType::GetGoodAnswer) ||
            (deviceStatus == DeviceStatusType::AnswerWithOverWeight) )
        {
            //if(Mode == 0)
            {
                deviceStatus = Driver::instance().GetScalesParameters();

                if( (deviceStatus == DeviceStatusType::GetGoodAnswer) ||
                    (deviceStatus == DeviceStatusType::AnswerWithOverWeight) )
                {
                    Driver::instance().GetScalesParametersStruct(scalesParameters);
                    //std::cout << "emit showCheckingWidget"<< std::endl;
                    emit updateMainWidget();
                } else {
                    emit lostConnection();
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
}

void MainWindow::updateMainWidgetMode0()
{
    if(messageWidget) {
        messageWidget->hide();
    }

    setVisible(true);

    if(is_full_screen) {
        showFullScreen();
    }

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
            //calCode = ::atoi(ptr_calc);
            display.codeAD = std::string(ptr_calc);
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

        if(deviceStatus == AnswerWithOverWeight) {
            ::sprintf(_buff, "> MAX");
        } else {
            if(scalesParameters.weight_stable) {
                ::sprintf(_buff, "%.3f kg", weight);
            } else {
                ::sprintf(_buff, "%.3f", weight);
            }
        }

        QString weight_temp(_buff);
        ui->weightLabel->setText(weight_temp);
    }

    { //set info
        //auto ackScaleParameters.P_Max.find("");
        display.parameters = getDisplayParameters(ackScaleParameters.P_Max, display.weight_clb);
        //std::string str1;
        if(display.parameters == "") {
            display.parameters = ackScaleParameters.P_Max + " " + ackScaleParameters.P_Min + " " + ackScaleParameters.P_e +
                    " " + ackScaleParameters.P_T;
        }

        if(display.parameters.empty()) {
            ui->infoLabel->setText("  ");
        } else {
            QString info_temp(display.parameters.c_str());
            ui->infoLabel->setText(info_temp);
        }
    }

    { //labels
        if(scalesParameters.weight_zero) {
            display.weight_zero = ">0<";
        } else {
            display.weight_zero = "  ";
        }        
        ui->zeroLabel->setText(QString(display.weight_zero.c_str()));

        if(scalesParameters.weight_net) {
            display.weight_net = "Net";
        } else {
            display.weight_net = "  ";
        }

        ui->netLabel->setText(QString(display.weight_net.c_str()));
    }
}

void MainWindow::updateMainWidgetMode1_2()
{
    if(alignWidget) {
        alignWidget->updateWeightInfo(scalesParameters, display);
    }
}

std::string MainWindow::getDisplayParameters(const std::string& p_max, std::string& weight_clb)
{
    weight_clb = " ";
    if(p_max.find("=3/6 kg") != std::string::npos) {
        weight_clb = "6.000 kg";
        return std::string("Max = 3/6kg  Min=20g e= 1/2g  T=-3kg");
    } else if(p_max.find("=6 kg") != std::string::npos) {
        return std::string("Max = 6kg  Min=40g e= 2g  T=-6kg");
    } else if(p_max.find("=6/15 kg") != std::string::npos) {
        weight_clb = "15.000 kg";
        return std::string("Max = 6/15kg  Min=40g e= 2/5g  T=-6kg");
    } else if(p_max.find("=15 kg") != std::string::npos) {
        return std::string("Max = 15kg  Min=100g e= 5g  T=-15kg");
    } else if(p_max.find("=15/32 kg") != std::string::npos) {
        weight_clb = "30.000 kg";
        return std::string("Max = 15/32kg  Min=100g e= 5/10g  T=-15kg");
    } else if(p_max.find("=32 kg") != std::string::npos) {
        return std::string("Max = 32kg  Min=200g e= 10g  T=-32kg");
    }

    return "";
}

void MainWindow::on_closeButton_released()
{
    //close();
    std::cout << "on_closeButton_released" << std::endl;
    QCoreApplication::quit();
}

void MainWindow::on_updateMainWidget()
{
    std::cout << "on_updateMainWidget Mode: " << Mode << std::endl;
    if(Mode == 0) {
        updateMainWidgetMode0();
    } else if((Mode == 1) || (Mode == 2)) {
        updateMainWidgetMode1_2();
    }

}

void MainWindow::on_showMessageWidget()
{
    std::cout << "get MainWindow::on_showMessageWidget" << std::endl;
    hide();

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
    holdTimer->stop();
}

void MainWindow::on_holdTimerTimeout()
{
    std::cout << "on_holdTimerTimeout" << std::endl;
    Mode = 1;
    setVisible(false);

    if(!alignWidget) {
        alignWidget = std::make_unique<AlignWidget>();
        alignWidget->connectMainWindow(this);
    }
    alignWidget->setAlignWidgetType(true, display);
    alignWidget->show();

    if(is_full_screen) {
        alignWidget->showFullScreen();
    }
}

void MainWindow::on_lostConnection()
{
    hide();
    display.clear();
    ackScaleParameters.clear();
    scalesParameters.clear();
}

void MainWindow::on_saveToJson()
{
    auto _params = ackScaleParameters;
    QJsonObject jsonObj;

    jsonObj["P_Max"] = QString::fromStdString(_params.P_Max);
    jsonObj["P_Min"] = QString::fromStdString(_params.P_Min);
    jsonObj["P_e"] = QString::fromStdString(_params.P_e);
    jsonObj["P_T"] = QString::fromStdString(_params.P_T);
    jsonObj["Fix"] = QString::fromStdString(_params.Fix);
    jsonObj["Calcode"] = QString::fromStdString(_params.Calcode);
    jsonObj["PO_Ver"] = QString::fromStdString(_params.PO_Ver);
    jsonObj["PO_Summ"] = QString::fromStdString(_params.PO_Summ);

    QJsonDocument jsonDoc(jsonObj);
    QFile file(QString(json_file_name.c_str()));

    if (file.open(QIODevice::WriteOnly)) {
        file.write(jsonDoc.toJson());
        file.close();
        std::cout << "Struct saved to " << json_file_name << std::endl;
    } else {
        std::cerr << "Failed to open file for writing" << std::endl;
    }
}

void MainWindow::on_logoButton_pressed()
{
    std::cout << "on_logoButton_pressed" << std::endl;
    holdTimer->start();    
}

