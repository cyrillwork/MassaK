#include "protocol.h"
#include "messages.h"
//#include "boost/crc.hpp"

#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <string.h>

uint16_t CRC16(uint16_t crc, uint8_t* buf, size_t len)
{
    uint16_t bits, k;
    uint16_t accumulator, temp;
    for(k = 0; k<len; k++) {
        accumulator = 0;
        temp = (crc>>8)<<8;
        for(bits = 0; bits < 8; bits++) {
            if( (temp ^ accumulator) & 0x8000 ) {
                accumulator = (accumulator << 1) ^ 0x1021;
            } else {
                accumulator <<= 1;
            }
            temp <<= 1;
        }
        crc = accumulator^(crc<<8)^(buf[k]&0xff);
    }
    return crc;
}

void Protocol::getMassa(Data& buff)
{
    size_t len_message = sizeof(GetMassa);
    buff.clear();
    buff.reserve(len_message);

    GetMassa message;
    uint8_t* ptr1 = (uint8_t*)&message;
    addCRC(ptr1, len_message);
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
    std::cout << std::dec << std::endl;
}

bool Protocol::parseResponseGetMassa(Data& buff)
{
    bool result = false;
    auto len = buff.size();
    if(len <= 7) {
        std::cout << "Protocol::parseResponse Error too small len: " << len << std::endl;
        return result;
    }

    if(!((buff[0] == 0xf8) && (buff[1] == 0x55) && (buff[2] == 0xce))) {
        std::cout << "Protocol::parseResponse Error header" << std::endl;
        return result;
    }

    CommonMessage commonMessage(CMD_ACK_MASSA);

    std::copy(buff.data(), buff.data() + sizeof(CommonMessage),
              (uint8_t*)&commonMessage);

    std::cout << std::hex << "parseResponseGetMassa command:" << (int)commonMessage.command << std::endl;

    return result;
}

void Protocol::test_crc()
{
    //proper crc 0x7d, 0x42
    Data buff = {0xf8, 0x55, 0xce, 0xd, 0x0, 0x24, 0xf, 0x0, 0x0, 0x0, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
//First test
//    uint16_t _crc = 0;
//    uint16_t _res = CRC16(_crc, buff.data() + 5, buff.size() - 7);
//    uint8_t *ptr1 = (uint8_t*)&_res;
//    std::cout << std::hex << "crc1: " << (int)ptr1[0] << "\n";
//    std::cout << std::hex << "crc2: " << (int)ptr1[1] << "\n";
//    std::cout << std::dec;

//Second test
    addCRC(buff.data(), buff.size());
    print(buff);
    std::cout << "Good CRC: 0x7d 0x42" << std::endl;
}

bool Protocol::check_crc(const Data& buff)
{
    bool result = false;
    auto len = buff.size();
    if(len <= 7) {
        std::cout << "Protocol::check_crc Error too small len: " << len << std::endl;
        return result;
    }

    uint16_t init_crc = 0;
    uint16_t _res = CRC16(init_crc, (uint8_t*)(buff.data() + 5), len - 7);
    uint8_t *ptr1 = (uint8_t*)&_res;

    if((buff[len - 2] == ptr1[0]) && (buff[len - 1] == ptr1[1])) {
        std::cout << "crc ok" << std::endl;
        result = true;
    } else {
        std::cout << std::hex << "Protocol::check_crc error crc: " << (int)ptr1[0]
                  << " " << (int)ptr1[1] << std::endl;
    }
    return result;
}

void Protocol::addCRC(uint8_t* data, size_t len)
{
//    boost::crc_ccitt_true_t  _res;
//    _res.process_bytes(data + 5, len - 7);
//    *(uint16_t*)(data + len - 2) = _res.checksum();
    if(len <= 7) {
        std::cout << "Protocol::addCRC Error too small len: " << len << std::endl;
        return;
    }

    uint16_t _crc = 0;
    uint16_t _res = CRC16(_crc, data + 5, len - 7);
    uint8_t *ptr1 = (uint8_t*)&_res;
    data[len - 2] = ptr1[0];
    data[len - 1] = ptr1[1];
}
