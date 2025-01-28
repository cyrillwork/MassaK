#pragma once

#include <QMainWindow>
#include <thread>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_getMassa_released();
    void on_setZero_released();
    void on_setTare_released();

private:
    Ui::MainWindow *ui;

    void show_info();

    void routine();

    bool is_run;
    std::thread main_thread;
};
