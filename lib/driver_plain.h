#ifndef DRIVER_PLAIN_H
#define DRIVER_PLAIN_H

#include "scales_parameters.h"

#include <stdio.h>

extern "C" {

    bool GetScalesParameters();

    bool SetZero();

    bool SetTare(int32_t tare = 0);

    void GetScalesParametersStruct(ScalesParameters* get_params);
}


#endif
