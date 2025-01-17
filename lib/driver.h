#pragma once

#include <thread>
#include <memory>

#include "protocol.h"

class Driver
{
public:
    Driver();
    ~Driver();

    void start();
    void stop();

    void getMassa();
    void setZero();
    void setTare();

    const ScalesParameters& getScalesParameters() const;
    void printScalesParameters() const;

private:
    bool is_run;
    std::unique_ptr<std::thread> main_thread;

    bool need_getMassa;
    bool need_setZero;
    bool need_setTare;

    bool getScalePar();
    bool m_getMassa();
    void m_setZero();
    void m_setTare();

    void routine();

    ScalesParameters scalesParameters;
    void resetScaleParameters();
    void setConnected();
    void setScalesParameters(const ScalesParameters& params);
};

