#pragma once

#include <cinttypes>
#include <vector>

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


using Data = std::vector<uint8_t>;


class Protocol
{
public:    
    static void getMassa(Data& buff);
    static void setZero(Data& buff);
    static void setTare(Data& buff);
    static void getScalePar(Data& buff);
    static void print(const Data& buff);

    static bool parseResponseGetMassa(Data& buff, ScalesParameters& params);

    static void test_crc();
    static bool check_crc(const Data& buff);

private:
    static void addCRC(uint8_t* data, size_t len);
};
