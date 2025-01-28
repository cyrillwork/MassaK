#include "driver_plain.h"
#include "driver.h"

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
