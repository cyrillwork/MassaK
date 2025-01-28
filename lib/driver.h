#pragma once

#include <thread>
#include <memory>
#include <mutex>

#include "protocol.h"

ScalesParameters scalesParameters;

class Driver
{
public:    
    Driver();
    virtual ~Driver();

    bool GetScalesParameters();
    bool SetZero();
    bool SetTare(int32_t tare = 0);

private:
    bool getScalePar();

    void start();
    void stop();

    bool checkPortGetMassa();

    bool is_run;
    std::unique_ptr<std::thread> main_thread;

    void routine();

    std::mutex mutexParams;
    void resetScaleParameters();
    void setConnected();
    void setScalesParameters(const ScalesParameters& params);
};

