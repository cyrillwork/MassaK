#pragma once

#include <cinttypes>
#include <vector>


using Data = std::vector<uint8_t>;

class Protocol
{
public:    
    static void getMassa(Data& buff);
    static void setZero(Data& buff);
    static void setTare(Data& buff);
    static void getScalePar(Data& buff);
    static void print(const Data& buff);

    static bool parseResponseGetMassa(Data& buff);

    static void test_crc();
    static bool check_crc(const Data& buff);

private:
    static void addCRC(uint8_t* data, size_t len);
};
