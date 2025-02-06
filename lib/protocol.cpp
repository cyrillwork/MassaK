#include "protocol.h"
#include "messages.h"

#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <string.h>

#include "aixlog.hpp"

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

void Protocol::setTare(Data& buff, int32_t tare)
{
    size_t len_message = sizeof(SetTare);
    buff.clear();
    buff.reserve(len_message);

    SetTare message;
    message.tare = tare;
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

bool Protocol::parseResponseGetMassa(const Data& buff, ScalesParameters& params)
{
    bool result = false;
    auto len = buff.size();
    if(len <= 7) {
        LOG(INFO) << "Protocol::parseResponse Error too small len: " << len << std::endl;
        return result;
    }

    if(!((buff[0] == 0xf8) && (buff[1] == 0x55) && (buff[2] == 0xce))) {
        LOG(INFO) << "Protocol::parseResponse Error header" << std::endl;
        return result;
    }

    CommonMessage commonMessage(CMD_NONE);

    std::copy(buff.data(), buff.data() + sizeof(CommonMessage),
              (uint8_t*)&commonMessage);

    LOG(INFO) << std::hex << "parseResponseGetMassa command:" << (int)commonMessage.command << std::endl;

    if(commonMessage.command == CMD_ACK_MASSA)
    {
        LOG(INFO) << "CMD_ACK_MASSA" << std::endl;

        AckMassaTare ackMassa;
        if(commonMessage.length == 0x9) {
            std::copy(buff.data(), buff.data() + sizeof(AckMassa),
                      (uint8_t*)&ackMassa);
        } else if(commonMessage.length == 0xd) {
            std::copy(buff.data(), buff.data() + sizeof(AckMassaTare),
                      (uint8_t*)&ackMassa);
        } else {
            LOG(INFO) << std::dec << "len error:" << (int)commonMessage.length << std::endl;
            return result;
        }
        result = true;
        params.connection   = true;
        params.condition    = true;
        params.weight       = ackMassa.weight;
        params.weight_stable  = ackMassa.stable;
        params.weight_overmax = false;
        params.weight_zero    = ackMassa.zero;
        params.weight_net     = ackMassa.net;
    } else if (commonMessage.command == CMD_ERROR) {
        result = true;
        LOG(INFO) << "CMD_ERROR" << std::endl;
        ErrorMessage errorMessage;
        std::copy(buff.data(), buff.data() + sizeof(ErrorMessage),
                  (uint8_t*)&errorMessage);

        LOG(INFO) << std::hex << "errorCode:" << (int)errorMessage.errorCode << std::endl;

        if(errorMessage.errorCode == ErrorCodes::OVER_WEIRGHT) {
            params.connection       = true;
            params.condition        = true;
            params.weight           = 0;//ackMassa.weight;            
            params.weight_stable    = false;//ackMassa.stable;
            params.weight_overmax   = true;
            params.weight_net       = false;//ackMassa.net;
            params.weight_zero      = false;                        
        } else {
            params.connection       = true;
            params.condition        = false;
            params.weight           = 0;
            params.weight_net       = false;
            params.weight_zero      = false;
            params.weight_stable    = false;
            params.weight_overmax   = false;
        }
    } else {
        LOG(INFO) << std::dec << "unknown command " << std::endl;
    }

    return result;
}

bool Protocol::parseResponseSetZero(const Data& buff, ScalesParameters& params)
{
    bool result = false;
    auto len = buff.size();
    if(len <= 7) {
        LOG(INFO) << "Protocol::parseSetZero Error too small len: " << len << std::endl;
        return result;
    }

    if(!((buff[0] == 0xf8) && (buff[1] == 0x55) && (buff[2] == 0xce))) {
        LOG(INFO) << "Protocol::parseSetZero Error header" << std::endl;
        return result;
    }

    CommonMessage commonMessage(CMD_NONE);

    std::copy(buff.data(), buff.data() + sizeof(CommonMessage),
              (uint8_t*)&commonMessage);

    LOG(INFO) << std::hex << "parseResponseSetZero command:" << (int)commonMessage.command << std::endl;

    if(commonMessage.command == CMD_ACK_SET_ZERO) {
        LOG(INFO) << "CMD_ACK_SET_ZERO" << std::endl;
        result = true;
        params.connection       = true;
        params.condition        = true;
        params.weight           = 0;
        params.weight_stable    = true;
        params.weight_overmax   = false;
        params.weight_net       = false;
        params.weight_zero      = true;
    } else if (commonMessage.command == CMD_ERROR) {
        LOG(INFO) << "CMD_ERROR" << std::endl;
        result = true;
        ErrorMessage errorMessage;
        std::copy(buff.data(), buff.data() + sizeof(ErrorMessage),
                  (uint8_t*)&errorMessage);
        LOG(INFO) << std::hex << "errorCode:" << (int)errorMessage.errorCode << std::endl;

        if(errorMessage.errorCode == ErrorCodes::NONE_SET_ZERO) {
            params.connection       = true;
            params.condition        = true;
            params.weight           = 0;//ackMassa.weight;
            params.weight_stable    = false;//ackMassa.stable;
            params.weight_overmax   = true;
            params.weight_net       = false;//ackMassa.net;
            params.weight_zero      = false;
        } else {
            params.connection       = true;
            params.condition        = false;
            params.weight           = 0;
            params.weight_stable    = false;
            params.weight_overmax   = false;
            params.weight_net       = false;
            params.weight_zero      = false;
        }
    } else {
        LOG(INFO) << std::dec << "unknown command " << std::endl;
    }
    return result;
}

bool Protocol::parseResponseSetTare(const Data& buff, ScalesParameters& params)
{
    bool result = false;
    auto len = buff.size();
    if(len <= 7) {
        LOG(INFO) << "Protocol::parseSetTare Error too small len: " << len << std::endl;
        return result;
    }

    if(!((buff[0] == 0xf8) && (buff[1] == 0x55) && (buff[2] == 0xce))) {
        LOG(INFO) << "Protocol::parseSetTare Error header" << std::endl;
        return result;
    }

    CommonMessage commonMessage(CMD_NONE);
    std::copy(buff.data(), buff.data() + sizeof(CommonMessage),
              (uint8_t*)&commonMessage);
    LOG(INFO) << std::hex << "parseResponseSetTare command:" << (int)commonMessage.command << std::dec << std::endl;

    if(commonMessage.command == CMD_ACK_SET_TARE) {
        LOG(INFO) << "CMD_ACK_SET_TARE" << std::endl;
        result = true;
        params.connection       = true;
        params.condition        = true;
        params.weight           = 0;
        params.weight_stable    = true;
        params.weight_overmax   = false;
        params.weight_net       = true;
        params.weight_zero      = false;
    } else if (commonMessage.command == CMD_NACK_SET_TARE) {
        LOG(INFO) << "CMD_NACK_SET_TARE" << std::endl;
        result = true;
        params.connection       = true;
        params.condition        = true;
        params.weight           = 0;//ackMassa.weight;
        params.weight_stable    = false;//ackMassa.stable;
        params.weight_overmax   = false;
        params.weight_net       = false;//ackMassa.net;
        params.weight_zero      = false;
    } else {
        LOG(INFO) << "Scale broken" << std::endl;
        result = true;
        params.connection       = true;
        params.condition        = false;
        params.weight           = 0;
        params.weight_stable    = false;
        params.weight_overmax   = false;
        params.weight_net       = false;
        params.weight_zero      = false;
    }
    return result;
}


bool Protocol::parseResponseGetScalePar(const Data& buff, AckScaleParameters& params)
{
    bool result = false;
    auto len = buff.size();
    if(len <= 7) {
        LOG(INFO) << "Protocol::parseResponseGetScalePar Error too small len: " << len << std::endl;
        return result;
    }

    if(!((buff[0] == 0xf8) && (buff[1] == 0x55) && (buff[2] == 0xce))) {
        LOG(INFO) << "Protocol::parseResponseGetScalePar Error header" << std::endl;
        return result;
    }

    CommonMessage commonMessage(CMD_NONE);

    std::copy(buff.data(), buff.data() + sizeof(CommonMessage),
              (uint8_t*)&commonMessage);

    LOG(INFO) << std::hex << "parseResponseGetScalePar command:" << (int)commonMessage.command << std::endl;
    if(commonMessage.command == CMD_ACK_SCALE_PAR) {        

        auto _get_string = [](char* ptr1, size_t max_size, std::string& res_str) {
            size_t _pos = 0;
            while(true) {
                if(_pos >= max_size) {
                    break;
                }
                if((ptr1[_pos] == 0x0D) && (ptr1[_pos + 1] == 0x0A)) {
                    _pos++;
                    _pos++;
                    break;
                }
                res_str.push_back(ptr1[_pos]);
                _pos++;
            }
            return _pos;
        };

        LOG(INFO) << "CMD_ACK_SCALE_PAR" << std::endl;                
        //char P_Min[21] = {};

        char* ptr1  = (char*)(buff.data()) + 6;
        size_t pos1 = 0;

        LOG(INFO) << std::dec << "pos begin: " << pos1 << std::endl;

        pos1 += _get_string(ptr1, 20, params.P_Max);
        LOG(INFO) << "P_Max: " << params.P_Max << std::endl;
        LOG(INFO) << std::dec << "pos1: " << pos1 << std::endl;

        pos1 += _get_string(ptr1 + pos1, 20, params.P_Min);
        LOG(INFO) << "P_Min: " << params.P_Min << std::endl;
        LOG(INFO) << "pos1: " << pos1 << std::endl;

        pos1 += _get_string(ptr1 + pos1, 10, params.P_e);
        LOG(INFO) << "P_e: " << params.P_e << std::endl;
        LOG(INFO) << "pos1: " << pos1 << std::endl;

        pos1 += _get_string(ptr1 + pos1, 10, params.P_T);
        LOG(INFO) << "P_T: " << params.P_T << std::endl;
        LOG(INFO) << "pos1: " << pos1 << std::endl;

        pos1 += _get_string(ptr1 + pos1, 7, params.Fix);
        LOG(INFO) << "Fix: " << params.Fix << std::endl;
        LOG(INFO) << "pos1: " << pos1 << std::endl;

        pos1 += _get_string(ptr1 + pos1, 12, params.Calcode);
        LOG(INFO) << "Calcode: " << params.Calcode << std::endl;
        LOG(INFO) << "pos1: " << pos1 << std::endl;

        pos1 += _get_string(ptr1 + pos1, 12, params.PO_Ver);
        LOG(INFO) << "PO_Ver: " << params.PO_Ver << std::endl;
        LOG(INFO) << "pos1: " << pos1 << std::endl;

        pos1 += _get_string(ptr1 + pos1, 12, params.PO_Summ);
        LOG(INFO) << "PO_Summ: " << params.PO_Summ << std::endl;
        LOG(INFO) << "pos1: " << pos1 << std::endl;

        result = true;
    } else if(commonMessage.command == CMD_ERROR) {
        LOG(INFO) << "CMD_ERROR" << std::endl;
        params.is_error = true;
        result = true;
    }

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
        LOG(INFO) << "Protocol::check_crc Error too small len: " << len << std::endl;
        return result;
    }

    uint16_t init_crc = 0;
    uint16_t _res = CRC16(init_crc, (uint8_t*)(buff.data() + 5), len - 7);
    uint8_t *ptr1 = (uint8_t*)&_res;

    if((buff[len - 2] == ptr1[0]) && (buff[len - 1] == ptr1[1])) {
        LOG(INFO) << "crc ok" << std::endl;
        result = true;
    } else {
        LOG(INFO) << std::hex << "Protocol::check_crc error crc: " << (int)ptr1[0]
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
