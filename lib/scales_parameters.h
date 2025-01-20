#ifndef SCALES_PARAMETERS_H
#define SCALES_PARAMETERS_H

#include <stdint.h>

struct ScalesParameters
{
    bool connection     = false;
    bool condition      = false;
    int32_t weight      = 0;
    bool weight_stable  = false;
    bool weight_overmax = false;
    bool weight_net     = false;
    bool weight_zero    = false;
};

#endif
