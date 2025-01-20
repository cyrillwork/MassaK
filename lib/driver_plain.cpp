#include "driver_plain.h"
#include "driver.h"

void start()
{
    Driver::instance().start();
}

void stop()
{
    Driver::instance().stop();
}

bool getScalePar()
{
    return Driver::instance().getScalePar();
}

bool getMassa()
{
    return Driver::instance().getMassa();
}

bool setZero()
{
    return Driver::instance().setZero();
}

bool setTare(int32_t tare)
{
    return Driver::instance().setTare(tare);
}

void getScalesParameters(ScalesParameters* get_params)
{
    ScalesParameters temp_get_params;
    Driver::instance().getScalesParameters(temp_get_params);
    *get_params = temp_get_params;
}
