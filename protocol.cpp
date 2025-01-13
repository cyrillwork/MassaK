#include "protocol.h"
#include "messages.h"
#include "boost/crc.hpp"

#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <string.h>

void Protocol::getMassa(Data& buff)
{
    size_t len_message = sizeof(GetMassa);
    buff.clear();
    buff.reserve(len_message);

    GetMassa message;
    uint8_t* ptr1 = (uint8_t*)&message;
    //addCRC(ptr1, len_message);
    std::copy(ptr1, ptr1 + len_message, back_inserter(buff));
}

void Protocol::setZero(Data& buff)
{
    size_t len_message = sizeof(SetZero);
    buff.clear();
    buff.reserve(len_message);

    SetZero message;
    uint8_t* ptr1 = (uint8_t*)&message;
    addCRC(ptr1, len_message);
    std::copy(ptr1, ptr1 + len_message, back_inserter(buff));
}

void Protocol::setTare(Data& buff)
{
    size_t len_message = sizeof(SetTare);
    buff.clear();
    buff.reserve(len_message);

    SetTare message;
    uint8_t* ptr1 = (uint8_t*)&message;
    addCRC(ptr1, len_message);
    std::copy(ptr1, ptr1 + len_message, back_inserter(buff));
}

void Protocol::getScalePar(Data& buff)
{
    size_t len_message = sizeof(GetScalePar);
    buff.clear();
    buff.reserve(len_message);

    GetScalePar message;
    uint8_t* ptr1 = (uint8_t*)&message;
    addCRC(ptr1, len_message);
    std::copy(ptr1, ptr1 + len_message, back_inserter(buff));
}

void Protocol::print(const Data& buff)
{
    for (const auto iii: buff) {
        std::cout << std::hex << (int)iii << " ";
    }
    std::cout << std::dec << "\n";
}

void Protocol::test_crc()
{
//    test_main();

//    //proper crc 0x7d, 0x42
//    Data buff = {0xf8, 0x55, 0xce, 0xd, 0x0, 0x24, 0xf, 0x0, 0x0, 0x0, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
//    //addCRC(buff.data() , buff.size());
//    calc_crc(buff.data() + 5, buff.size() - 7);

//    std::cout << std::hex << "crc: " << (int)good_crc << "\n";
//    std::cout << std::dec << "\n";
//    print(buff);
}

void Protocol::addCRC(uint8_t* data, size_t len)
{
    boost::crc_ccitt_true_t  _res;
    _res.process_bytes(data + 5, len - 7);
    *(uint16_t*)(data + len - 2) = _res.checksum();
}
