#pragma once

#include <thread>
#include <memory>

struct ScalesParameters
{
    bool connection = false;
    bool condition  = false;
    uint32_t weight = 0;
    bool weight_stable = false;
    bool weight_overmax = false;
    bool weight_net = false;
    bool weight_zero = false;
};

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

    void routine();

    ScalesParameters scalesParameters;
};

