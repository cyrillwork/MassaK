#pragma once

#include <thread>
#include <memory>

class Controller
{
public:
    Controller();
    ~Controller();

    void start();
    void stop();

private:
    bool is_run;
    std::unique_ptr<std::thread> main_thread;

    void routine();
};

