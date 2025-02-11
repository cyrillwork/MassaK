#pragma once

#include <QMainWindow>
#include <QTimer>
#include <thread>
#include <memory>

#include "driver.h"

#include "alignwidget.h"
#include "messageform.h"

//#define DEBUG_TEST

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
    void updateMainWidget();
    void showMessageWidget();

public slots:
    void on_finishAlignWidget();

private slots:
    void on_getMassa_released();
    void on_setZero_released();
    void on_setTare_released();
    void on_closeButton_released();

    void on_updateMainWidget();
    void on_showMessageWidget();

    void on_logoButton_pressed();
    void on_logoButton_released();
    void on_holdTimerTimeout();

private:
    Ui::MainWindow *ui;

#ifdef DEBUG_TEST
    const int32_t timeoutUsec = 3000;
#else
    const int32_t timeoutUsec = 10000;
#endif

    QTimer *holdTimer;

    std::unique_ptr<AlignWidget> alignWidget = nullptr;
    std::unique_ptr<MessageForm> messageWidget = nullptr;

    DeviceStatusType deviceStatus   = DeviceStatusType::NoPortAnswer;
    AckScaleParameters  ackScaleParameters;
    ScalesParameters    scalesParameters;

    int Mode = 0;
    int32_t calCode = 0;
    DisplayParameters display;

    void show_info();

    void routine();

    bool is_run;
    std::unique_ptr<std::thread> main_thread;

    void updateMainWidgetMode0();
    void updateMainWidgetMode1_2();

    std::string getDisplayParameters(const std::string& p_max);
};
