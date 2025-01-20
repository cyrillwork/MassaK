#pragma once

#include <cinttypes>
#include <vector>

#include "scales_parameters.h"
#include "protocol.h"

using Data = std::vector<uint8_t>;

class Protocol
{
public:    
    static void getMassa(Data& buff);
    static void setZero(Data& buff);
    static void setTare(Data& buff, int32_t tare);
    static void getScalePar(Data& buff);
    static void print(const Data& buff);

    static bool parseResponseGetMassa(const Data& buff, ScalesParameters& params);
    static bool parseResponseSetZero(const Data& buff, ScalesParameters& params);
    static bool parseResponseSetTare(const Data& buff, ScalesParameters& params);
    static bool parseResponseGetScalePar(const Data& buff);

    static void test_crc();
    static bool check_crc(const Data& buff);

private:
    static void addCRC(uint8_t* data, size_t len);
};
