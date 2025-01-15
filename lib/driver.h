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

    bool getMassa();
    void setZero();
    void setTare();

    const ScalesParameters& getScalesParameters() const;
    void printScalesParameters() const;

private:
    bool is_run;
    std::unique_ptr<std::thread> main_thread;

    void routine();

    ScalesParameters scalesParameters;
    void resetScaleParameters();
};

