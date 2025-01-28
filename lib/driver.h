#pragma once

#include <thread>
#include <memory>
#include <mutex>

#include "protocol.h"

class Driver
{
public:    
    Driver();
    virtual ~Driver();

    bool GetScalesParameters();
    bool SetZero();
    bool SetTare(int32_t tare = 0);
    void GetScalesParameters(ScalesParameters& get_params);

private:
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

    ScalesParameters scalesParameters;
};

