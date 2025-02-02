#include "driver_plain.h"
#include "driver.h"

#include <iostream>

bool GetScalesParameters()
{
    return Driver::instance().GetScalesParameters();
}

bool SetZero()
{
    return Driver::instance().SetZero();
}

bool SetTare(int32_t tare)
{
    return Driver::instance().SetTare(tare);
}

void getScalesParametersStruct(ScalesParameters* get_params)
{
    if(!get_params) {
        std::cout << "!!! Error get_params is NULL" << std::endl;
        return;
    }
    ScalesParameters temp_get_params;
    Driver::instance().getScalesParameters(temp_get_params);
    *get_params = temp_get_params;
}
