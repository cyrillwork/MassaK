#ifndef SCALES_PARAMETERS_H
#define SCALES_PARAMETERS_H

#include <string>
#include <stdint.h>

#define PROGRAM_VERSION "<font color='white'>MK module inspection, версия VF_1.1.0.3 </font>"

struct ScalesParameters
{
    bool connection     = false;
    bool condition      = false;
    bool error          = false;
    int32_t weight      = 0;
    bool weight_stable  = false;
    bool weight_overmax = false;
    bool weight_net     = false;
    bool weight_zero    = false;

    void clear() {
        connection  = false;
        condition   = false;
        error       = false;
        weight = 0;
        weight_stable   = false;
        weight_overmax  = false;
        weight_net      = false;
        weight_zero     = false;
    }
};

struct DeviceParameters
{
    uint32_t    status  = 1;
    std::string p_max;
    std::string calcode;
    int32_t     weight  = 0;
    bool    stable      = false;
    bool    zaro        = false;
    bool    net         = false;
};

struct DisplayParameters
{
    std::string parameters;     // Метрологические параметры ВУ [до 40 знаков]
    std::string weight_txt;     // Величина веса нетто в граммах [5-6 знаков]
    std::string weight_clb;     // Величина веса калибровочной гири в граммах [5-6знаков]
    std::string weight_stable;  // Индикатор стабилизации веса [пробел или "kg"]
    std::string weight_zero;    // Индикатор нуля веса [пробел или ">0<"]
    std::string weight_net;     // Индикатор веса нетто [пробел или "NET"]
    std::string codeAD;         // Код юстировки [шесть знаков]

    void clear() {
        parameters.clear();
        weight_txt.clear();
        weight_clb.clear();
        weight_stable.clear();
        weight_zero.clear();
        weight_net.clear();
        codeAD.clear();
    }
};


#endif
