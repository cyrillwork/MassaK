#pragma once

#include <QMainWindow>
#include <QTimer>
#include <thread>
#include <memory>

#include "driver.h"

#include "alignwidget.h"
#include "messageform.h"

//#define DEBUG_TEST
//#define DEBUG_SHOW_MAIN

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(const std::string& name, QWidget *parent = nullptr);
    ~MainWindow();

    void setJsonFilename(const std::string& name);
signals:
    void updateMainWidget();
    void showMessageWidget();
    void lostConnection();
    void saveToJson();

public slots:
    void on_finishAlignWidget();
    void on_closeButton_released();

private slots:
    void on_setZero_released();
    void on_setTare_released();

    void on_updateMainWidget();
    void on_showMessageWidget();

    void on_logoButton_pressed();
    void on_logoButton_released();
    void on_holdTimerTimeout();

    void on_lostConnection();
    void on_saveToJson();

private:
    Ui::MainWindow *ui;

#ifdef DEBUG_SHOW_MAIN
    const int32_t timeoutUsec = 1000;
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
    bool isFinishAlign = false;
    bool needUpdateParams = false;
    int32_t calCode = 0;
    DisplayParameters display;

    std::string json_file_name;
    std::string port_name;

    void show_info();
    void routine();

    bool is_run;
    std::unique_ptr<std::thread> main_thread;

    void updateMainWidgetMode0();
    void updateMainWidgetMode1_2();

    int32_t w_cal = 0;
    std::string getDisplayParameters(const std::string& p_max, std::string& weight_clb);
    void resetSET_CAL();

    bool verbose = false;
};
