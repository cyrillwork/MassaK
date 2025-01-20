#ifndef DRIVER_PLAIN_H
#define DRIVER_PLAIN_H

#include "scales_parameters.h"

#include <stdio.h>

extern "C" {
    void start();

    void stop();

    bool getScalePar();

    bool getMassa();

    bool setZero();

    bool setTare(int32_t tare = 0);

    void getScalesParameters(ScalesParameters* get_params);
}


#endif
