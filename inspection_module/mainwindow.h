#pragma once

#include <QMainWindow>
#include <thread>
#include <memory>

#include "driver.h"

#include "alignwidget.h"
#include "messageform.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void showCheckingWidget();
    void showMessageWidget();

private slots:
    void on_getMassa_released();
    void on_setZero_released();
    void on_setTare_released();
    void on_closeButton_released();


    void on_showCheckingWidget();
    void on_showMessageWidget();

    void on_logoButton_released();

private:
    Ui::MainWindow *ui;

    std::unique_ptr<AlignWidget> checkingWidget = nullptr;
    std::unique_ptr<MessageForm> messageWidget;

    DeviceStatusType deviceStatus   = DeviceStatusType::NoPortAnswer;
    AckScaleParameters  ackScaleParameters;
    ScalesParameters    scalesParameters;

    int Mode = 0;

    void show_info();

    void routine();

    bool is_run;
    std::unique_ptr<std::thread> main_thread;
};
