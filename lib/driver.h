#pragma once

#include <thread>
#include <memory>
#include <mutex>

#include "protocol.h"

class Driver
{
public:
    Driver();
    ~Driver();

    void start();
    void stop();

    bool getScalePar();
    bool getMassa();
    bool setZero();
    bool setTare(int32_t tare = 0);

    void getScalesParameters(ScalesParameters& get_params);
    void printScalesParameters();

private:
    bool is_run;
    std::unique_ptr<std::thread> main_thread;

    void routine();

    std::mutex mutexParams;
    ScalesParameters scalesParameters;
    void resetScaleParameters();
    void setConnected();
    void setScalesParameters(const ScalesParameters& params);
};

