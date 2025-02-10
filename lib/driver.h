#pragma once

#include <thread>
#include <memory>
#include <mutex>

#include "protocol.h"

class SetCalP;
class SetCal;
enum DeviceStatusType: uint32_t
{
    GetGoodAnswer        = 0,
    NoPortAnswer         = 1,
    AnswerWithError      = 2,
    AnswerWithOverWeight = 3
};

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
    void GetScalesParametersStruct(ScalesParameters& get_params);
    bool SetCal(int32_t cal_code);
    bool SetCalP(int32_t w_cal);

    DeviceStatusType GetScaleParCheck(AckScaleParameters& params);

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

