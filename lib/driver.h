#pragma once

#include <thread>
#include <memory>
#include <mutex>

#include "protocol.h"

class Driver
{
public:        
    static Driver& instance() {
        static Driver _instance;
        return _instance;
    }
    virtual ~Driver();

    bool GetScalesParameters();
    bool SetZero();
    bool SetTare(int32_t tare = 0);
    void getScalesParameters(ScalesParameters& get_params);

private:    
    Driver();

    bool checkPortGetMassa();
    bool search_port();

    std::mutex mutexParams;
    void resetScaleParameters();
    void setConnected();
    void setScalesParameters(const ScalesParameters& params);

    ScalesParameters scalesParameters;
};

